
# install_vcpkg(<vcpkg commit, branch, or tag>)
# Installs vcpkg into the ${VCPKG_ROOT} directory.
function(install_vcpkg vcpkgCommit)
    if(NOT EXISTS ${VCPKG_ROOT})
        message("Cloning vcpkg in ${VCPKG_ROOT}")
        execute_process(COMMAND git clone https://github.com/Microsoft/vcpkg.git ${VCPKG_ROOT})
    else()

    if (DEFINED vcpkgCommit)
        execute_process(COMMAND git checkout ${vcpkgCommit} WORKING_DIRECTORY ${VCPKG_ROOT})
    endif()

    # The following command has no effect if the vcpkg repository is in a detached head state.
    message(STATUS "Auto-updating vcpkg in ${VCPKG_ROOT}")
    execute_process(COMMAND git pull WORKING_DIRECTORY ${VCPKG_ROOT})
    endif()

    if(NOT EXISTS ${VCPKG_ROOT}/README.md)
    message(FATAL_ERROR "***** FATAL ERROR: Could not clone vcpkg *****")
    endif()

    if(WIN32)
    set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe)
    set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
    else()
    set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
    set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
    endif()

    if(NOT EXISTS ${VCPKG_EXEC})
    message(STATUS "Bootstrapping vcpkg in ${VCPKG_ROOT}")
    execute_process(COMMAND ${VCPKG_BOOTSTRAP} WORKING_DIRECTORY ${VCPKG_ROOT})
    endif()

    if(NOT EXISTS ${VCPKG_EXEC})
    message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
    endif()

    #set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endfunction()

# set_vcpkg_dependencies(<package name>...)
# Adds a list of dependencies to download, build and install in the vcpkg instance located at ${VCPKG_ROOT}.
function(set_vcpkg_dependencies)
    #set(VCPKG_ROOT ${CMAKE_SOURCE_DIR}/vcpkg)
    set(VCPKG_DEPENDENCIES ${ARGN})
    set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
    message(STATUS "Checking dependencies in ${VCPKG_ROOT} for platform ${VCPKG_TARGET_TRIPLET}")
    execute_process(COMMAND ${VCPKG_EXEC} install ${VCPKG_DEPENDENCIES} --triplet ${VCPKG_TARGET_TRIPLET} WORKING_DIRECTORY ${VCPKG_ROOT})
endfunction()
