cmake_minimum_required(VERSION 3.15)
project (cxx_project_with_rust_lib)
set(CMAKE_CXX_STANDARD 14)

set(EXTERNAL_DEPENDENCIES_SRC_DIR "ExternalDependencies")
include("${EXTERNAL_DEPENDENCIES_SRC_DIR}/SetupExternalDependencies.cmake")

add_executable(cxx_project_with_rust_lib main.cpp)
add_dependencies(cxx_project_with_rust_lib ${CPP_CHECK_COMPONENT_NAME})
target_link_libraries(cxx_project_with_rust_lib ${CWE_CHECKER_COMPONENT_NAME} Qt5::Widgets)