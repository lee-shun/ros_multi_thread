#include<ros/ros.h>
#include<ros/callback_queue.h>
#include<std_msgs/String.h>

#include<boost/thread.hpp>

/**
  * this tutorial demonstrates the use of custom sparate callback queues that can
  * be processed independently, whether in different threads or just at different times.
  */

void chatterCallbackMainQueue(const std_msgs::String::ConstPtr &msg) {
  ROS_INFO_STREAM("I heard :[" << msg->data << "]in thread["
                               << boost::this_thread::get_id()
                               << "](Main thread)");
}

void chatterCallbackCustomQueue(const std_msgs::String::ConstPtr &msg) {
  ROS_INFO_STREAM("I heard: [ " << msg->data << "] in thread ["
                                << boost::this_thread::get_id() << "]");
}

/**
 * the custom queue used for one of the suscription callbacks
 */
ros::CallbackQueue g_queue;/*!!!global var*/
void callbackThread() {

  ROS_INFO_STREAM("Call back thread id is: " << boost::this_thread::get_id());
  ros::NodeHandle n;
  while (ros::ok()) {
    /*timeot*/
    g_queue.callAvailable(ros::WallDuration(0.01));
  }
}

int main(int argc, char **argv){
    ros::init(argc,argv,"multi_thread_listener");
    ros::NodeHandle n;

    /**
     * The SubscribeOptions structure lets you specify a custom queue to use
     * for a specific subscription.
     * You can also set a default queue on a NodeHandle using the
     * NodeHandle::setCallbackQueue() function.
     * AdvertiseOptions and AdvertiseServiceOptions offer similar functionality.
     */

    ros::SubscribeOptions ops = ros::SubscribeOptions::create<std_msgs::String>(
        "/multi_thread/chatter", 10, chatterCallbackCustomQueue, ros::VoidPtr(),
        &g_queue);

    ros::Subscriber sub = n.subscribe(ops);
    ros::Subscriber sub2 =
        n.subscribe("/multi_thread/chatter", 10, chatterCallbackMainQueue);

    boost::thread chatter_thread(callbackThread);

    ROS_INFO_STREAM("Main thread id=" << boost::this_thread::get_id());

    ros::Rate r(1);

    while (n.ok()) {
      ros::spinOnce();
      r.sleep();
    }

    chatter_thread.join();
}
