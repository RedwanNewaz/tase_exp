cmake_minimum_required(VERSION 3.5.1)
project(PolicyChecker)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(Checker REQUIRED)
add_executable(PolicyChecker
        main.cpp
        )

target_link_libraries(${PROJECT_NAME} Checker)

## Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
## Instruct CMake to run moc automatically when needed
#set(CMAKE_AUTOMOC ON)
## Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)
#
## Find the QtWidgets library
#find_package(Qt5Widgets CONFIG REQUIRED)
#find_package(Qt5Core)
#get_target_property(QtCore_location Qt5::Core LOCATION)
#target_link_libraries(${PROJECT_NAME}  Qt5::Core Qt5::Widgets)