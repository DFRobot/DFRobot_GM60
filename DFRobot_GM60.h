/*!
 *@file DFRobot_GM60.h
 *@brief Define the basic structure of class DFRobot_GM60, the implementation of basic methods.
 *@details this module is used to identify the information in the QR code
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@License     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-28
 *@https://github.com/DFRobot/DFRobot_GM60
*/


#ifndef DFRobot_GM60_H
#define DFRobot_GM60_H

#include <Arduino.h>
#include <string.h>
#include <Wire.h>
//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif
//extern Stream *dbg;
class DFRobot_GM60
{
public:

#define REG_SETUP_CODE_CONFIG     0X0003  //Set code config
#define REG_ENCODE_SET            0x000D  //Output encoding config
typedef enum{
  eGBK,  //gbk encoding
  eUTF8, //utf8 encoding
}eEncode_t;
#define REG_DATA_WITH_PACKET      0X001A //Protocol packet header data
typedef enum{
 e3 = 0, //Packet header data is 0x03
 e4 = 1, //Packet header data is 0x04
 eNone = 3, // No packet header data 
}eHead_t;
#define REG_IDENTIFY_CONFIG      0x002C  //QR code recognition switch
typedef enum{
  eForbidAllBarcode, //Forbid all the QR code recognition
  eEnableAllBarcode, // Enable all the QR code recognition
  eEnableDefaultcode, // Enable the default QR code recognition
}eBarCode_t;


#define REG_IDENTIFY_EAN13       0X002E  

#define REG_IDENTIFY_EAN13-2     0X002F
#define REG_IDENTIFY_EAN13-5     0X0030
#define REG_IDENTIFY_EAN8        0X0031
#define REG_IDENTIFY_EAN8-2      0X0032
#define REG_IDENTIFY_EAN8-5      0X0033
#define REG_IDENTIFY_UPCA        0X0034
#define REG_IDENTIFY_UPCA-2        0X0035
#define REG_IDENTIFY_UPCA-5        0X0036
#define REG_IDENTIFY_UPCE0        0X0037
#define REG_IDENTIFY_UPCE1        0X0038
#define REG_IDENTIFY_UPCE1-2        0X0039
#define REG_IDENTIFY_UPCE1-5        0X003A
#define REG_IDENTIFY_CODE128        0X003B

#define REG_CODE128_MIN_LEN_SET        0X003C
#define REG_CODE128_MAX_LEN_SET        0X003D 
#define REG_IDENTIFY_CODE39        0X003E
#define REG_IDENTIFY_CODE32        0X003F
#define REG_IDENTIFY_CODE39_FULL_ASCII   0X0040


#define REG_CODE39_MIN_LEN_SET        0X0041
#define REG_CODE39_MAX_LEN_SET        0X0042
#define REG_IDENTIFY_CODE93           0X0043
#define REG_CODE93_MIN_LEN_SET        0X0044
#define REG_CODE93_MAX_LEN_SET        0X0045
#define REG_IDENTIFY_CODEBAR           0X0046
#define REG_CODEBAR_MIN_LEN_SET        0X0047
#define REG_CODEBAR_MAX_LEN_SET        0X0048
#define REG_IDENTIFY_QR                0X0049
#define REG_IDENTIFY_INT25               0X004A
#define REG_INT25_MIN_LEN_SET            0X004B
#define REG_INT25_MAX_LEN_SET            0X004C
#define REG_IDENTIFY_PDF417              0X004D
#define REG_IDENTIFY_DM                  0X004E

#define REG_OUTPUT_PROTOCOL_CONFIG       0X0060 //Protocol config register
#define REG_RECOVER_FACTORY_SET          0X00D9 //Register for restoring to factory settings

public:

  /**
   * @fn DFRobot_GM60
   * @brief Constructor 
   */
  DFRobot_GM60();
  
  /**
   * @fn begin
   * @brief Init chip
   * @return Boolean type, Indicates the initialization result
   * @retval true The setting succeeded
   * @retval false Setting failed
   */
  bool begin();
  
  /**
   * @fn encode
   * @brief Read the data encoding mode
   * @param encode Encoding mode
                   eGBK,  //gbk encoding
                   eUTF8, //utf8 encoding
   * @return Boolean type, the result of seted
   * @retval true The setting succeeded
   * @retval false Setting failed
  */
  bool encode(eEncode_t encode);
  
  /**
   * @fn setupCode
   * @brief Set code config, the value of chip register can be changed by using the module to scan QR code
   * @param on true (Enable setting code)/false (Disable setting code)
   * @param content true (Output the set code content)/false (Not output the set code content)
   * @return Boolean type, the result of seted
   * @retval true The setting succeeded
   * @retval false Setting failed
  */
  bool setupCode(bool on,bool content);
  
