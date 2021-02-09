# Builds library at 'build/' and prepares sphinx configuration at 'build/docs/conf.py'
set(project_root "${CMAKE_CURRENT_LIST_DIR}/../..")

# Copy docs/source/ contents to docs/readthedocs/ folder
file(COPY "${project_root}/docs/source/" DESTINATION ${CMAKE_CURRENT_LIST_DIR})

execute_process(COMMAND ${CMAKE_COMMAND} -D DEPTHAI_PYTHON_BUILD_DOCS:BOOL=YES -D DEPTHAI_BUILD_DOCS:BOOL=YES -S . -B build WORKING_DIRECTORY ${project_root} COMMAND_ECHO STDOUT)
execute_process(COMMAND ${CMAKE_COMMAND} --build build --parallel WORKING_DIRECTORY ${project_root} COMMAND_ECHO STDOUT)