// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/time_source.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform.hpp>
#include <tf2/LinearMath/Quaternion.h>

using namespace std::chrono_literals;

std::shared_ptr<rclcpp::Node> node;
std::shared_ptr<tf2_ros::TransformBroadcaster> tf_bc_ptr;
std::shared_ptr<rclcpp::Clock> _clock;

bool trigger = true;
void timer_callback()
{
  auto tf_msg = geometry_msgs::msg::TransformStamped();
  tf2::Quaternion qtn;
  if (trigger){
    qtn.setRPY(30,0,0);
  }
  else{
    qtn.setRPY(45,30,0);
  }
  trigger = !trigger;
  tf_msg.transform.rotation.w = qtn.w();
  tf_msg.transform.rotation.x = qtn.x();
  tf_msg.transform.rotation.y = qtn.y();
  tf_msg.transform.rotation.z = qtn.z();

  tf_msg.header.stamp = _clock->now();
  tf_msg.header.frame_id = "base_link";
  tf_msg.child_frame_id = "leg_left_1_link";
  tf_bc_ptr->sendTransform(tf_msg);
}


int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("tf_pub_node");
  rclcpp::TimeSource ts(node);
  _clock = std::make_shared<rclcpp::Clock>(RCL_ROS_TIME);  
  ts.attachClock(_clock);
  tf_bc_ptr = std::make_shared<tf2_ros::TransformBroadcaster>(node);
  //set to rotate about z axis 30deg
  // auto qtn = geometry_msgs::msg::Quaternion();
  // qtn.w = 0.966;
  // qtn.x = 0;
  // qtn.y = 0;
  // qtn.z = 0.259;
  // auto transform = geometry_msgs::msg::Transform();
  // transform.set__rotation(qtn);
  // auto tf_msg = geometry_msgs::msg::TransformStamped();
  // tf_msg.set__transform(transform);
  // tf_bc.sendTransform(tf_msg);

  auto timer = node->create_wall_timer(1s, &timer_callback);

  rclcpp::spin(node);
  return 0;
}