cmake_minimum_required(VERSION 2.8.3)
project(ros_serial_node)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  serial
  std_msgs
)

generate_messages(
   DEPENDENCIES
   std_msgs
 )

catkin_package(
  CATKIN_DEPENDS
    serial
    std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(ros_serial_node src/ros_serial_node.cpp)

target_link_libraries(ros_serial_node
  ${catkin_LIBRARIES}
)

