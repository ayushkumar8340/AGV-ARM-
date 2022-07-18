#include <SPI.h>
#include "mcp_can.h"

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

const int SPI_CS_PIN = 53;

MCP_CAN CAN(SPI_CS_PIN);

unsigned char flagRecv = 0;
unsigned char buf[8];

unsigned long pos=0;
volatile int trig=0;
int w_1=0;
int w_2=0;
int w_3=0;
int w_4=0;

int incremental_angle=100;
int alpha=1;

void setup()
{
    Serial.begin(1000000);
//    attachInterrupt(digitalPinToInterrupt(18),a_ISR,RISING);
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
//    attachInterrupt(0, MCP2515_ISR, FALLING);

    buf[0] = 0x34;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00; 
    buf[4] = 0x10;  // for accleration of small motor/big motor
    buf[5] = 0x27;
    buf[6] = 0x00; 
    buf[7] = 0x00;
    CAN.sendMsgBuf(0x142, 0, 8, buf);
    CAN.sendMsgBuf(0x143, 0, 8, buf);
    CAN.sendMsgBuf(0x141, 0, 8, buf);
    CAN.sendMsgBuf(0x144, 0, 8, buf);
}
void MCP2515_ISR()
{
    flagRecv = 1;
}
//void a_ISR()
//{
//  trig=1;
//}
void loop()
{
  int data=Serial.read()-'0';
 ////////////////////////////////////
 ////FORWARD BLOCK////
// if (trig==1)
// {
//  buf[0] = 0x81;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = 0x00;
//  buf[5] = 0x00;    
//  buf[6] = 0x00; 
//  buf[7] = 0x00;
//  CAN.sendMsgBuf(0x141, 0, 8, buf);
//  CAN.sendMsgBuf(0x142, 0, 8, buf);
//  CAN.sendMsgBuf(0x143, 0, 8, buf);
//  CAN.sendMsgBuf(0x144, 0, 8, buf);
//  trig=0;
//  Serial.println("Entered");
// }
 if (data==8)
 {
  w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141, 0, 8, buf);
  
  w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143, 0, 8, buf);
  
  w_4=w_4+(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  w_2=w_2+(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
 }
 //////////////////////////////////

 //////////////////////////////////
 ////Backward Block////
 if(data==2)
 {
  Serial.println("here");
  w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  
  w_1=w_1+(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  w_3=w_3+(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
 }
  //////////////////////////////////

  /////////////////////////////////
  ////right strafe////
  if(data==6){
  w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  
  w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  w_3=w_3+(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  
  w_2=w_2+(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  } 
 //////////////////////////////////

 /////////////////////////////////
 ////left strafe////
 if (data==4)
 {
  w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
//  
  w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);

  
  w_4=w_4+(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  w_1=w_1+(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
 }
  ////////////////////////////////////

  ///////////////////////////////////
  ////right rotate /////
  if(data==5)
  {
  
  w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  }
  /////////////////////////////////
  ////left rotate////
  if(data==0)
  {
    
  w_1=w_1+(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  
  w_2=w_2+(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  
  w_3=w_3+(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  
  w_4=w_4+(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  }
  ///////////////////////////////////
  ////stop////
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = 0x00;
//  buf[5] = 0x00;    
//  buf[6] = 0x00; 
//  buf[7] = 0x00;
//  CAN.sendMsgBuf(0x141,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  CAN.sendMsgBuf(0x143,0,8,buf);
//  CAN.sendMsgBuf(0x144,0,8,buf);
  
  ///////////////////////////////////

  ///////////////////////////////////
  ////top right stafe////
  if(data==9)
  {
  w_2=w_2+(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);
  
  w_1=w_1-(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);
  }
  /////////////////////////////////
  
  /////////////////////////////////
  /////bottom right stafe////
  if(data==1)
  {
  w_2=w_2-(incremental_angle*alpha);
  pos=w_2;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x142,0,8,buf);

  w_1=w_1+(incremental_angle*alpha);
  pos=w_1;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x141,0,8,buf);

  }
  //////////////////////////////////
  
  //////////////////////////////////
  ////top left stafe////
  if(data==7)
  {
    
  w_3=w_3-(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);

  w_4=w_4+(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);
  
  }
  //////////////////////////////////
  
  //////////////////////////////////
  ////bottom left strafe////
  if(data==3)
  {
  w_4=w_4-(incremental_angle*alpha);
  pos=w_4;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x144,0,8,buf);

  w_3=w_3+(incremental_angle*alpha);
  pos=w_3;
  pos=pos*3500;
  buf[0] = 0xA3;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x00;
  buf[4] = pos;
  buf[5] = pos>>8;    
  buf[6] = pos>>16; 
  buf[7] = pos>>24;
  CAN.sendMsgBuf(0x143,0,8,buf);
  }
 //////////////////////////////////////

 //////////////////////////////////////
 ////Torque zero////
//  buf[0] = 0xA1;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = 0x00;
//  buf[5] = 0x00;    
//  buf[6] = 0x00; 
//  buf[7] = 0x00;
//  CAN.sendMsgBuf(0x141,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  CAN.sendMsgBuf(0x143,0,8,buf);
//  CAN.sendMsgBuf(0x144,0,8,buf);
 //////////////////////////////////////
   
}

    
