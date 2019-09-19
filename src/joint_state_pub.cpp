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

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <sensor_msgs/msg/joint_state.hpp>

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class JointStatePublisher : public rclcpp::Node
{
public:
  JointStatePublisher()
  : Node("joint_state_pub"), count_(0)
  {
    publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);
    timer_ = this->create_wall_timer(
      0.1s, std::bind(&JointStatePublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = sensor_msgs::msg::JointState();
    message.name = {"leg_left_1_joint", "leg_right_1_joint", "leg_left_2_joint", "leg_right_2_joint", "leg_left_3_joint", "leg_right_3_joint"};
    message.header.stamp = this->get_clock()->now();
    message.position = {pos_, -pos_, pos_*0.2, pos_*-0.2, pos_*0.2, pos_*-0.2};
    message.velocity = {0,0,0,0,0,0};
    message.effort = {0,0,0,0,0,0};
    publisher_->publish(message);
    pos_ < 0.8 ? pos_ += 0.01 : pos_ = 0.01;
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
  size_t count_;
  double pos_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointStatePublisher>());
  rclcpp::shutdown();
  return 0;
}
