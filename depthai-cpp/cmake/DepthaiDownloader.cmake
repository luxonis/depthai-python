# This script downloads depthai device side artifacts

function(DepthaiDownload)

    ### VARIABLES
    # Artifactory 
    set(DEPTHAI_BASE_URL "https://artifacts.luxonis.com/artifactory")

    # Repositories
    set(DEPTHAI_REPO_SNAPSHOT "luxonis-myriad-snapshot-local")
    set(DEPTHAI_REPO_RELEASE "luxonis-myriad-release-local")

    # Prefix
    set(DEPTHAI_ARTIFACT_PREFIX "depthai-device-side")

    # Errors and retry count
    set(DEPTHAI_TIMEOUT_S 120)
    set(DEPTHAI_INACTIVE_TIMEOUT_S 30)
    set(DEPTHAI_DOWNLOAD_RETRY_NUM 5)
    ### END VARIABLES


    # PARSE ARGUMENTS
    set(_download_patch_only OFF)

    #if first argument PATCH_ONLY, second must be either ON/OFF
    if("${ARGV0}" STREQUAL "PATCH_ONLY")
        if( ${ARGV1} )
            set(_download_patch_only ON)
        else()
            set(_download_patch_only OFF)
        endif()

        set(folder "${ARGV2}")
        set(output_list_var "${ARGV3}")
        set(maturity "${ARGV4}")
        set(commit "${ARGV5}")
        set(version "${ARGV6}") #optional

    else()
        
        set(folder "${ARGV0}")
        set(output_list_var "${ARGV1}")
        set(maturity "${ARGV2}")
        set(commit "${ARGV3}")
        set(version "${ARGV4}") #optional

    endif("${ARGV0}" STREQUAL "PATCH_ONLY")

    if(_download_patch_only)
        message(STATUS "Downloading depthai and patch")
    else()
        message(STATUS "Downloading depthai, depthai-usb2 and patch")
    endif()

    # END PARSE ARGUMENTS

    #DEBUG
    #message(STATUS "folder ${folder}")
    #message(STATUS "maturity ${maturity}")
    #message(STATUS "commit ${commit}")
    #message(STATUS "version ${version}") #optional

    string(TOLOWER "${maturity}" matority_lower)

    # Switch between maturity
    if(${matority_lower} STREQUAL "snapshot")
        set(_selected_repo "${DEPTHAI_REPO_SNAPSHOT}")

        # Create download directory string
        string(CONFIGURE "@DEPTHAI_BASE_URL@/@DEPTHAI_REPO_SNAPSHOT@/@DEPTHAI_ARTIFACT_PREFIX@/@commit@" _download_directory_url)

        # Create _version_commit_identifier
        set(_version_commit_identifier "${commit}")

    elseif(${matority_lower} STREQUAL "release")
        set(_selected_repo "${DEPTHAI_REPO_RELEASE}")

        # TODO
        # Create download directory string
        #string(CONFIGURE "@DEPTHAI_BASE_URL@/@DEPTHAI_REPO_SNAPSHOT@/@DEPTHAI_ARTIFACT_PREFIX@/@_commit_hash@" _download_directory_url)
        
    else()
        # Not a recognized maturity level
        message(FATAL_ERROR "Cannot download DepthAI Device Side binaries. Maturity level not recognized")
        return()        
    endif(${matority_lower} STREQUAL "snapshot")
    
    # Prints error message
    macro(PrintErrorMessage status)
        if(${status} EQUAL 22)
            message(STATUS "Resource not found, check if commit hash is correctly specified.\n")
        elseif(${status} EQUAL 28)
            message(STATUS "Timeout.\n")
        else()
            message(STATUS "Unknown error.\n")
        endif()
    endmacro() 

    # Download files 
    function(DownloadAndChecksum url url_checksum output status_var)

        # Retry again if failed
        set(_num_retries_left ${DEPTHAI_DOWNLOAD_RETRY_NUM})
        # Set error by default
        set("${status_var}" "1" PARENT_SCOPE)

        while(NOT ${_num_retries_left} EQUAL 0)
            math(EXPR _num_retries_left "${_num_retries_left} - 1")

            # Download checksum first
            file(DOWNLOAD "${url_checksum}" "${output}.checksum" STATUS _status TIMEOUT ${DEPTHAI_TIMEOUT_S})
            # Read checksum to file
            file(READ "${output}.checksum" _file_checksum)
            string(REGEX REPLACE "\n$" "" _file_checksum "${_file_checksum}")
            # Remove checksum file
            file(REMOVE "${output}.checksum")
            #CHECKS
            list(GET _status 0 _status_num)
            if(${_status_num})
                message(STATUS "Status error: ${_status}")
                set("${status_var}" "${_status_num}" PARENT_SCOPE)
                continue()
            endif()


            # Download file and validate checksum
            file(DOWNLOAD "${url}" "${output}" INACTIVITY_TIMEOUT ${DEPTHAI_INACTIVE_TIMEOUT_S} STATUS _status TIMEOUT ${DEPTHAI_TIMEOUT_S} SHOW_PROGRESS EXPECTED_HASH "SHA256=${_file_checksum}" )

            #CHECKS
            list(GET _status 0 _status_num)
            if(${_status_num})
                message(STATUS "Status error: ${_status}")
                set("${status_var}" "${_status_num}" PARENT_SCOPE)
                continue()
            endif()

            # If no errors happened, set status to 0
            set("${status_var}" "0" PARENT_SCOPE)
            # And break the loop
            break()

        endwhile()

    endfunction()


    # depthai.cmd
    message(STATUS "Downloading and checking depthai.cmd")
    DownloadAndChecksum(
        "${_download_directory_url}/depthai-${_version_commit_identifier}.cmd" # File
        "${_download_directory_url}/depthai-${_version_commit_identifier}.sha256.checksum" # File checksum
        "${folder}/depthai.cmd"
        status
    )
    if(${status})
        message(STATUS "\nCouldn't download depthai.cmd\n")
        PrintErrorMessage(${status})
        message(FATAL_ERROR "Aborting.\n")
    endif()
    # add depthai.cmd to list
    list(APPEND "${output_list_var}" "${folder}/depthai.cmd")
    

    if(NOT _download_patch_only)
        # depthai-usb2.cmd
        message(STATUS "Downloading and checking depthai-usb2.cmd")
        DownloadAndChecksum(
            "${_download_directory_url}/depthai-usb2-${_version_commit_identifier}.cmd" # File
            "${_download_directory_url}/depthai-usb2-${_version_commit_identifier}.sha256.checksum" # File checksum
            "${folder}/depthai-usb2.cmd"
            status
        )

        if(${status})
            message(STATUS "\nCouldn't download depthai-usb2.cmd.\n")
            PrintErrorMessage(${status})
            message(FATAL_ERROR "Aborting.\n")
        endif()

        # add depthai-usb2.cmd to list
        list(APPEND "${output_list_var}" "${folder}/depthai-usb2.cmd")

    endif(NOT _download_patch_only)

    # depthai-usb2-patch.patch
    message(STATUS "Downloading and checking depthai-usb2-patch.patch")
    DownloadAndChecksum(
        "${_download_directory_url}/depthai-usb2-patch-${_version_commit_identifier}.patch" # File
        "${_download_directory_url}/depthai-usb2-patch-${_version_commit_identifier}.sha256.checksum" # File checksum
        "${folder}/depthai-usb2-patch.patch"
        status
    )
    if(${status})
        message(STATUS "\nCouldn't download depthai-usb2-patch.patch.\n")
        PrintErrorMessage(${status})
        message(FATAL_ERROR "Aborting.\n")
    endif()    

    # add depthai-usb2.cmd to list
    list(APPEND "${output_list_var}" "${folder}/depthai-usb2-patch.patch")
    set("${output_list_var}" "${${output_list_var}}" PARENT_SCOPE)

