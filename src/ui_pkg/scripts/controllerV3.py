#!/usr/bin/env python3
from functools import partial
import rospy
import cv2
from tkinter import *
from PIL import Image as Img
from PIL import ImageTk
from std_msgs.msg import Int16
from cv_bridge import CvBridge
from sensor_msgs.msg import CompressedImage
from threading import *
import time
import numpy as np
from std_msgs.msg import String
from std_msgs.msg import Int16MultiArray

node_name=str(int(time.time()))
node_name="gui_v3_" + node_name

rospy.init_node(node_name)
end =False
main_window=Tk()
main_window.geometry("1960x1080")
main_window.title("Controller")
gui_mode=True
x_grid=[]
x_grid_pos=[]
y_grid_pos=[]
y_grid=[]
#for bot
temp=[]
saved_angles=[]
names=[]
delays=[]
temp_delays=[]
delay_var=IntVar()
name_var=StringVar()
play_var=StringVar()
itr_var=0
trigger=0
name_var.set("Custom1")
delay_var.set(5)

up_arrow=PhotoImage(file=r"img/up_arrow.png")
up_arrow_pressed=PhotoImage(file=r"img/up_arrow_pressed.png")
right_arrow=PhotoImage(file=r"img/right_rotate.png")
right_arrow_pressed=PhotoImage(file=r"img/right_rotate_pressed.png")
right_arrow1=PhotoImage(file=r"img/right_rotate1.png")
right_arrow_pressed1=PhotoImage(file=r"img/right_rotate_pressed1.png")

strafe_right=PhotoImage(file=r'img/right_arrow.png')
strafe_right_pressed=PhotoImage(file=r'img/right_arrow_pressed.png')
strafe_left=PhotoImage(file=r'img/left_arrow.png')
strafe_left_pressed=PhotoImage(file=r'img/left_arrow_pressed.png')
down_arrow=PhotoImage(file=r'img/down_arrow.png')
down_arrow_pressed=PhotoImage(file=r'img/down_arrow_pressed.png')

left_arrow=PhotoImage(file=r"img/left_rotate.png")
left_arrow_pressed=PhotoImage(file=r"img/left_rotate_pressed.png")
left_arrow1=PhotoImage(file=r"img/left_rotate1.png")
left_arrow_pressed1=PhotoImage(file=r"img/left_rotate_pressed1.png")

agv_img=PhotoImage(file=r"img/agv.png")
gui_icon=PhotoImage(file=r"img/gui.png")
joy_icon=PhotoImage(file=r"img/joy.png")
stop_icon=PhotoImage(file=r"img/stop.png")
stop_pressed=PhotoImage(file=r"img/stop_pressed.png")

pannel_icon=PhotoImage(file=r"img/floor_painting.png")
pannel_icon_pressed=PhotoImage(file=r"img/floor_painting_pressed.png")
wall_icon=PhotoImage(file=r"img/wall_paint.png")
wall_icon_pressed=PhotoImage(file=r"img/wall_paint_pressed.png")
bot_icon=PhotoImage(file=r"img/bot_home_p.png")
bot_icon_pressed=PhotoImage(file=r"img/bot_home.png")

shutdown_icon_on=PhotoImage(file=r"img/power_on.png")
shutdown_icon_off=PhotoImage(file=r"img/power_off.png")
bot_angles=PhotoImage(file=r"img/angles_set.png")
bot_angles_pressed=PhotoImage(file=r"img/angles_set_pressed.png")
robot_img=PhotoImage(file=r"img/robot.png")
reset_icon=PhotoImage(file=r"img/reset.png")
reset_icon_pressed=PhotoImage(file=r"img/reset_pressed.png")
record_icon=PhotoImage(file=r"img/record.png")
record_icon_pressed=PhotoImage(file=r"img/record_pressed.png")
save_icon=PhotoImage(file=r"img/save.png")
save_icon_pressed=PhotoImage(file=r"img/save_pressed.png")
play_icon=PhotoImage(file=r"img/play.png")
play_icon_pressed=PhotoImage(file=r"img/play_pressed.png")

