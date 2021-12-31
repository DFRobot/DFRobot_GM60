# -*- coding: utf-8 -*
'''
  @file DFRobot_GM60.py
  @brief Define the basic structure of class DFRobot_GM60, the implementation of basic methods.
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT license (MIT)
  @author [fengli](li.feng@dfrobot.com)
  @version  V1.0
  @date  2021-6-28
  @https://github.com/DFRobot/DFRobot_GM60
'''


import serial
import time
import smbus
                

class DFRobot_GM60(object):
  REG_SETUP_CODE_CONFIG   = 0X0003#Set code config
  
  REG_ENCODE_SET          =  0x000D#Output encoding config
  
  GBK                      = 0  #gbk encoding
  UTF8                     = 1   #utf8 encoding
  
  REG_DATA_WITH_PACKET     = 0X001A  #Protocol packet header data
  
  E3                      = 0  #Packet header data is 0x03
  E4                      = 1  #Packet header data is 0x04
  ENONE                   = 3  #No packet header data 

  REG_UART_COMM_CONFIG     = 0x002A
  REG_IDENTIFY_CONFIG      = 0x002C #QR code recognition switch
  
  FORBIDALLBARCODE   = 0  #Forbid all the QR code recognition
  ENABLEALLBARCODE   = 1  #Enable all the QR code recognition
  ENABLEDEFAULTCODE  = 2  #Enable the default QR code recognition
  
  I2C_MODE                  = 0x01  
  UART_MODE                 = 0x02
  REG_IDENTIFY_EAN13      = 0X002E
  
  REG_IDENTIFY_EAN13_2    = 0X002F
  REG_IDENTIFY_EAN13_5    = 0X0030
  REG_IDENTIFY_EAN8       = 0X0031
  REG_IDENTIFY_EAN8_2     = 0X0032
  REG_IDENTIFY_EAN8_5     = 0X0033
  REG_IDENTIFY_UPCA       = 0X0034
  REG_IDENTIFY_UPCA_2     =   0X0035
  REG_IDENTIFY_UPCA_5     =   0X0036
  REG_IDENTIFY_UPCE0      =  0X0037
  REG_IDENTIFY_UPCE1      =  0X0038
  REG_IDENTIFY_UPCE1_2    =    0X0039
  REG_IDENTIFY_UPCE1_5     =   0X003A
  REG_IDENTIFY_CODE128     =   0X003B
  
  REG_CODE128_MIN_LEN_SET   =     0X003C
  REG_CODE128_MAX_LEN_SET   =     0X003D 
  REG_IDENTIFY_CODE39       = 0X003E
  REG_IDENTIFY_CODE32        =0X003F
  REG_IDENTIFY_CODE39_FULL_ASCII  = 0X0040
  
  
  REG_CODE39_MIN_LEN_SET     =   0X0041
  REG_CODE39_MAX_LEN_SET     =   0X0042
  REG_IDENTIFY_CODE93        =   0X0043
  REG_CODE93_MIN_LEN_SET     =   0X0044
  REG_CODE93_MAX_LEN_SET     =   0X0045
  REG_IDENTIFY_CODEBAR       =    0X0046
  REG_CODEBAR_MIN_LEN_SET     =   0X0047
  REG_CODEBAR_MAX_LEN_SET     =   0X0048
  REG_IDENTIFY_QR             =   0X0049
  REG_IDENTIFY_INT25          =      0X004A
  REG_INT25_MIN_LEN_SET      =  0X004B
  REG_INT25_MAX_LEN_SET      =  0X004C
  REG_IDENTIFY_PDF417         =       0X004D
  REG_IDENTIFY_DM             =     0X004E
  
  REG_OUTPUT_PROTOCOL_CONFIG   =    0X0060#Protocol config register
  REG_RECOVER_FACTORY_SET      =    0X00D9#Register for restoring factory settings
  ''' register configuration '''
  
  
  ''' Conversion data '''

  def __init__(self ,bus ,Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = self.I2C_MODE
      self.write_data(0xaa)
    else:
      #print("uart")
      self.ser = serial.Serial("/dev/ttyAMA0" ,baudrate=Baud,stopbits=1, timeout=0.5)
      self.__uart_i2c = self.UART_MODE
      if self.ser.isOpen == False:
        self.ser.open()
        #print("u1")

  def begin(self):
  '''
    @fn begin
    @brief Init chip
  '''
    self.reset()
    self.set_head_data(self.E4)
  
  def reset(self):
  '''
    @fn reset
    @brief Restore factory settings
    @return true (The setting succeeds)/false (The setting failed)
  '''  
  data = 0x55
    ack = self.write_reg(self.REG_RECOVER_FACTORY_SET,data,1)
    if ack == 0x02:
      return True
    else:
      return False
      

  def setup_code(self,on,content):
  '''
    @fn setup_code
    @brief Set code config, the value of chip register can be changed by using the module to scan QR code
    @param on true (Enable setting code)/false (Disable setting code)
    @param content true (Output the set code content)/false (The set code content is not output)
    @return true (The setting succeeds)/false (The setting failed)
  '''
    data = 0
    self.set_head_data(self.E4)
    if on == False:
      data = 1 << 1
    data = data | content
    ack = self.write_reg(self.REG_SETUP_CODE_CONFIG,data,1)
    if ack == 0x02:
      return True
    else:
      return False


  def encode(self,encode):
  '''
    @fn encode
    @brief Read the data encoding scheme
    @param encode Encoding scheme
                    eGBK,
                    eUTF8,
    @return true (The setting succeeds)/false (The setting failed)
  '''
    data = 0
    if encode == self.GBK:
      data = 0
    elif encode == self.UTF8:
      data = 0x3c
    
    ack = self.write_reg(self.REG_ENCODE_SET,data,1)
    if ack == 0x02:
      return True
    else:
      return False


  def set_head_data(self,head):
  '''
    @fn set_head_data
    @brief Add the packet header data into the output data
    @param head The concrete data of packet header e3(0x03)/e4(0x04)
    @return true (The setting succeeds)/false (The setting failed)
  '''
    data = 0x80
    ack = self.write_reg(self.REG_OUTPUT_PROTOCOL_CONFIG,data,1)
    data = head << 6
    ack = self.write_reg(self.REG_DATA_WITH_PACKET,data,1)
    if ack == 0x02:
      return True
    else:
      return False
      

  def set_identify(self,berCode):
  '''
    @fn set_identify
    @brief Set the available types of QR code for recognition
    @param berCode 
           eForbidAllBarcode, //Forbid all the QR code recognition
           eEnableAllBarcode, // Enable all the QR code recognition
           eEnableDefaultcode, // Enable the default QR code recognition
    @return true (The setting succeeds)/false (The setting failed)
  '''
    data = berCode <<1
    ack = self.write_reg(self.REG_IDENTIFY_CONFIG,data,1)
    if ack == 0x02:
      return True
    else:
      return False
      

  def response_payload(self):
  '''
    @fn response_payload
    @brief Detect the data contained in the scanned QR code
    @return  Return the scanned data as a character string
  '''
     time.sleep(0.01)
     stri = ""
     if self.read_prefix() == 0:
       head = self.read(2)
       time.sleep(0.03)
       data = self.read(head[1])
       for i in range(0,head[1]):
           stri += chr(data[i])
      
     return stri
     

  def read_prefix(self):
  '''
    @fn read_prefix
    @brief Read the packet header data
    @return  Return (0) when the read packet header data is 0x04, otherwise, return (1)
  '''
     for i in range(0,200):
       time.sleep(0.01)
       prefix = self.read(1)
       if prefix[0] == 0x04:
         return 0
     return 1
class DFRobot_GM60_I2C(DFRobot_GM60): 

  def __init__(self ,bus ,addr):
    self.__addr = addr
    super(DFRobot_GM60_I2C, self).__init__(bus,0)
    

  def write_reg(self, reg, data,len):
  '''
    @fn write_reg
    @brief Write data to register
    @param reg 16-bit register address
    @param data 8-bit config register value
    @param len  Continuously configure register numbers
  '''
    packet = [0x7E,0x00,0x08,0x01,0x00,0x00,0x22,0xAB,0xCD]
    packet[4] = reg>>8
    packet[5] = reg&0xff
    packet[6] = data
    for i in range(0,9):
      self.i2cbus.write_byte(self.__addr ,packet[i])
 
    for i in range(0,200):
       time.sleep(0.01)
       prefix = self.read(1)
       if prefix[0] == 0x02:
         break;
    return 0
  

  def read_reg(self,reg,data,length):
  '''
    @fn read_reg
    @brief Read data from the register
    @param reg 16-bit register address
    @param data Storage location for the read register value
    @param length  Continuously read register numbers
  '''
    packet = [0x7E,0x00,0x07,0x01,0x00,0x00,0x22,0xAB,0xCD]
    packet[4] = reg>>8
    packet[5] = reg&0xff
    packet[6] = length
    
    for i in range(0,9):
      self.i2cbus.write_byte(self.__addr ,packet[i])
    time.sleep(2);
    
    for i in range(0,7):
       data[i] = self.i2cbus.read_byte(self.__addr)
    return 0
       

  def write_data(self, data):
  '''
    @fn write_data
    @brief Write a data to the sensor
    @param data The data to be written
  '''
      self.i2cbus.write_byte(self.__addr ,data)
    

  def read(self,len):
  '''
    @fn read
    @brief Read data from the sensor
    @param len Length of the data to be read
    @return Return the read data as an array
  '''
    buf = [0]*len
    for i in range(0,len):
       time.sleep(0.001)
       buf[i] = self.i2cbus.read_byte(self.__addr)
    #print(buf)
    return buf
    
class DFRobot_GM60_UART(DFRobot_GM60): 

  def __init__(self ,Baud):
    self.__Baud = Baud;
    super(DFRobot_GM60_UART, self).__init__(0,Baud)


  def write_reg(self, reg, data,len):
  '''
    @fn write_reg
    @brief Write data to register
    @param reg 16-bit register address
    @param data 8-bit config register value
    @param len  Continuously configure register numbers
  '''
    packet = [0x7E,0x00,0x08,0x01,0x00,0x00,0x22,0xAB,0xCD]
    packet[4] = reg>>8
    packet[5] = reg&0xff
    packet[6] = data
    self.ser.write(packet)
    for i in range(0,200):
       time.sleep(0.01)
       prefix = self.read(1)
       if prefix[0] == 2:
         self.read(10)
         break;
    return 0
  

  def read_reg(self,reg,data,length):
  '''
    @fn read_reg
    @brief Read data from the register
    @param reg 16-bit register address
    @param data Storage location for the read register value
    @param length  Continuously read register numbers
  '''
    packet = [0x7E,0x00,0x07,0x01,0x00,0x00,0x22,0xAB,0xCD]
    packet[4] = reg>>8
    packet[5] = reg&0xff
    packet[6] = length
    
    self.ser.write(packet)
    time.sleep(2);
    
    for i in range(0,7):
       data[i] = self.ser.read(1)
    return 0


  def read(self,len):
  '''
    @fn read
    @brief Read data from the sensor
    @param len Length of the data to be read
    @return Return the read data as an array
  '''
    buf = [0]*len
    for i in range(0,len):
       #time.sleep(0.001);
       try:
         buf[i] = ord(self.ser.read(1))
       except:
         buf[i] = 255
    return buf