  /**
   * @fn setIdentify
   * @brief Set the available types of QR code for recognition
   * @param berCode 
            eForbidAllBarcode, //Forbid all the QR code recognition
            eEnableAllBarcode, // Enable all the QR code recognition
            eEnableDefaultcode, // Enable the default QR code recognition
   * @return Boolean type, the result of seted
   * @retval true The setting succeeded
   * @retval false Setting failed
  */
  bool setIdentify(eBarCode_t berCode);
  
  /**
   * @fn reset
   * @brief Restore to factory settings
   * @return Boolean type, the result of seted
   * @retval true The setting succeeded
   * @retval false Setting failed
  */
  bool reset();
  
   /**
   * @fn detection
   * @brief Detect the data contained in the scanned QR code
   * @return  return the scanned data as a character string
   */
  String detection();

  uint8_t communication;
private:

  String responsePayload();
  uint8_t readPrefix();

  /**
   * @fn setHeadData
   * @brief Add the packet header data to the output data
   * @param head The data of packet header e3(0x03)/e4(0x04)
   * @return true (The setting succeeds)/false (The setting failed)
   */
  bool setHeadData(eHead_t head);
  
  /**
   * @fn writeReg
   * @brief Write data to sensor register 
   * @param reg register
   * @param data  buf for store data to write 
   * @param lenght  The number of the data in pBuf
   * @return The number of successfully sent data
   */
  virtual uint8_t writeReg(uint16_t reg,uint8_t data[],uint8_t lenght)=0;
  
  /**
   * @fn readReg
   * @brief read data from sensor chip register
   * @param reg chip register 
   * @param data  buf for store data to read 
   * @param length  number of data to read
   * @return The number of successfully read data
   */
  virtual uint8_t readReg(uint16_t reg,uint8_t data[],uint8_t length)=0;
  
  /**
   * @fn read
   * @brief read data from sensor
   * @param data  buf for store data to read 
   * @param length  number of data to read
   * @return The number of successfully read data
   */
  virtual size_t read(uint8_t* buf_, size_t len)=0; 
   
  TwoWire *_pWire;

  uint8_t _address;
  
};

class DFRobot_GM60_IIC : public DFRobot_GM60{
public: 
  /**
   * @fn DFRobot_GM60_IIC
   * @brief Constructor 
   * @param pWire I2c controller
   * @param address  I2C address(0x1A)
   */
   DFRobot_GM60_IIC(TwoWire *pWire = &Wire, uint8_t address = 0x1A);
   
  /**
   * @fn begin
   * @brief Init communication port
   * @return true or false
   */
   bool begin();
protected:

  /**
   * @fn writeReg
   * @brief Write data to sensor register 
   * @param reg register
   * @param data  buf for store data to write 
   * @param lenght  The number of the data in pBuf
   * @return The number of successfully sent data
   */
   virtual uint8_t writeReg(uint16_t reg,uint8_t data[],uint8_t lenght);

  /**
   * @fn readReg
   * @brief read data from sensor chip register
   * @param reg chip register 
   * @param data  buf for store data to read 
   * @param length  number of data to read
   * @return The number of successfully read data
   */
   virtual uint8_t readReg(uint16_t reg,uint8_t data[],uint8_t length);
   
  /**
   * @fn read
   * @brief read data from sensor
   * @param buf  buf for store data to read 
   * @param len  number of data to read
   * @return The number of successfully read data
   */
   virtual size_t read(uint8_t* buf, size_t len);
private:
  TwoWire *_pWire;
  uint8_t _deviceAddr;
};

class DFRobot_GM60_UART : public DFRobot_GM60{

public: 

  /**
   * @fn DFRobot_GM60_UART
   * @brief Constructor 
   */
  DFRobot_GM60_UART();
  /**
   * @fn begin
   * @brief Init communication port
   * @param s_ Software serial or hardware serial 
   * @return true or false
   */
  bool begin(Stream &s_);
protected:

  /**
   * @fn writeReg
   * @brief Write data to sensor register 
   * @param reg register
   * @param data  buf for store data to write 
   * @param lenght  The number of the data in pBuf
   * @return The number of successfully sent data
   */
  virtual uint8_t writeReg(uint16_t reg,uint8_t data[],uint8_t lenght);

  /**
   * @fn readReg
   * @brief read data from sensor chip register
   * @param reg chip register 
   * @param data  buf for store data to read 
   * @param length  number of data to read
   * @return The number of successfully read data
   */
  virtual uint8_t readReg(uint16_t reg,uint8_t data[],uint8_t length);
   
  /**
   * @fn read
   * @brief read data from sensor
   * @param buf_  buf for store data to read 
   * @param len  number of data to read
   * @return The number of successfully read data
   */
  virtual size_t read(uint8_t* buf_, size_t len);
private:

  Stream *s;
};
#endif
