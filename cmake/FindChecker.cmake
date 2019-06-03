
set(ROOT_PATH ${PROJECT_SOURCE_DIR}/examples/PolicyChecker)

set(POLICY_CHECKER_LIB
        ${ROOT_PATH}/src/PolicyReader.cpp
        ${ROOT_PATH}/src/ValidatePolicy.cpp
        ${ROOT_PATH}/src/PomdpPolicyChecker.cpp
        )
#set(POLICY_CHECKER_INCLUDE
#        ${ROOT_PATH}/include/PolicyReader.h
#        ${ROOT_PATH}/include/ValidatePolicy.h
#        ${ROOT_PATH}/include/PomdpPolicyChecker.h
#        )


find_path(POLICY_CHECKER_INCLUDE
        NAMES PolicyReader.h ValidatePolicy.h PomdpPolicyChecker.h
        PATH_SUFFIXES include
        PATHS ${ROOT_PATH}
)


find_library(POLICY_CHECKER_LIBRARY
        NAMES Checker
        PATH_SUFFIXES src
        PATHS ${ROOT_PATH}
)

# set z3 include directory and lib path
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_PATH}/lib)
include_directories(
        /usr/local/include

        )
link_directories(/usr/local/lib)
add_library(Checker ${POLICY_CHECKER_LIB})
target_link_libraries(Checker z3)
target_include_directories(Checker PUBLIC ${POLICY_CHECKER_INCLUDE})