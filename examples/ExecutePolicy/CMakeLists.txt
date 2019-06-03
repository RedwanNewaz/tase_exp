message(STATUS __OFF__POLICY__)

 add_executable(offpolicy_node
         offpolicy_node.cpp
         src/ConditionalPlan.cpp include/ConditionalPlan.h)

## Specify libraries to link a library or executable target against
 target_link_libraries(offpolicy_node
   ${catkin_LIBRARIES}

 )

