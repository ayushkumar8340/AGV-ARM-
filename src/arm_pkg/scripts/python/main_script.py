#!/usr/bin/env python3
import rospy
from std_msgs.msg import String
from std_msgs.msg  import Int16MultiArray
import json

def angle_cb(msg):
    global data_frame
    data_frame["base"]=msg.data[0]
    data_frame["shoulder"]=msg.data[1]
    data_frame["elbow"]=msg.data[2]
    data_frame["wrist1"]=msg.data[3]
    data_frame["wrist2"]=msg.data[4]
    data_frame["wrist3"]=msg.data[5]
    
def control():
    global json_string
    global data_frame
    rate=rospy.Rate(10)
    while not rospy.is_shutdown():
        json_string.data=json.dumps(data_frame)
        json_pub.publish(json_string)
        rate.sleep()

if __name__=="__main__":
    rospy.init_node("json_node",anonymous=True)
    data_frame={
    "base": 0,
    "shoulder": 0,
    "elbow": 0,
    "wrist1": 0,
    "wrist2": 0,
    "wrist3": 0
    } 
    json_string=String()  
    rospy.Subscriber("/raw/angle",Int16MultiArray,angle_cb)
    json_pub=rospy.Publisher("/angles",String,queue_size=1)
    control()