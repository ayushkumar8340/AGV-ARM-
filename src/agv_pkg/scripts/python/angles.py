#!/usr/bin/env python3
import rospy
from std_msgs.msg import Int16MultiArray
from std_msgs.msg import Int16
import time

def flag_cb(msg):
    global flag
    flag=msg
def control():
    global flag,agv_pub
    angles=Int16MultiArray()
    angles.data=[0,0,0,0,0,0]
    rate=rospy.Rate(10)
    while not rospy.is_shutdown():
        if(flag.data==1):
            angles.data=[75,-30,10,10,10,10]   #change angles here
            angle_pub.publish(angles)
            time.sleep(5)
            agv_pub.publish(8)
            time.sleep(4)
            agv_pub.publish(0)

            angles.data=[-75,-30,10,10,10,10]   #change angles here
            angle_pub.publish(angles)
            time.sleep(5)
            agv_pub.publish(2)
            time.sleep(4)
            agv_pub.publish(0)
            time.sleep(5)
            angles.data=[0,0,0,0,0,0]   #change angles here
            angle_pub.publish(angles)
            time.sleep(7)
            flag.data=0

        rate.sleep()

if __name__=="__main__":
    rospy.init_node("angles_node",anonymous=True)
    flag=Int16()
    angle_pub=rospy.Publisher("/raw/angle",Int16MultiArray,queue_size=1)
    agv_pub=rospy.Publisher("/agv_command",Int16,queue_size=1)
    rospy.Subscriber("/bot_control_flag",Int16,flag_cb)
    control()