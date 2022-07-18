#include<ros.h>
#include<std_msgs/Int16.h>
#include <SPI.h>
#include <mcp_can.h>

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

const int SPI_CS_PIN = 53;

unsigned char flagRecv = 0;
unsigned char len = 0;

long enc_pos[4];
long ang[4];

MCP_CAN CAN(SPI_CS_PIN);

unsigned long pos=0;
volatile int trig=0;
int w_1=0;
int w_2=0;
int w_3=0;
int w_4=0;

long curr_time;
long time_elapsed;
int incremental_angle=3000;
int alpha=1;

unsigned char buf[8];

std_msgs::Int16 data_recv;

ros::NodeHandle node_data;
void data_cb(std_msgs::Int16& cmd_data)
{
  data_recv=cmd_data;
}

ros::Subscriber<std_msgs::Int16> subs("/teleops",data_cb);

void setup()
{
  Serial.begin(57600);
  attachInterrupt(0, MCP2515_ISR, FALLING);
  START_INIT:
  
  if(CAN_OK == CAN.begin(CAN_1000KBPS))                   
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
    buf[4] = 0x4C;  //10// for accleration of small motor/big motor
    buf[5] = 0x1D;  //27
    buf[6] = 0x00; 
    buf[7] = 0x00;
    CAN.sendMsgBuf(0x142, 0, 8, buf);
    CAN.sendMsgBuf(0x143, 0, 8, buf);
    CAN.sendMsgBuf(0x141, 0, 8, buf);
    CAN.sendMsgBuf(0x144, 0, 8, buf);

  node_data.initNode();
  node_data.subscribe(subs);
}
void MCP2515_ISR()
{
    flagRecv = 1;
    
}
void loop()
{ 
  node_data.spinOnce();
  if(data_recv.data==8)
  {
  w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141, 0, 8, buf);
  
  w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143, 0, 8, buf);
  
  w_4=w_4+(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  w_2=w_2+(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  //delay(200);
  data_recv.data=0;
  }
  if(data_recv.data==88)
  {
  buf[0] = 0x81;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = 0x00;
  buf[5] = 0x00;    
  buf[6] = 0x00; 
  buf[7] = 0x00;
  CAN.sendMsgBuf(0x141, 0, 8, buf);
  CAN.sendMsgBuf(0x142, 0, 8, buf);
  CAN.sendMsgBuf(0x143, 0, 8, buf);
  CAN.sendMsgBuf(0x144, 0, 8, buf);

  curr_time=millis();
while(1)
{
  time_elapsed=millis();
  buf[0] = 0x92;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = 0x00;
  buf[5] = 0x00;    
  buf[6] = 0x00; 
  buf[7] = 0x00;
  CAN.sendMsgBuf(0x141, 0, 8, buf);
  
  if(flagRecv) 
    {                                   
        flagRecv = 0;                   
        while (CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            CAN.readMsgBuf(&len, buf);
            if(0x141==CAN.getCanId())
            {
               if(buf[4]==0)
               {
                   enc_pos[0] = ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]);
                   ang[0] = ((enc_pos[0]*0.01/35));
               }
              else 
               {
                    enc_pos[0] =-(16777215 - ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]));
                    ang[0] = ((enc_pos[0]*0.01/35));
               }
      
            }
        }
    }
 //////////////////////////////////////////////////  
  buf[0] = 0x92;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = 0x00;
  buf[5] = 0x00;    
  buf[6] = 0x00; 
  buf[7] = 0x00;
  CAN.sendMsgBuf(0x142, 0, 8, buf); 
    if(flagRecv) 
    {                                   
        flagRecv = 0;                   
        while (CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            CAN.readMsgBuf(&len, buf);
            if(0x142==CAN.getCanId())
            {
               if(buf[4]==0)
               {
                   enc_pos[1] = ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]);
                   ang[1] = ((enc_pos[1]*0.01/35));
               }
              else 
               {
                    enc_pos[1] =-(16777215 - ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]));
                    ang[1] = ((enc_pos[1]*0.01/35));
               }
      
            }
        }
    }
 ////////////////////////////////////////////////
 delay(10);
  buf[0] = 0x92;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = 0x00;
  buf[5] = 0x00;    
  buf[6] = 0x00; 
  buf[7] = 0x00;
  CAN.sendMsgBuf(0x143, 0, 8, buf);
    if(flagRecv) 
    {                                   
        flagRecv = 0;                   
        while (CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            CAN.readMsgBuf(&len, buf);
            if(0x143==CAN.getCanId())
            {
               if(buf[4]==0)
               {
                   enc_pos[2] = ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]);
                   ang[2] = ((enc_pos[2]*0.01/35));
               }
              else 
               {
                    enc_pos[2] =-(16777215 - ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]));
                    ang[2] = ((enc_pos[2]*0.01/35));
               }
      
            }
        }
    }
 /////////////////////////////////////////////////
 delay(10);
  buf[0] = 0x92;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = 0x00;
  buf[5] = 0x00;    
  buf[6] = 0x00; 
  buf[7] = 0x00;
  CAN.sendMsgBuf(0x144, 0, 8, buf);
    if(flagRecv) 
    {                                   
        flagRecv = 0;                   
        while (CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            CAN.readMsgBuf(&len, buf);
            if(0x144==CAN.getCanId())
            {
               if(buf[4]==0)
               {
                   enc_pos[3] = ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]);
                   ang[3] = ((enc_pos[3]*0.01/35));
               }
              else 
               {
                    enc_pos[3] =-(16777215 - ((((unsigned long)buf[3]) << 16) | (((unsigned long)buf[2]) << 8) | buf[1]));
                    ang[3] = ((enc_pos[3]*0.01/35));
               }
      
            }
        }
    }
    w_1=ang[0];
    w_2=ang[1];
    w_3=ang[2];
    w_4=ang[3];
 if(time_elapsed-curr_time>=2000)
 {
   break;
 }
 }
 data_recv.data=0;
  }
  if (data_recv.data==2)
  {
    w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  
  w_1=w_1+(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  w_3=w_3+(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  data_recv.data=0;
  }
  
  if(data_recv.data==4)
  {
    w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  
  w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);

  
  w_4=w_4+(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  w_1=w_1+(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  data_recv.data=0;
  }
  if(data_recv.data==6)
  {
    w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  
  w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  w_3=w_3+(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  
  w_2=w_2+(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);

  data_recv.data=0;
  }

  if(data_recv.data==5)
  {
  w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  data_recv.data=0;
  }
  if(data_recv.data==9)
  {
  w_2=w_2+(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  
  w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  data_recv.data=0;
  }
  if(data_recv.data==1)
  {
    w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);

  w_1=w_1+(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  data_recv.data=0;
  }
  if(data_recv.data==7)
  {
  w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);

  w_4=w_4+(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  data_recv.data=0;
  }

  if(data_recv.data==3)
  {
    w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);

  w_3=w_3+(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA4;
  buf[1] = 0x00;
  buf[2] = 0xC4;
  buf[3] = 0x09;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  data_recv.data=0;
  }
  
  delay(1);
}
