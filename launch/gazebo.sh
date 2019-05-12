#!/bin/bash
export ROS_ENV_LOADER="$HOME/.kavrakilab/setup.bash"
terminator -T "test-fetch-simulation-full world_name:=move_base_demo_fetch.sdf prepare_robot_pick_place:=false mapping:=true vicon_bridge:=true vicon_bridge_params_file:=move_base_pick_place_gmapping_localization.yaml moveit:=true gui:=false &
test-fetch-common-move-base sim_file:=_sim"