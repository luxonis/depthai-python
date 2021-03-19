import sys
import os
import re


project_root = os.path.dirname(__file__)
version_depthai_core_path = project_root + "/" + "depthai-core/CMakeLists.txt"
version_depthai_python_path = project_root + "/" + "CMakeLists.txt"

cmake_lists_txt_version_pattern = r'project[\s]*\([^Vv]*version[\s]+((\"(?P<ver1>\S*)\"|(?P<ver2>\S*)\s))'

def get_version_from_cmake_lists(path):
    with open(path, 'r') as file:
        content = file.read()
        match = re.search(cmake_lists_txt_version_pattern, content, flags=re.IGNORECASE)  
        ver1 = match.group('ver1')
        ver2 = match.group('ver2')
        version = ver1
        if ver1 == None:
            version = ver2
        return version

def get_package_version():
    
    version_core = '0.0.0'
    version_revision = '0'
    version_core = get_version_from_cmake_lists(version_depthai_core_path)
    version_revision = get_version_from_cmake_lists(version_depthai_python_path)
    package_version = version_core + '.' + version_revision

    return package_version


def get_package_dev_version(commit_hash):
    return get_package_version() + ".dev+" + commit_hash