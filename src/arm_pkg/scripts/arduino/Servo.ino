#include<Servo.h>
#include<ros.h>
#include<std_msgs/Int16.h>

Servo trigger;

ros::NodeHandle node_data;
std_msgs::Int16 data_recv;

void servo1_cb(std_msgs::Int16& cmd_msg)
{
  data_recv=cmd_msg;
}

ros::Subscriber<std_msgs::Int16> subs("/teleops",servo1_cb);

void setup()
{
  node_data.initNode();
  node_data.subscribe(subs);
  trigger.attach(2);
  trigger.write(0);
}
void loop()
{
  node_data.spinOnce();
  if(data_recv.data==8)
  {
    trigger.write(50);
    data_recv.data=0;
  }
  else if(data_recv.data==2)
  {
    trigger.write(0);
    data_recv.data=0;
  }
  delay(1);
}
