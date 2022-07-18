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
int negative_val=-360;
int positive_val=360;
int alpha=1;

void setup()
{
    
    Serial.begin(1000000);
    START_INIT:

    if(CAN_OK == CAN.begin(CAN_1000KBPS))                   // init can bus : baudrate = 1000k
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
    attachInterrupt(0, MCP2515_ISR, FALLING);

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
void loop()
{
 ////////////////////////////////////
 ////FORWARD BLOCK////
 
//  pos=alpha*negative_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x141, 0, 8, buf);
//  CAN.sendMsgBuf(0x143, 0, 8, buf);
//  pos =alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x144,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  
 //////////////////////////////////

 //////////////////////////////////
 ////Backward Block////
// pos=alpha*negative_val;
// pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x144,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  pos=alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x141,0,8,buf);
//  CAN.sendMsgBuf(0x143,0,8,buf);
  //////////////////////////////////

  /////////////////////////////////
  ////right strafe////
  
//  pos=alpha*negative_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x141,0,8,buf);
//  CAN.sendMsgBuf(0x144,0,8,buf);
//  pos=alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x143,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
  
 //////////////////////////////////

 /////////////////////////////////
 ////left strafe////

// pos=alpha*negative_val;
// pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x143,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  pos=alpha*positive_val;
//  pos=pos*3600;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x144,0,8,buf);
//  CAN.sendMsgBuf(0x141,0,8,buf);

  ////////////////////////////////////

  ///////////////////////////////////
  ////right rotate /////
//  pos=alpha*negative_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x141,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  CAN.sendMsgBuf(0x143,0,8,buf);
//  CAN.sendMsgBuf(0x144,0,8,buf);
  /////////////////////////////////
  ////left rotate////
  
//  pos=alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x141,0,8,buf);
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  CAN.sendMsgBuf(0x143,0,8,buf);
//  CAN.sendMsgBuf(0x144,0,8,buf);
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
//  pos=alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x142,0,8,buf);
//  
//  pos=alpha*negative_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x141,0,8,buf);
  /////////////////////////////////
  
  /////////////////////////////////
  /////bottom right stafe////
//  pos=alpha*negative_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x142,0,8,buf);
//
//  pos=alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x141,0,8,buf);
  //////////////////////////////////
  
  //////////////////////////////////
  ////top left stafe////
//  pos=alpha*negative_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x143,0,8,buf);
//
//  pos=alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x144,0,8,buf);
  //////////////////////////////////
  
  //////////////////////////////////
  ////bottom left strafe////
//  pos=alpha*negative_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x144,0,8,buf);
//
//  pos=alpha*positive_val;
//  pos=pos*3500;
//  buf[0] = 0xA3;
//  buf[1] = 0x00;
//  buf[2] = 0x00;
//  buf[3] = 0x00;
//  buf[4] = pos;
//  buf[5] = pos>>8;    
//  buf[6] = pos>>16; 
//  buf[7] = pos>>24;
//  CAN.sendMsgBuf(0x143,0,8,buf);
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

    
