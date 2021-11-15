cmake_minimum_required(VERSION 3.15)
project (cxx_project_with_rust_lib)
set(CMAKE_CXX_STANDARD 14)

set(EXTERNAL_DEPENDENCIES_SRC_DIR "ExternalDependencies")
include("${EXTERNAL_DEPENDENCIES_SRC_DIR}/SetupExternalDependencies.cmake")
add_subdirectory(AnalysisCaller)

add_executable(HybridAnalyzer main.cpp)
add_dependencies(HybridAnalyzer ${EXTERNAL_DEPENDENCIES_SRC_DIR})
target_link_libraries(HybridAnalyzer AnalysisCaller Qt5::Widgets)