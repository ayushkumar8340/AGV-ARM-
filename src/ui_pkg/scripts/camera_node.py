#!/usr/bin/env python3
import rospy
import cv2
import numpy as np
from cv_bridge import CvBridge
from sensor_msgs.msg import CompressedImage
def control():
    rospy.init_node("test")
    bridge=CvBridge()
    pub=rospy.Publisher("/test_img",CompressedImage,queue_size=1)
    vid=cv2.VideoCapture(0)
    rate=rospy.Rate(100)
    while not rospy.is_shutdown():
        _,frame=vid.read()
        resized_frame=cv2.resize(frame,(500,650),interpolation=cv2.INTER_LINEAR)
        resized_frame=cv2.rotate(resized_frame,cv2.ROTATE_90_COUNTERCLOCKWISE)
        # _,img=cv2.imencode('.jpeg',resized_frame)
        # img=np.array(img).tostring()
        # print(img)
        # img=bridge.cv2_to_imgmsg(resized_frame,"bgr8")
        img=bridge.cv2_to_compressed_imgmsg(resized_frame)
        # print(img)
        pub.publish(img)
        rate.sleep()

if __name__=="__main__":
    control()