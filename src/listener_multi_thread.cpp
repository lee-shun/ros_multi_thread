#include <boost/thread.hpp>
#include <ros/ros.h>
#include <std_msgs/String.h>

ros::Duration dur(0.01);

class Listener {
public:
  void chatter_func1(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO_STREAM("chatter_func1: [" << msg->data << "] [thread="
                                       << boost::this_thread::get_id() << "]");
    dur.sleep();
  }
  void chatter_func2(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO_STREAM("chatter_func2: [" << msg->data << "] [thread="
                                       << boost::this_thread::get_id() << "]");
    dur.sleep();
  }
  void chatter_func3(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO_STREAM("chatter_func3: [" << msg->data << "] [thread="
                                       << boost::this_thread::get_id() << "]");
    dur.sleep();
  }
};

void chatter_func4(const std_msgs::String::ConstPtr &msg) {
  ROS_INFO_STREAM("chatter_func4: [" << msg->data << "] [thread="
                                     << boost::this_thread::get_id() << "]");
  dur.sleep();
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;

  Listener l;
  ros::Subscriber sub1 =
      n.subscribe("/multi_thread/chatter", 10, &Listener::chatter_func1, &l);
  ros::Subscriber sub2 =
      n.subscribe("/multi_thread/chatter", 10, &Listener::chatter_func2, &l);
  ros::Subscriber sub3 =
      n.subscribe("/multi_thread/chatter", 10, &Listener::chatter_func3, &l);
  ros::Subscriber sub4 =
      n.subscribe("/multi_thread/chatter", 10, chatter_func4);

  /**
   * The MultiThreadedSpinner object allows you to specify a number of threads
   * to use to call callbacks.  If no explicit # is specified, it will use the #
   * of hardware threads available on your system.  Here we explicitly specify 4
   * threads.
   */
  ros::MultiThreadedSpinner s(4);
  ros::spin(s);

  return 0;
}
