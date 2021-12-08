message(STATUS "Processing AnalysisCaller")

add_library(AnalysisCaller AnalysisCaller.cpp AnalysisCaller.h Private/Utils.cpp Private/Utils.h)
target_link_libraries(AnalysisCaller Qt5::Core)

include_directories("${CMAKE_SOURCE_DIR}")