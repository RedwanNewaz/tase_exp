cmake_minimum_required(VERSION 3.5.1)
project(tase_exp)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  control_msgs
  geometric_shapes
  geometry_msgs
  moveit_core
  moveit_ros_planning
  moveit_ros_planning_interface
  pluginlib
  tf2
  tf2_eigen
  tf2_ros
  visualization_msgs
  fetch_move_base_msgs

)

include_directories(include)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES tase_exp
#  CATKIN_DEPENDS control_msgs geometric_shapes geometry_msgs moveit_core moveit_ros_planning moveit_ros_planning_interface pluginlib tf2 tf2_eigen tf2_ros
#  DEPENDS system_lib
)

###########
## Build ##
###########
add_subdirectory(PolicyChecker)
## Specify additional locations of header files
## Your package locations should be listed before other locations
find_package(Qt5Core REQUIRED)
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/tase_exp.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
 add_executable(${PROJECT_NAME}_node
         src/tase_exp_node.cpp
         src/MotionPrimitives.cpp
         include/MotionPrimitives.h
         src/StateEstimator.cpp
         include/StateEstimator.h
         include/HamiltonianCycle.h
         include/TrajPlanner.h
         include/VizTraj.h
         src/PomdpPolicy.cpp
         include/PomdpPolicy.h
         )

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
 target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
         Qt5::Core
 )

