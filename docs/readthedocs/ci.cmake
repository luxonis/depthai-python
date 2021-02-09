# Builds library at 'build/' and prepares sphinx configuration at 'build/docs/conf.py'
set(project_root "${CMAKE_CURRENT_LIST_DIR}/../..")

# Copy docs/source folder to docs/readthedocs
file(COPY "${project_root}/docs/source" DESTINATION "${project_root}/docs/readthesource")

execute_process(COMMAND ${CMAKE_COMMAND} -D DEPTHAI_PYTHON_BUILD_DOCS:BOOL=YES -D DEPTHAI_BUILD_DOCS:BOOL=YES -S . -B build WORKING_DIRECTORY ${project_root} COMMAND_ECHO STDOUT)
execute_process(COMMAND ${CMAKE_COMMAND} --build build --target depthai WORKING_DIRECTORY ${project_root} COMMAND_ECHO STDOUT)