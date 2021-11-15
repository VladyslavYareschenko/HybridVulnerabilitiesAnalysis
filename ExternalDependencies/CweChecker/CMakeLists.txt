include(ExternalProject)

set(CWE_CHECKER_SRC_DIR "cwe_checker_lib")
set(CWE_CHECKER_HOME_PATH "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_COMPONENT_NAME}")
set(CWE_CHECKER_SRC_PATH "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_SRC_DIR}")
set(CWE_CHECKER_BUILD_PATH "${CMAKE_BINARY_DIR}/${CWE_CHECKER_COMPONENT_NAME}")

# Dilemma:
# 1) An external shared library can be configured during the root project configuration, in which case we can easily
#    find it at the configuration step and link it. But it looks a bit hacky.
# 2) An external shared library can be configured during compilation,
#    but than we need to hardcode its path at the configuration step.  It seems that this option is not better...
configure_file("ExternalCMakeProject/CMakeLists.txt.in" "ExternalCMakeProject/CMakeLists.txt")
set(EXTERNAL_CMAKE_PROJECT_DIR "ExternalCMakeProject")
set(EXTERNAL_CMAKE_PROJECT_WORKING_DIR "${CMAKE_CURRENT_BINARY_DIR}/${EXTERNAL_CMAKE_PROJECT_DIR}")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}"
        "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}" WORKING_DIRECTORY "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}"
        )

execute_process(COMMAND "${CMAKE_COMMAND}" --build
        "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}" WORKING_DIRECTORY "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}"
        )