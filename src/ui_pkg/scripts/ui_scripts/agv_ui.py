import rospy
from std_msgs.msg import Int16
from std_msgs.msg import Int16MultiArray

speed_var=[0,0,0,0]
speed_var_pub=Int16MultiArray()
speed_var_pub.data=speed_var
speed=Int16()
but_dis=Int16()
but_dis.data=1
def speed_cb(msg):
    global speed
    speed=msg
def disable_cb(msg):
    global but_dis
    but_dis=msg
def command_cb(msg):
    global speed_pub
    global speed_var
    global speed
    if(msg.data==2):
        speed_var=[speed.data,-speed.data,speed.data,-speed.data]
    elif(msg.data==8):
        speed_var=[-speed.data,speed.data,-speed.data,speed.data]
    elif(msg.data==6):
        speed_var=[-speed.data,speed.data,speed.data,-speed.data]
    elif(msg.data==4):
        speed_var=[speed.data,-speed.data,-speed.data,speed.data]
    elif(msg.data==9):
        speed_var=[-speed.data,-speed.data,-speed.data,-speed.data]
    elif(msg.data==7):
        speed_var=[speed.data,speed.data,speed.data,speed.data]
    elif(msg.data==0):
        speed_var=[0,0,0,0]
    speed_var_pub.data=speed_var
def control():
    rospy.Subscriber("/agv_command",Int16,command_cb)
    rospy.Subscriber("/speed_pub",Int16,speed_cb)
    rospy.Subscriber("/button_disable_flag",Int16,disable_cb)
    rate=rospy.Rate(10)
    while not rospy.is_shutdown():
        if(but_dis.data==1):
            speed_pub.publish(speed_var_pub)
        rate.sleep()
if __name__=="__main__":
    rospy.init_node("agv_node",anonymous=True)
    speed_pub=rospy.Publisher("/raw/wheels_velocity",Int16MultiArray,queue_size=1)
    control()