agv_pub=rospy.Publisher("/agv_command",Int16,queue_size=1)
speed_pub=rospy.Publisher("/speed_pub",Int16,queue_size=1)
accel_pub=rospy.Publisher("/raw/acceleration",Int16,queue_size=1)
bot_pub=rospy.Publisher("bot_control_flag",Int16,queue_size=1)
button_disable_pub=rospy.Publisher("/button_disable_flag",Int16,queue_size=1)
bot_home_flag=rospy.Publisher("/bot_home_flag",Int16,queue_size=1)
shutdown_pub=rospy.Publisher("/shutdown_flag",Int16,queue_size=1)
bot_angle_pub=rospy.Publisher("/raw/angle",Int16MultiArray,queue_size=1)

speed_var=0
shutdown_status=False

raw_angles=Int16MultiArray()
raw_angles.data=[0,0,0,0,0,0]

published_data=Int16MultiArray()

key_frame=Frame(main_window,height=1080,width=650,bg="#343434")
key_frame.grid(row=0,column=0,sticky="nsew")
x=0
y=0
def move_grid_x():
    global x,end,speed_var,trigger,names,delays,saved_angles,bot_angle_pub,published_data,names,play_var
    temp=len(x_grid) 
    itr=0
    while True:
        if(trigger==1):
            itr=names.index(play_var.get())
            # print(names.index(play_var.get()))
            for i in range(len(saved_angles[itr])):
                published_data.data=list(saved_angles[itr][i])
                print(published_data.data)
                bot_angle_pub.publish(published_data)
                if(i==len(saved_angles[itr])-1):
                    pass
                else:
                    time.sleep(delays[itr][i+1])
                
            trigger=0
        else:
            speed_temp=0.1-(0.00014*speed_var)
            for i in range(0,temp):
                if(x_grid_pos[i]<=0 and x==-5):
                    x_grid_pos[i]=750
                    graphic_canvas.move(x_grid[i],750,0)
                if(x_grid_pos[i]>750 and x==5):
                    x_grid_pos[i]=0
                    graphic_canvas.move(x_grid[i],-750,0)
                else:
                    graphic_canvas.move(x_grid[i],x,0)
                x_grid_pos[i]+=x
            time.sleep(speed_temp)
def move_grid_y():
    global y,end,speed_var
    temp=len(y_grid) 
    while True:
        speed_temp=0.1-(0.00014*speed_var)
        for i in range(0,temp):
            if(y_grid_pos[i]<=0 and y==-5):
                y_grid_pos[i]=700
                graphic_canvas.move(y_grid[i],0,700)
            if(y_grid_pos[i]>700 and y==5):
                y_grid_pos[i]=0
                graphic_canvas.move(y_grid[i],0,-700)
            else:
                graphic_canvas.move(y_grid[i],0,y)
            y_grid_pos[i]+=y
        time.sleep(speed_temp)
        if(end==True):
            break
def on_press_up(msg):
    global y
    global x,agv_pub
    up_button.config(image=up_arrow_pressed)
    if(gui_mode==True):
        if(speed_var!=0):
            y=5
            x=0
            status_bar.config(text="Moving Forward")
        agv_pub.publish(8)
def on_release_up(msg):
    global y,x,agv_pub
    up_button.config(image=up_arrow)
    if(gui_mode==True):
        x=0
        y=0
        status_bar.config(text="")
        agv_pub.publish(0)
def on_press_down(msg):
    global  y,x,agv_pub
    down_button.config(image=down_arrow_pressed)
    if(gui_mode==True):
        if(speed_var!=0):
            y=-5
            x=0
            status_bar.config(text="Moving Back")
        agv_pub.publish(2)
def on_release_down(msg):
    global y,x,agv_pub
    down_button.config(image=down_arrow)
    if(gui_mode==True):
        x=0
        y=0
        status_bar.config(text="")
        agv_pub.publish(0)
