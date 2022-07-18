#include<ros.h>
#include<SPI.h>
#include<mcp_can.h>
#include<std_msgs/Int32MultiArray.h>
#include<std_msgs/Int16MultiArray.h>
#include<std_msgs/Int16.h>
#include<std_msgs/Int32.h>

const int SPI_CS_PIN = 53;
MCP_CAN CAN(SPI_CS_PIN);

unsigned char len;
unsigned char data_frame[8];

int32_t vel[4]={0,0,0,0};
int32_t acceleration=500;
int16_t reduction=35;
int16_t ids[4]={0x141,0x142,0x143,0x144};
ros::NodeHandle node_data;

int32_t temp_data=0;

void set_acc(int32_t _acc)
{
  _acc=_acc*reduction;
  data_frame[0]=0x34;
  data_frame[2]=0x00;
  data_frame[3]=0x00;
  data_frame[4]=_acc;
  data_frame[5]=_acc>>8;
  data_frame[6]=_acc>>16;
  data_frame[7]=_acc>>24;
  CAN.sendMsgBuf(ids[0],0,8,data_frame);
  CAN.sendMsgBuf(ids[1],0,8,data_frame);
  CAN.sendMsgBuf(ids[2],0,8,data_frame);
  CAN.sendMsgBuf(ids[3],0,8,data_frame);
}

void wheels_cb(const std_msgs::Int16MultiArray& val)
{
    vel[0]=val.data[0];
    vel[1]=val.data[1];
    vel[2]=val.data[2];
    vel[3]=val.data[3];
    
    run_motor(ids[0],vel[0]);
    run_motor(ids[1],vel[1]);
    run_motor(ids[2],vel[2]);
    run_motor(ids[3],vel[3]);
}

void reduction_cb(const std_msgs::Int16& val)
{
  reduction=val.data;

}

void id_cb(const std_msgs::Int16MultiArray& val)
{
  ids[0]=val.data[0];
  ids[1]=val.data[1];
  ids[2]=val.data[2];
  ids[3]=val.data[3];
}
void acc_cb(const std_msgs::Int16& val)
{
  acceleration=val.data;
  set_acc(acceleration);
}

ros::Subscriber<std_msgs::Int16MultiArray> id("/raw/ids",id_cb);
ros::Subscriber<std_msgs::Int16> accel("/raw/acceleration",acc_cb);
ros::Subscriber<std_msgs::Int16> reduction_ratio("/raw/reductioRatio",reduction_cb);
ros::Subscriber<std_msgs::Int16MultiArray> wheels("/raw/wheels_velocity",wheels_cb);

void run_motor(unsigned int id,int32_t velocity)
{
  temp_data=velocity*reduction/0.01;
  data_frame[0]=0xA2;
  data_frame[2]=0x00;
  data_frame[3]=0x00;
  data_frame[4]=temp_data;
  data_frame[5]=temp_data>>8;
  data_frame[6]=temp_data>>16;
  data_frame[7]=temp_data>>24;
  CAN.sendMsgBuf(id,0,8,data_frame);
}

void setup()
{
//Serial.begin(57600);
START_INIT:

  if (CAN_OK == CAN.begin(CAN_1000KBPS))
  {
    Serial.println("CAN BUS Shield init ok!");
  }
  else
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
    goto START_INIT;
  }
node_data.initNode();
//node_data.getHardware()->setBaud(115200);
node_data.subscribe(id);
node_data.subscribe(accel);
node_data.subscribe(reduction_ratio);
node_data.subscribe(wheels);
set_acc(acceleration);
}

void loop()
{
  node_data.spinOnce();
}
