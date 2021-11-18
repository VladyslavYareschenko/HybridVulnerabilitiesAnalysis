message(STATUS "Processing Qt GUI")

add_library(GUI MainWindow.cpp MainWindow.h)
target_link_libraries(GUI AnalysisCaller Qt5::Core Qt5::Widgets)

include_directories("${CMAKE_SOURCE_DIR}")