def on_press_rrotate(msg):
    global agv_pub
    right_button.config(image=right_arrow_pressed)
    if(gui_mode==True):
        if(speed_var!=0):
            status_bar.config(text="Rotating Clockwise")
        agv_pub.publish(9)
def on_release_rrotate(msg):
    global agv_pub
    right_button.config(image=right_arrow)
    if(gui_mode==True):
        status_bar.config(text="")
        agv_pub.publish(0)
def on_press_lrotate(msg):
    global agv_pub
    left_button.config(image=left_arrow_pressed)
    if(gui_mode==True):
        if(speed_var!=0):
            status_bar.config(text="Rotating Anticlockwise")
        agv_pub.publish(7)
def on_release_lrotate(msg):
    global agv_pub
    left_button.config(image=left_arrow)
    if(gui_mode==True):
        status_bar.config(text="")
        agv_pub.publish(0)
def on_press_lrotate1(msg):
    global agv_pub
    left_button1.config(image=left_arrow_pressed1)
    if(gui_mode==True):
        if(speed_var!=0):
            status_bar.config(text="Rotating Anticlockwise")
        agv_pub.publish(7)
def on_release_lrotate1(msg):
    global agv_pub
    left_button1.config(image=left_arrow1)
    if(gui_mode==True):
        status_bar.config(text="")
        agv_pub.publish(0)
def on_press_rrotate1(msg):
    global agv_pub
    right_button1.config(image=right_arrow_pressed1)
    if(gui_mode==True):
        if(speed_var!=0):
            status_bar.config(text="Rotating Clockwise")
        agv_pub.publish(9)
def on_release_rrotate1(msg):
    global agv_pub
    right_button1.config(image=right_arrow1)
    if(gui_mode==True):
        status_bar.config(text="")
        agv_pub.publish(0)

def op_strafe_right(msg):
    global x
    global y,agv_pub
    strafe_right_button.config(image=strafe_right_pressed)
    if(gui_mode==True):
        if(speed_var!=0):
            x=-5
            y=0
            status_bar.config(text="Strafing Right")
        agv_pub.publish(6)
def or_strafe_right(msg):
    global x
    global y,agv_pub
    strafe_right_button.config(image=strafe_right)
    if(gui_mode==True):
        x=0
        y=0
        status_bar.config(text="")
        agv_pub.publish(0)
def op_strafe_left(msg):
    global x
    global y,agv_pub
    strafe_left_button.config(image=strafe_left_pressed)
    if(gui_mode==True):
        if(speed_var!=0):
            x=5
            y=0
            status_bar.config(text="Strafing Left")
        agv_pub.publish(4)
def or_strafe_left(msg):
    global x
    global y,agv_pub
    strafe_left_button.config(image=strafe_left)
    if(gui_mode==True):
        x=0
        y=0
        status_bar.config(text="")
        agv_pub.publish(0)
def create_grid():
    for i in range(0,750,50):
        x_grid.append(graphic_canvas.create_line(i,0,i,700,fill="#32CD32",width=3,dash=(5,2)))
        x_grid_pos.append(i)
    for i in range(0,700,50):
        y_grid.append(graphic_canvas.create_line(0,i,750,i,fill="#32CD32",width=3,dash=(5,2)))
        y_grid_pos.append(i)
def speed_slider(msg):
    global speed_var
    speed_var=int(msg)
    speed_pub.publish(int(msg))
def accel_slider(msg):
    accel_pub.publish(int(msg))
def mode_cb():
    global gui_mode,button_disable_pub
    if(gui_mode==True):
        gui_mode=False
        mode_button.config(image=joy_icon)
        button_disable_pub.publish(0)
    else:
        gui_mode=True
        mode_button.config(image=gui_icon)
        button_disable_pub.publish(1)
def graphic_movement_cb(msg):
    global x,y
    if(msg.data==-5 or msg.data==5):
        x=msg.data
    elif(msg.data==-10):
        y=-5
    elif(msg.data==10):
        y=5
    elif(msg.data==0):
        x=0
        y=0
    else:
        print("wrong input")

