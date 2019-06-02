
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(Checker REQUIRED)
message(WARNING ${PROJECT_SOURCE_DIR})
add_executable(PolicyChecker
        main.cpp
        )

include_directories(/usr/local/include
        ${POLICY_CHECKER_INCLUDE}
        )
link_directories(/usr/local/lib)


target_link_libraries(PolicyChecker ${POLICY_CHECKER_LIB})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core)
get_target_property(QtCore_location Qt5::Core LOCATION)
target_link_libraries(PolicyChecker  Qt5::Core Qt5::Widgets
        z3
        )