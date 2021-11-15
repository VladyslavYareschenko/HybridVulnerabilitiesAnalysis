message(STATUS "Processing AnalysisCaller")

add_library(AnalysisCaller AnalysisCaller.cpp AnalysisCaller.h Private/Utils.cpp Private/Utils.h)
target_link_libraries(AnalysisCaller ${CWE_CHECKER_LIB} Qt5::Core)