endfunction()


function(DepthaiLocal patch_only patch_only_on_off output_dir output_list_var path1 path2 path3)

    #debug
    message(STATUS "patch only: ${patch_only}\non/off${patch_only_on_off}\noutput dir: ${output_dir}\nout_var: ${output_list_var}\npath1: ${path1}\npath2: ${path2}\npath3: ${path3}")

    # If patch only
    if(patch_only_on_off)
        if(NOT path1 OR NOT path3)
            message(FATAL_ERROR "Both depthai binary and depthai usb2 patch files must be specified")
        else()
            configure_file("${path1}" "${output_dir}/depthai.cmd" COPYONLY)
            configure_file("${path3}" "${output_dir}/depthai-usb2-patch.patch" COPYONLY)
            
            # Set output list
            list(APPEND _resource_list "${output_dir}/depthai.cmd" "${output_dir}/depthai-usb2-patch.patch")

        endif()
    else()
        if(NOT path1 OR NOT path2)
            message(FATAL_ERROR "Both depthai and depthai usb2 binary files must be specified")
        else()
            configure_file("${path1}" "${output_dir}/depthai.cmd" COPYONLY)
            configure_file("${path2}" "${output_dir}/depthai-usb2.cmd" COPYONLY)

            # Set output list
            list(APPEND _resource_list "${output_dir}/depthai.cmd" "${output_dir}/depthai-usb2.cmd")

        endif()
    endif()

    set("${output_list_var}" "${_resource_list}" PARENT_SCOPE)

endfunction()