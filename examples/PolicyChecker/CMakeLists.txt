

#find_package(Checker)
#add_executable(PolicyChecker main.cpp )
#target_link_libraries(PolicyChecker Checker ${POLICY_CHECKER_LIB})

#message(WARNING ${PROJECT_SOURCE_DIR})



link_directories(ThirdParty/lib)

set(HEADERS
        include/PolicyReader.h
        include/PomdpPolicyChecker.h
        include/ValidatePolicy.h
        ../../include/FetchActionClient.h)
set(SOURCES
        src/PolicyReader.cpp
        src/PomdpPolicyChecker.cpp
        src/ValidatePolicy.cpp
        )
include_directories(
        ThirdParty/include
        ${PROJECT_SOURCE_DIR}/examples/PolicyChecker/include

)
add_executable(PolicyChecker
        main.cpp ${SOURCES} ${HEADERS}
        )


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
        ${catkin_LIBRARIES}
        )