def op_stop(msg):
    global agv_pub
    agv_pub.publish(0)
    stop_button.config(image=stop_pressed)
def or_stop(msg):
    global agv_pub
    agv_pub.publish(0)
    stop_button.config(image=stop_icon)

def op_pannel(msg):
    global bot_pub
    pannel_paint.config(image=pannel_icon_pressed)
    bot_pub.publish(1)
def or_pannel(msg):
    pannel_paint.config(image=pannel_icon)
def op_wall(msg):
    global bot_pub
    bot_pub.publish(2)
    wall_paint.config(image=wall_icon_pressed)
def or_wall(msg):
    wall_paint.config(image=wall_icon)
def op_bot(msg):
    global bot_home_flag
    bot_home_button.config(image=bot_icon_pressed)
    bot_home_flag.publish(1)
def or_bot(msg):
    bot_home_button.config(image=bot_icon)
def op_shutdown():
    global shutdown_status,shutdown_pub
    if(shutdown_status==False): 
        shutdown_button.config(image=shutdown_icon_off)
        shutdown_status=True
        shutdown_pub.publish(1)
        
    else:
        shutdown_button.config(image=shutdown_icon_on)
        shutdown_status=False 
        shutdown_pub.publish(0)
        status_bar.config(text="")
def shutdown_cb(msg):
    global shutdown_status
    if(shutdown_status==True):
        text="Shutting Down in " + str(msg.data) + 's'
        status_bar.config(text=text)
def status_cb(msg):
    status_bar.config(text=msg.data)
def bot_sliders(index,data):
    global raw_angles,bot_angle_pub
    raw_angles.data[index]=int(data)
    bot_angle_pub.publish(raw_angles)

