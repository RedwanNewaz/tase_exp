
set(ROOT_PATH ${PROJECT_SOURCE_DIR}/examples/PolicyChecker)

set(POLICY_CHECKER_LIB
        ${ROOT_PATH}/src/PolicyReader.cpp
        ${ROOT_PATH}/src/ValidatePolicy.cpp
        ${ROOT_PATH}/src/PomdpPolicy.cpp
        )
set(POLICY_CHECKER_INCLUDE
        ${ROOT_PATH}/include/PolicyReader.h
        ${ROOT_PATH}/include/ValidatePolicy.h
        ${ROOT_PATH}/include/PomdpPolicy.h
        )

# set z3 include directory and lib path
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(CHECKERD ${POLICY_CHECKER_LIB})
target_link_libraries(CHECKERD z3)
target_include_directories(CHECKERD PUBLIC ${POLICY_CHECKER_INCLUDE})