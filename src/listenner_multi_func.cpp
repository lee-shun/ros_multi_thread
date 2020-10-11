#include <ros/ros.h>
#include <std_msgs/String.h>

class Listener {
public:
  void chatter_func1(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO("chatter1:[%s]", msg->data.c_str());
  }

  void chatter_func2(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO("chatter2:[%s]", msg->data.c_str());
  }

  void chatter_func3(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO("chatter3:[%s]", msg->data.c_str());
  }

  void chatter_func4(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO("chatter4:[%s]", msg->data.c_str());
  }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "listener_multi_func");
  ros::NodeHandle n;

  Listener l;

  ros::Subscriber sub1 =
      n.subscribe("/multi_thread/chatter", 10, &Listener::chatter_func1, &l);
  ros::Subscriber sub2 =
      n.subscribe("/multi_thread/chatter", 10, &Listener::chatter_func2, &l);
  ros::Subscriber sub3 =
      n.subscribe("/multi_thread/chatter", 10, &Listener::chatter_func3, &l);
  ros::Subscriber sub4 =
      n.subscribe("/multi_thread/chatter", 10, &Listener::chatter_func4, &l);

  ros::spin();
  return 0;
}
