#include <ros/callback_queue.h>
#include <ros/ros.h>
#include <std_msgs/String.h>

#include <boost/thread.hpp>

/**
 * TODO: 问题是:
 * 主线程与子线程的ID不一致：
 * 现象是：1. 主线程调用main_queue+custom_queue，两个函数里的id都为主线程
 *         2. 子线程调用main_queue+custom_queue，两个函数里的id都为子线程
 *         3. ......
 *
 * 可能的原因：对比listener.cpp:
 * 本程序的订阅的声明绑定在了两个不同的NodeHandle上
 */

void chatterCallbackMainQueue(const std_msgs::String::ConstPtr &msg) {
  ROS_INFO_STREAM("I heard:[" << msg->data << "]in thread["
                              << boost::this_thread::get_id()
                              << "](Main thread)");
}

void chatterCallbackCustomQueue(const std_msgs::String::ConstPtr &msg) {
  ROS_INFO_STREAM("I heard:[" << msg->data << "] in thread ["
                              << boost::this_thread::get_id()
                              << "](Sub thread)");
}

void sub_thread_fun() {
  ros::NodeHandle n;

  ROS_INFO_STREAM("Sub thread id=" << boost::this_thread::get_id());

  ros::Subscriber sub =
      n.subscribe("/multi_thread/chatter", 10, chatterCallbackCustomQueue);

  ros::Rate rate(4);
  while (n.ok()) {
    ros::spinOnce();
    rate.sleep();
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "multi_thread_listener");
  ros::NodeHandle nh;

  ros::Subscriber sub =
      nh.subscribe("/multi_thread/chatter", 10, chatterCallbackMainQueue);

  boost::thread chatter_thread(sub_thread_fun);

  ROS_INFO_STREAM("Main thread id=" << boost::this_thread::get_id());

  ros::Rate rate(1);

  while (nh.ok()) {
    ros::spinOnce();
    rate.sleep();
  }

  chatter_thread.join();
}
