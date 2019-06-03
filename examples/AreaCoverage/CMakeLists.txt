message(STATUS __HAMILTONIAN_NODE__)
set(Hamiltonian_INCLUDE_PATH
        include/HamiltonianCycle.h
        )

## Declare a C++ library
 add_library(Hamiltonian_LIB
   src/HamiltonianCycle.cpp
 )

link_directories(
        src/HamiltonianCycle.cpp
)

include_directories(${Hamiltonian_INCLUDE_PATH})
 add_executable(hamiltonian_node
         hamiltonian_node.cpp
         )

## Specify libraries to link a library or executable target against
 target_link_libraries(hamiltonian_node
   ${catkin_LIBRARIES}
         Hamiltonian_LIB
 )

