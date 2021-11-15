message(STATUS "Searching for ${QT_COMPONENT_NAME} in the development environment.")

find_package(${QT_COMPONENT_NAME} COMPONENTS Widgets Gui Core REQUIRED)