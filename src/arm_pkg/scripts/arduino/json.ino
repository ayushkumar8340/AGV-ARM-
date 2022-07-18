#include <ArduinoJson.h>
#include<ros.h>
#include<SPI.h>
#include<mcp_can.h>
#include<std_msgs/String.h>

ros::NodeHandle node_data;


DynamicJsonDocument doc(100);

const int SPI_CS_PIN = 53;
MCP_CAN CAN(SPI_CS_PIN);

unsigned long pos = 0;
unsigned char len;

long prev_time=0;
long curr_time=0;

unsigned char buf[8];

int angle_data[8] = {0, 0, 0, 0, 0, 0};

void run_motor(unsigned int id, int angle, int reduction_ratio)
{
  pos = angle;
  pos = pos * reduction_ratio * 100;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos >> 8;
  buf[6] = pos >> 16;
  buf[7] = pos >> 24;
  CAN.sendMsgBuf(id, 0, 8, buf);
}

void data_cb(const std_msgs::String& frame)
{
  deserializeJson(doc,frame.data);
  angle_data[0]=doc["base"];
  angle_data[1]=doc["shoulder"];
  angle_data[2]=doc["elbow"];
  angle_data[3]=doc["wrist1"];
  angle_data[4]=doc["wrist2"];
  angle_data[5]=doc["wrist3"];
  run_motor(0x141,angle_data[0],40);
  run_motor(0x142,angle_data[1],40);
  run_motor(0x143,angle_data[2],40);
  run_motor(0x144,angle_data[3],35);
  run_motor(0x145,angle_data[4],35);
  run_motor(0x146,angle_data[5],35);
}

ros::Subscriber<std_msgs::String> sub("/angles", &data_cb );


void setup()
{
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

  buf[0] = 0x34;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = 0xEE;
  buf[5] = 0x02;
  buf[6] = 0x00;
  buf[7] = 0x00;
  CAN.sendMsgBuf(0x142, 0, 8, buf);
  CAN.sendMsgBuf(0x143, 0, 8, buf);
  CAN.sendMsgBuf(0x144, 0, 8, buf);
  CAN.sendMsgBuf(0x145, 0, 8, buf);
  CAN.sendMsgBuf(0x146, 0, 8, buf);
  CAN.sendMsgBuf(0x141, 0, 8, buf);
  
  node_data.initNode();
  node_data.subscribe(sub);
}
void loop()
{ 
  buf[0] = 0x34;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = 0xEE;
  buf[5] = 0x02;
  buf[6] = 0x00;
  buf[7] = 0x00;
  CAN.sendMsgBuf(0x142, 0, 8, buf);
  CAN.sendMsgBuf(0x143, 0, 8, buf);
  CAN.sendMsgBuf(0x144, 0, 8, buf);
  CAN.sendMsgBuf(0x145, 0, 8, buf);
  CAN.sendMsgBuf(0x146, 0, 8, buf);
  CAN.sendMsgBuf(0x141, 0, 8, buf);
  
  node_data.spinOnce();
  delay(1);
}
