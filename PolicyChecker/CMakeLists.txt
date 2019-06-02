cmake_minimum_required(VERSION 3.5.1)
project(PolicyChecker)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
add_executable(PolicyChecker main.cpp
        src/PolicyReader.cpp
        include/PolicyReader.h
        src/ValidatePolicy.cpp
        include/ValidatePolicy.h
        src/PomdpPolicy.cpp
        include/PomdpPolicy.h)

target_link_libraries(${PROJECT_NAME} z3)

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