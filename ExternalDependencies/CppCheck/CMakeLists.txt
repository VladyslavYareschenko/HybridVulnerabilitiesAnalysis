include(ExternalProject)

add_custom_target(${CPP_CHECK_COMPONENT_NAME})

set(CPP_CHECK_SRC_DIR "cppcheck_lib")
set(CPP_CHECK_HOME_PATH "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_COMPONENT_NAME}")
set(CPP_CHECK_SRC_PATH "${CPP_CHECK_HOME_PATH}/${CPP_CHECK_SRC_DIR}")
set(CPP_CHECK_BUILD_PATH "${CMAKE_BINARY_DIR}/${CPP_CHECK_COMPONENT_NAME}")

# Dilemma:
# 1) An external executable can be found during the root project configuration, in which case we will know its location
#    at the configuration and compilation respectively. But it looks a bit hacky.
# 2) An external executable can be found at runtime.  It seems that this option is not better...
configure_file("ExternalCMakeProject/CMakeLists.txt.in" "ExternalCMakeProject/CMakeLists.txt")
set(EXTERNAL_CMAKE_PROJECT_DIR "ExternalCMakeProject")
set(EXTERNAL_CMAKE_PROJECT_WORKING_DIR "${CMAKE_CURRENT_BINARY_DIR}/${EXTERNAL_CMAKE_PROJECT_DIR}")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}"
        "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}" WORKING_DIRECTORY "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}"
)

execute_process(COMMAND "${CMAKE_COMMAND}" --build
        "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}" WORKING_DIRECTORY "${EXTERNAL_CMAKE_PROJECT_WORKING_DIR}"
)
