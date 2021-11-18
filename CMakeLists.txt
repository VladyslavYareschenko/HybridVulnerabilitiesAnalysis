cmake_minimum_required(VERSION 3.15)
project (cxx_project_with_rust_lib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

set(EXTERNAL_DEPENDENCIES_SRC_DIR "ExternalDependencies")
include("${EXTERNAL_DEPENDENCIES_SRC_DIR}/SetupExternalDependencies.cmake")

configure_file ("Config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/Config.h")
include_directories (${CMAKE_CURRENT_BINARY_DIR})

add_executable(HybridAnalyzer main.cpp)
add_subdirectory(AnalysisCaller)
add_subdirectory(GUI)
add_dependencies(HybridAnalyzer ${EXTERNAL_DEPENDENCIES_SRC_DIR})
target_link_libraries(HybridAnalyzer GUI Qt5::Widgets)