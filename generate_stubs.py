import sys
import subprocess
import re
import tempfile
import os
import textwrap

# Usage
if len(sys.argv) < 3:
    print(f"Usage: {sys.argv[0]} [module_name] [library_dir]")
    exit(-1)

MODULE_NAME = sys.argv[1]
DIRECTORY = sys.argv[2]

print(f'Generating stubs for module: "{MODULE_NAME}" in directory: "{DIRECTORY}"')

try:

    # Create stubs, add PYTHONPATH to find the build module
    # CWD to to extdir where the built module can be found to extract the types
    env = os.environ
    env['PYTHONPATH'] = f'{DIRECTORY}{os.pathsep}{env.get("PYTHONPATH", "")}'

    # Test importing depthai after PYTHONPATH is specified
    try:
        import depthai
    except Exception as ex:
        print(f'Could not import depthai: {ex}')

    print(f'PYTHONPATH set to {env["PYTHONPATH"]}')
    # Check if stubgen has the `--include-docstrings` flag
    includeDocstrings = False
    output = subprocess.check_output(['stubgen', '--help'], env=env)
    if b'--include-docstrings' in output:
        includeDocstrings = True
        print("Will include docstrings in stubs")
    else:
        print("Will not include docstrings in stubs")
    parameters = ['stubgen', '-p', MODULE_NAME, '-o', f'{DIRECTORY}']
    if includeDocstrings:
        parameters.insert(1, '--include-docstrings')
    subprocess.check_call(parameters, cwd=DIRECTORY, env=env)

    # Add py.typed
    open(f'{DIRECTORY}/depthai/py.typed', 'a').close()

    # imports and overloads
    with open(f'{DIRECTORY}/depthai/__init__.pyi' ,'r+') as file:
        # Read
        contents = file.read()

        # Add imports
        stubs_import = textwrap.dedent('''
            # Ensures that the stubs are picked up - thanks, numpy project
            from depthai import (
                node as node,
            )

            import typing
            json = dict
            from pathlib import Path
            from typing import Set, Type, TypeVar
            T = TypeVar('T')
        ''') + contents

        # Create 'create' overloads
        nodes = re.findall('def \S*\(self\) -> node.(\S*):', stubs_import)
        overloads = ''
        for node in nodes:
            overloads = overloads + f'\\1@overload\\1def create(self, arg0: typing.Type[node.{node}]) -> node.{node}: ...'
        final_stubs = re.sub(r"([\s]*)def create\(self, arg0: object\) -> Node: ...", f'{overloads}', stubs_import)

        final_lines = []
        for line in final_stubs.split('\n'):
            if 'class Pipeline:' in line:
                final_lines.append(line)
                final_lines.append('    @overload')
                final_lines.append('    def create(self, arg0: Type[T], *args, **kwargs) -> T: ...')
                continue
            match = re.match(r'^(    def getCvFrame\(self\) -> )object(:.*)$', line)
            if match:
                final_lines.append(f"{match.group(1)}numpy.ndarray{match.group(2)}")
                continue
            final_lines.append(line)

        final_stubs = '\n'.join(final_lines)
        # Writeout changes
        file.seek(0)
        file.truncate(0)
        file.write(final_stubs)

    # node fixes
    with open(f'{DIRECTORY}/depthai/node.pyi' ,'r+') as file:
        # Read
        contents = file.read()

        # Add imports
        stubs_import = textwrap.dedent('''
            from pathlib import Path
            from typing import Set
        ''') + contents

        # Remove import depthai.*
        final_stubs = re.sub(r"import depthai\.\S*", "", stubs_import)

        # Writeout changes
        file.seek(0)
        file.truncate(0)
        file.write(final_stubs)

    # Flush previous stdout
    sys.stdout.flush()

    # Check syntax (Mypy and later Pylance/Pyright)
    # Windows limitation - another process cannot normally read temporary file that is opened by this process
    # Close first and delete manually afterwards
    try:
        config = tempfile.NamedTemporaryFile(mode='w', delete=False)
        config.write('[mypy]\nignore_errors = True\n')
        config.close()
        print(f'Mypy config file: {config.name}')
        # Mypy check
        subprocess.check_call([sys.executable, '-m' 'mypy', f'{DIRECTORY}/{MODULE_NAME}', f'--config-file={config.name}'], env=env)
    finally:
        os.unlink(config.name)

    # # TODO(thamarpe) - Pylance / Pyright check
    # subprocess.check_call([sys.executable, '-m' 'pyright', f'{DIRECTORY}/{MODULE_NAME}'], env=env)

except subprocess.CalledProcessError as err:
    exit(err.returncode)

exit(0)
