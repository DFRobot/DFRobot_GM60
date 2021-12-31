/*!
 *@file DFRobot_GM60.cpp
 *@brief Define the basic structure of class DFRobot_GM60, the implementation of the basic methods
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@License     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-28
 *@https://github.com/DFRobot/DFRobot_GM60
*/

#include <DFRobot_GM60.h>

DFRobot_GM60::DFRobot_GM60() {
   
}

bool DFRobot_GM60::begin() {
  
  return reset();;

}
bool DFRobot_GM60::reset(){

  uint8_t data = 0x55;
  uint8_t ack;
  ack = writeReg(REG_RECOVER_FACTORY_SET,&data,1);
  if(ack == 0x02){
    return true;
  } else { 
    return false;
 }
}

bool DFRobot_GM60::setupCode(bool on,bool content){
  uint8_t data = 0;
  uint8_t ack;
  setHeadData(e4);
  if(on == false){
    data = 1 << 1;
  }
  data = data | content;
  ack = writeReg(REG_SETUP_CODE_CONFIG,&data,1);
  if(ack == 0x02){
    return true;
  } else { 
    return false;
 }
}
bool DFRobot_GM60::encode(eEncode_t encode)
{
  uint8_t ack;
  uint8_t data = 0;
  switch(encode){
    case eGBK :{
      data = 0 ;
      break;
	}
    case eUTF8 :{
      data = 0x3c ;
	  break;
	}
  }
  ack = writeReg(REG_ENCODE_SET,&data,1);
  if(ack == 0x02){
    return true;
  } else { 
    return false;
 }
}
bool DFRobot_GM60::setHeadData(eHead_t head){
  uint8_t ack;
  uint8_t data = 0x80;
  writeReg(REG_OUTPUT_PROTOCOL_CONFIG,&data,1);
  data = head << 6;
  ack = writeReg(REG_DATA_WITH_PACKET,&data,1);
  if(ack == 0x02){
    return true;
  } else { 
    return false;
 }
  
}
bool DFRobot_GM60::setIdentify(eBarCode_t berCode)
{
  uint8_t ack;
  uint8_t data = berCode <<1;
  
  ack = writeReg(REG_IDENTIFY_CONFIG,&data,1);
  if(ack == 0x02){
    return true;
  } else { 
    return false;
 }
}
String DFRobot_GM60::responsePayload(){
  uint8_t head[2];
  String str = "";
  delay(10);
  if(readPrefix() == 0){
    read(head,2);
    uint8_t *data = (uint8_t *)malloc(head[1]);

    read(data,head[1]);
    for(uint8_t i = 0 ;i < head[1];i++){
     str += char(data[i]);
    }
    free(data);
    return str;
  }
  return "null";
}

String DFRobot_GM60::detection()
{
  return responsePayload();
}
uint8_t DFRobot_GM60::readPrefix(){

  uint8_t head[2];
  long time1 = millis();
  while(true){
    read(head,1);
    if(communication)
      delay(150);
    if(head[0] == 0x04){
      break;
    }
    if((millis() - time1) > 2000){
      return 1;
    }
  }
  return 0;
}

DFRobot_GM60_IIC::DFRobot_GM60_IIC(TwoWire *pWire, uint8_t address )
{
_pWire  = pWire;
_deviceAddr = address;
}

bool DFRobot_GM60_IIC::begin(){

  _pWire->begin();
  _pWire->beginTransmission(_deviceAddr);
    _pWire->write(0xaa);
   communication = 1;
  _pWire->endTransmission();
  return DFRobot_GM60::begin();
}
uint8_t DFRobot_GM60_IIC::writeReg(uint16_t reg,uint8_t data[],uint8_t lenght)
{
  uint8_t packet[] = {0x7E,0x00,0x08,0x01,0x00,0x00,0x22,0xAB,0xCD};
  packet[4] = reg>>8;
  packet[5] = reg&0xff;
  packet[6] = data[0];
  _pWire->beginTransmission(_deviceAddr);
  for(uint8_t i = 0 ; i < 9 ; i++){
    _pWire->write(packet[i]);
  }
  _pWire->endTransmission();
  uint8_t retry = 100;
  while(retry--){
    uint8_t ack;
    read(&ack,1);
    if(ack == 0x02)return 0x02;
        delay(20);
  }
  return 0;
}
uint8_t DFRobot_GM60_IIC::readReg(uint16_t reg,uint8_t data[],uint8_t length)
{
  uint8_t packet[] = {0x7E,0x00,0x07,0x01,0x00,0x00,0x22,0xAB,0xCD};
  packet[4] = reg>>8;
  packet[5] = reg&0xff;
  packet[6] = length;
  _pWire->beginTransmission(_deviceAddr);
  for(uint8_t i = 0 ; i < 9 ; i++){
    _pWire->write(packet[i]);
  }
  _pWire->endTransmission();
  delay(2000);
  
  for(uint8_t i = 0 ; i < 7 ;i++){
     _pWire->requestFrom(_deviceAddr,1);
     packet[i] = _pWire->read();
	 DBG(packet[i])

  }

  return 0;
}
size_t DFRobot_GM60_IIC::read(uint8_t* buf, size_t len){
  
  for(uint8_t i = 0 ;i < len ;i++){
    _pWire->requestFrom(_deviceAddr,1);
    buf[i] = _pWire->read();
    DBG(buf[i])
   }
  return 0;
}
DFRobot_GM60_UART::DFRobot_GM60_UART(){}
bool DFRobot_GM60_UART::begin(Stream &s_){
   s = &s_;
   if(s != NULL){
     communication = 0;
     return DFRobot_GM60::begin();
   }
   
   return false;
}
uint8_t DFRobot_GM60_UART::writeReg(uint16_t reg,uint8_t data[],uint8_t lenght)
{
  uint8_t packet[] = {0x7E,0x00,0x08,0x01,0x00,0x00,0x22,0xAB,0xCD};
  packet[4] = reg>>8;
  packet[5] = reg&0xff;
  packet[6] = data[0];
  long time1 = millis();
  for(uint8_t i = 0 ; i < 9 ; i++){
    s->write(packet[i]);
  }
  while(true){
    delay(20);
    if(s->read() == 0x02){
      for(uint8_t i = 0 ; i < 6 ;i++){
        packet[i] = s->read();
      }
      break;
    }
    if((millis() - time1) > 2000){
      return 1;
    }
  }
  if(packet[0] == 0x02){
     return 0x02;
  } else {
     return 0;
  }
}
uint8_t DFRobot_GM60_UART::readReg(uint16_t reg,uint8_t data[],uint8_t length){

  uint8_t packet[] = {0x7E,0x00,0x07,0x01,0x00,0x00,0x22,0xAB,0xCD};
  packet[4] = reg>>8;
  packet[5] = reg&0xff;
  packet[6] = length;
  for(uint8_t i = 0 ; i < 9 ; i++){
    s->write(packet[i]);
  }
  delay(1000);
  
  for(uint8_t i = 0 ; i < 7 ;i++){
    data[i] =s->read();
    
  }
  return 0;

}
size_t DFRobot_GM60_UART::read(uint8_t * buf, size_t len)
{
   uint8_t offset = 0;

   delay(10);
   while(s->available()){
       buf[offset++] = s->read();
     delay(1);
    len--;
    if(len == 0)break;
   }

   return 0;
}