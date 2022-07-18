#!/usr/bin/env python3
from enum import Flag
import rospy
import time
from std_msgs.msg import Int16
flag=Int16()
control_data=Int16()
manual_flag=Int16()
home_flag=Int16()
home_flag.data=0
curr_time=0.0
prev_time=0.0
trig=rospy.Publisher("/bot_trigger",Int16,queue_size=1)
servo=rospy.Publisher("/trigger_servo",Int16,queue_size=1)
def control_cb(msg):
    global control_data
    control_data=msg
def manual_cb(msg):
    global manual_flag
    manual_flag=msg 
def home_cb(msg):
    global home_flag
    home_flag=msg
def check_data():
    global trig
    global manual_flag
    global control_data
    global servo
    if(home_flag.data==1):
        control_data.data=0
        manual_flag.data=0
        servo.publish(0)
        while True:
            trig.publish(88)
            if(home_flag.data==0):
                break   
        return -1  

def self_delay(time_in_secs):
    global curr_time
    global prev_time
    while True:
        curr_time=time.time()
        check_data()
        if(curr_time-prev_time>=time_in_secs):
            prev_time=0
            curr_time=0
            break

def control():
    global flag
    global control_data
    global manual_flag
    global trig
    global servo
    rospy.Subscriber("/bot_control_flag",Int16,control_cb)
    rospy.Subscriber("/manual_control_flag",Int16,manual_cb)
    rospy.Subscriber("/bot_home_flag",Int16,home_cb)

    while not rospy.is_shutdown():
        if(control_data.data==1 or manual_flag.data==1 and home_flag==0):
            
            flag=1
            trig.publish(flag)
            if(check_data==-1):
                continue 
            servo.publish(1)
            flag=2
            trig.publish(flag)
            if(check_data==-1):
                continue
            self_delay(3)                            #  time.sleep(3)

            servo.publish(0)
            self_delay(2)                           # time.sleep(2)


            flag=3
            trig.publish(flag)
            if(check_data==-1):
                continue
            self_delay(4) # time.sleep(4)
            servo.publish(1)


            flag=4
            trig.publish(flag)
            if(check_data==-1):
                continue
            self_delay(4) # time.sleep(4)
            servo.publish(0)
            self_delay(1) # time.sleep(1)

            flag=88
            trig.publish(flag)
            if(check_data==-1):
                continue
            self_delay(5) # time.sleep(5)
            control_data.data=0
            manual_flag.data=0
            

if __name__=="__main__":
    rospy.init_node("paint",anonymous=True)
    control()