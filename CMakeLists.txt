cmake_minimum_required(VERSION 3.8)
project(robot_localization_test)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(robot_localization REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(map_to_ll_publisher_node src/map_to_ll_publisher_node.cpp)
ament_target_dependencies(map_to_ll_publisher_node
  rclcpp
  robot_localization
  nav_msgs
  sensor_msgs
  tf2
  tf2_ros
)
target_link_libraries(map_to_ll_publisher_node 
  Eigen3::Eigen
)

install(
  TARGETS
    map_to_ll_publisher_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
