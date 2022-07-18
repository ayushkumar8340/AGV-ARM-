#!/usr/bin/env python3
import rospy
from keyboard.msg import Key
from std_msgs.msg import Int16

key_press_down=Key()
key_press_up=Key()
value=Int16()
count_1=0
count_2=0
count_3=0
count_4=0
count_5=0
count_6=0
count_7=0
count_8=0
count_9=0
count_10=0
count_11=0
def key_cb_down(data):
    global key_press_down
    key_press_down=data
def key_cb_up(data):
    global key_press_up
    key_press_up=data

def control():
    global key_press_up
    global key_press_down
    global count_1
    global count_2
    global count_3
    global count_4
    global count_5
    global count_6
    global count_7
    global count_8
    global count_9
    global count_10
    global count_11
    rospy.init_node("key_convertor",anonymous=True)
    data=rospy.Publisher("/teleops",Int16,queue_size=10)
    rospy.Subscriber("/keyboard/keydown",Key,key_cb_down)
    rospy.Subscriber("/keyboard/keyup",Key,key_cb_up)
    rate=rospy.Rate(60)
    while not rospy.is_shutdown():
        # print(key_press_down)
        # print(key_press_up) 

        if (key_press_down.code==273 and count_1 == 0):
            value = 8
            data.publish(value)
            count_1=count_1+1
        if (key_press_up.code==273):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_1=0
            ########################################
        if (key_press_down.code==274 and count_2 == 0):
            value = 2
            data.publish(value)
            count_2=count_2+1
        if (key_press_up.code==274):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_2=0
        ############################################
        if (key_press_down.code==276 and count_3 == 0):
            value = 4
            data.publish(value)
            count_3=count_3+1
        if (key_press_up.code==276):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_3=0
        ############################################
        if (key_press_down.code==275 and count_4== 0):
            value = 6
            data.publish(value)
            count_4=count_4+1
        if (key_press_up.code==275):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_4=0
#####################################################################
        if (key_press_down.code==97 and count_5 == 0):
            value = 5
            data.publish(value)
            count_5=count_5+1
        if (key_press_up.code==97):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_5=0
            ########################################
        if (key_press_down.code==100 and count_6 == 0):
            value = 0
            data.publish(value)
            count_6=count_6+1
        if (key_press_up.code==100):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_6=0
        ############################################
        if (key_press_down.code==113 and count_7 == 0):
            value = 7
            data.publish(value)
            count_7=count_7+1
        if (key_press_up.code==113):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_7=0
        ############################################
        if (key_press_down.code==101 and count_8== 0):
            value = 9
            data.publish(value)
            count_8=count_8+1
        if (key_press_up.code==101):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_8=0
            ########################################
        if (key_press_down.code==122 and count_9 == 0):
            value = 1
            data.publish(value)
            count_9=count_9+1
        if (key_press_up.code==122):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_9=0
        ############################################
        if (key_press_down.code==99 and count_10 == 0):
            value = 3
            data.publish(value)
            count_10=count_10+1
        if (key_press_up.code==99):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_10=0
        ############################################
        if (key_press_down.code==32 and count_11== 0):
            value = 10
            data.publish(value)
            count_11=count_11+1
        if (key_press_up.code==101):
            value = 88
            data.publish(88)
            key_press_down.code=0
            key_press_up.code=0
            count_11=0
    rospy.spin()
if __name__=="__main__":
    control()