def op_slider(msg):
    global delay_var
    def op_record(msg):
        global raw_angles,temp,temp_delays
        record_button.config(image=record_icon_pressed)
        temp.append(list(raw_angles.data))
        temp_delays.append(delay_var.get())
    def or_record(msg):
        record_button.config(image=record_icon)

    def op_save(msg):
        global temp_delays,temp,saved_angles,names,delays,name_var
        save_button.config(image=save_icon_pressed)
        saved_angles.append(list(list(temp)))
        delays.append(list(temp_delays))
        names.append(name_var.get())
        temp.clear()
        temp_delays.clear()
    def or_save(msg):
        save_button.config(image=save_icon)

    def op_reset(msg):
        reset_button.config(image=reset_icon_pressed)
    def or_reset(msg):
        reset_button.config(image=reset_icon)
        base_slider.set(0)
        shoulder_slider.set(0)
        elbow_slider.set(0)
        wrist1_slider.set(0)
        wrist2_slider.set(0)
        wrist3_slider.set(0)
    def op_lslide(msg):
        global names,play_var,itr_var
        left_slide.config(image=strafe_left_pressed)
        play_var.set(names[itr_var])
        if(itr_var<=0):
            itr_var=len(names)-1
        else:
            itr_var-=1
        print(itr_var)
        
    def or_lslide(msg):
        left_slide.config(image=strafe_left)
    def op_rslide(msg):
        right_slide.config(image=strafe_right_pressed)
        global names,play_var,itr_var
        play_var.set(names[itr_var])
        if(itr_var>=len(names)-1):
            itr_var=0
        else:
            itr_var+=1   
    def or_rslide(msg):
        right_slide.config(image=strafe_right)
    
    def op_play(msg):
        global trigger
        trigger=1
        play_button.config(image=play_icon_pressed)
    def or_play(msg):
        play_button.config(image=play_icon)
    bot_slider.config(image=bot_angles_pressed)
    bot_sw=Toplevel(main_window,bg="#343434")
    bot_sw.geometry("1200x1080")
    bot_sw.title("Bot Slider Menu")
    Label(bot_sw,text="Base",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=275,y=75)
    base_slider=Scale(bot_sw,from_=-120,to=120,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=partial(bot_sliders,0),troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
    base_slider.place(x=150,y=100)

    Label(bot_sw,text="Shoulder",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=255,y=190)
    shoulder_slider=Scale(bot_sw,from_=-120,to=120,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=partial(bot_sliders,1),troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
    shoulder_slider.place(x=150,y=215)
    
    Label(bot_sw,text="Elbow",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=260,y=305)
    elbow_slider=Scale(bot_sw,from_=-120,to=120,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=partial(bot_sliders,2),troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
    elbow_slider.place(x=150,y=330)

    Label(bot_sw,text="Wrist1",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=260,y=420)
    wrist1_slider=Scale(bot_sw,from_=-120,to=120,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=partial(bot_sliders,3),troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
    wrist1_slider.place(x=150,y=445)

    Label(bot_sw,text="Wrist2",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=260,y=535)
    wrist2_slider=Scale(bot_sw,from_=-120,to=120,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=partial(bot_sliders,4),troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
    wrist2_slider.place(x=150,y=560)

    Label(bot_sw,text="Wrist3",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=260,y=650)
    wrist3_slider=Scale(bot_sw,from_=-120,to=120,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=partial(bot_sliders,5),troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
    wrist3_slider.place(x=150,y=675)
    reset_button=Button(bot_sw,image=reset_icon,bd=0,highlightthickness = 0,borderwidth=0,height=48,width=98)
    reset_button.place(x=250,y=780)
    reset_button.bind('<ButtonPress>',op_reset)
    reset_button.bind('<ButtonRelease>',or_reset)

    robot_canvas=Canvas(bot_sw,height=366,width=170,bg="#343434",bd=0,borderwidth=0,highlightthickness=0)
    robot_canvas.place(x=900,y=100)
    robot_canvas.create_image(80,190,image=robot_img)

    
    Label(bot_sw,text="Delay: ",font=("Aerial",12,"bold"),bg="#343434",fg="#32CD32").place(x=960,y=500)
    entry_delay=Entry(bot_sw,textvariable=delay_var,width=20)
    entry_delay.place(x=910,y=530)
    record_button=Button(bot_sw,image=record_icon,bd=0,highlightthickness = 0,borderwidth=0,height=48,width=98)
    record_button.place(x=940,y=580)
    record_button.bind('<ButtonPress>',op_record)
    record_button.bind('<ButtonRelease>',or_record)

    Label(bot_sw,text="Name: ",font=("Aerial",12,"bold"),bg="#343434",fg="#32CD32").place(x=960,y=670)
    name_entry=Entry(bot_sw,textvariable=name_var,width=20)
    name_entry.place(x=910,y=700)
    save_button=Button(bot_sw,image=save_icon,bd=0,highlightthickness = 0,borderwidth=0,height=48,width=98)
    save_button.place(x=940,y=750)
    save_button.bind('<ButtonPress>',op_save)
    save_button.bind('<ButtonRelease>',or_save)

    left_slide=Button(bot_sw,image=strafe_left,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
    left_slide.place(x=500,y=830)
    left_slide.bind('<ButtonPress>',op_lslide)
    left_slide.bind('<ButtonRelease>',or_lslide)

    play_entry=Entry(bot_sw,textvariable=play_var,width=20)
    play_entry.place(x=600,y=865)
    right_slide=Button(bot_sw,image=strafe_right,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
    right_slide.place(x=770,y=830)
    right_slide.bind('<ButtonPress>',op_rslide)
    right_slide.bind('<ButtonRelease>',or_rslide)
    play_button=Button(bot_sw,image=play_icon,bd=0,highlightthickness = 0,borderwidth=0,height=48,width=98,relief="raised")
    play_button.place(x=630,y=920)
    play_button.bind('<ButtonPress>',op_play)
    play_button.bind('<ButtonRelease>',or_play)

    bot_sw.attributes('-topmost', 'true')
    bot_sw.geometry("+%d+%d" %(730,0))
    bot_sw.mainloop()


def or_slider(msg):
    bot_slider.config(image=bot_angles)

up_button=Button(key_frame,image=up_arrow,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
up_button.place(x=280,y=600)
up_button.bind('<ButtonPress>',on_press_up) #lawn green 
up_button.bind('<ButtonRelease>',on_release_up) #lime green
right_button=Button(key_frame,image=right_arrow,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
right_button.place(x=380,y=625)
right_button.bind('<ButtonPress>',on_press_rrotate)
right_button.bind('<ButtonRelease>',on_release_rrotate)

right_button1=Button(key_frame,image=right_arrow1,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
right_button1.place(x=380,y=825)
right_button1.bind('<ButtonPress>',on_press_rrotate1)
right_button1.bind('<ButtonRelease>',on_release_rrotate1)

strafe_right_button=Button(key_frame,image=strafe_right,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
strafe_right_button.place(x=390,y=725)
strafe_right_button.bind('<ButtonPress>',op_strafe_right)
strafe_right_button.bind('<ButtonRelease>',or_strafe_right)

down_button=Button(key_frame,image=down_arrow,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
down_button.place(x=280,y=850)
down_button.bind('<ButtonPress>',on_press_down)  
down_button.bind('<ButtonRelease>',on_release_down) 

left_button=Button(key_frame,image=left_arrow,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
left_button.place(x=180,y=625)
left_button.bind('<ButtonPress>',on_press_lrotate)
left_button.bind('<ButtonRelease>',on_release_lrotate)

left_button1=Button(key_frame,image=left_arrow1,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
left_button1.place(x=180,y=825)
left_button1.bind('<ButtonPress>',on_press_lrotate1)
left_button1.bind('<ButtonRelease>',on_release_lrotate1)

strafe_left_button=Button(key_frame,image=strafe_left,bd=0,highlightthickness = 0,borderwidth=0,height=94,width=94,relief="raised")
strafe_left_button.place(x=170,y=725)
strafe_left_button.bind('<ButtonPress>',op_strafe_left)
strafe_left_button.bind('<ButtonRelease>',or_strafe_left)

camera_frame=Frame(key_frame,height=500,width=650,bg="#343434") #650x500 (video resolution)
camera_frame.grid(row=0,column=0,sticky="nsew")
camera_label=Label(key_frame,height=500,width=650,bd=0)
camera_label.grid(row=0,column=0)

def image_cb(msg):
    bridge=CvBridge()
    img=bridge.compressed_imgmsg_to_cv2(msg)
    cv2image=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
    image=Img.fromarray(cv2image)
    imgtk=ImageTk.PhotoImage(image=image)
    camera_label.imgtk=imgtk
    camera_label.configure(image=imgtk)
    camera_label.image=imgtk

right_frame=Frame(main_window,height=1080,width=1310,bg="red")
right_frame.grid(row=0,column=1,sticky="nsew")

uppper_frame=Frame(right_frame,height=700,width=1310,bg="green")
uppper_frame.grid(row=0,column=0,sticky="nsew")

graphic_frame=Frame(uppper_frame,height=700,width=750,bg="#343434",bd=0)
graphic_frame.grid(row=0,column=0,sticky="nsew")

graphic_canvas=Canvas(graphic_frame,height=500,width=750,bg="#343434",bd=0,borderwidth=0,highlightthickness=0)
graphic_canvas.grid(row=0,column=0)


create_grid()
graphic_canvas.create_image(380,260,image=agv_img)

slider_frame=Frame(uppper_frame,height=700,width=560,bg="#343434")
slider_frame.grid(row=0,column=1,sticky="nsew")

Label(slider_frame,text="Speed",font=("Aerial",20,"bold"),bg="#343434",fg="#32CD32").place(x=200,y=268)
speed=Scale(slider_frame,from_=0,to=500,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=speed_slider,troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
speed.place(x=100,y=300)

Label(slider_frame,text="Acceleration",font=("Aerial",20,"bold"),bg="#343434",fg="#32CD32").place(x=160,y=418)
accel=Scale(slider_frame,from_=0,to=500,orient=HORIZONTAL,bg="#343434",bd=0,highlightbackground="#343434",length=300,fg="#32CD32",command=accel_slider,troughcolor='#32CD32',activebackground='#7CFC00',width=30,cursor="dot",font=("Aerial",15,"bold"))
accel.place(x=100,y=450)
status_bar=Label(slider_frame,text="",font=("Aerial",20,"bold"),bg="#343434",fg="#32CD32")
status_bar.place(relx=.45, rely=.9,anchor="center")

shutdown_button=Button(slider_frame,image=shutdown_icon_on,bd=0,highlightthickness = 0,borderwidth=0,height=118,width=118,command=op_shutdown)
shutdown_button.place(x=180,y=80)

predefined_frame=Frame(right_frame,height=380,width=1310,bg="#343434")
predefined_frame.grid(row=1,column=0,sticky="nsew")
mode_button=Button(predefined_frame,image=gui_icon,bd=0,highlightthickness = 0,borderwidth=0,height=220,width=220,command=mode_cb)
mode_button.place(x=100,y=50)
stop_button=Button(predefined_frame,image=stop_icon,bd=0,highlightthickness = 0,borderwidth=0,height=220,width=220)
stop_button.place(x=850,y=50)
stop_button.bind('<ButtonPress>',op_stop)
stop_button.bind('<ButtonRelease>',or_stop)
bot_home_button=Button(predefined_frame,image=bot_icon,bd=0,highlightthickness = 0,borderwidth=0,height=220,width=220)
bot_home_button.place(x=475,y=50)
bot_home_button.bind('<ButtonPress>',op_bot)
bot_home_button.bind('<ButtonRelease>',or_bot)

pannel_paint=Button(graphic_frame,image=pannel_icon,bd=0,highlightthickness = 0,borderwidth=0,height=118,width=118)
pannel_paint.place(x=50,y=530)
pannel_paint.bind('<ButtonPress>',op_pannel)
pannel_paint.bind('<ButtonRelease>',or_pannel)
Label(graphic_frame,text="Panel Painting",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=20,y=650)
wall_paint=Button(graphic_frame,image=wall_icon,bd=0,highlightthickness = 0,borderwidth=0,height=118,width=118)
wall_paint.place(x=600,y=530)
wall_paint.bind('<ButtonPress>',op_wall)
wall_paint.bind('<ButtonRelease>',or_wall)
Label(graphic_frame,text="Wall Painting",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=580,y=650)
bot_slider=Button(graphic_frame,image=bot_angles,bd=0,highlightthickness = 0,borderwidth=0,height=118,width=118)
bot_slider.place(x=330,y=530)
bot_slider.bind('<ButtonPress>',op_slider)
bot_slider.bind('<ButtonRelease>',or_slider)
Label(graphic_frame,text="Set Angles",font=("Aerial",15,"bold"),bg="#343434",fg="#32CD32").place(x=330,y=650)


if(gui_mode==True):
    button_disable_pub.publish(1)

moving_thread_x=Thread(target=move_grid_x)
moving_thread_x.start()
moving_thread_y=Thread(target=move_grid_y)
moving_thread_y.start()

rospy.Subscriber("/agv_movement",Int16,graphic_movement_cb) # 5,-5 -->x & 10,10-->5,-5 in y
rospy.Subscriber("/shutdown_timer",Int16,shutdown_cb)
rospy.Subscriber("/test_img",CompressedImage,image_cb)
rospy.Subscriber("/painting_status",String,status_cb)

main_window.mainloop()

end=True
try:
    main_window.destroy()
except:
    pass













__author__ = "Ayush Kumar"
__license__ = "2022"
__version__ = "3.0.0"
__maintainer__ = "Ayush Kumar"
__email__ = "akp8340135@gmail.com"
__status__ = "Prototype"
