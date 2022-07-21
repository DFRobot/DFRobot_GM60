# -*- coding: utf-8 -*
'''
  @file detect.py
  @brief Scan QR codes continuously and serial output the information contained in them
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @License     The MIT License (MIT)
  @author [fengli](li.feng@dfrobot.com)
  @version  V1.0
  @date  2021-6-29
  @https://github.com/DFRobot/DFRobot_GM60
'''

import sys
import time
sys.path.append("../")
from DFRobot_GM60 import *

IIC_MODE         = 0x01            # default use IIC1
IIC_ADDRESS      = 0x1A           # default i2c device address
'''
   # The first  parameter is to select iic0 or iic1
   # The second parameter is the iic device address
'''
asr = DFRobot_GM60_I2C(IIC_MODE ,IIC_ADDRESS)
# For using UART, uncomment the line below and connect hardware as required
#asr = DFRobot_GM60_UART(9600)
#Init chip
asr.begin()

'''
Set code config, the value of chip register can be changed by using the module to scan QR code
on true (Enable setting code)/false (Disable setting code)
content true (Output the set code content)/false (Not output the set code content)
'''
asr.setup_code(on = True,content = True)

'''
Read the data encoding mode
encode Encoding mode
         eGBK,//gbk encoding
         eUTF8,//utf8 encoding
'''
asr.encode(asr.UTF8)

'''
Set the available types of QR code for recognition
berCode:
eForbidAllBarcode, //Forbid all the QR code recognition
eEnableAllBarcode, // Enable all the QR code recognition
eEnableDefaultcode, // Enable the default QR code recognition
'''
asr.set_identify(asr.ENABLEALLBARCODE)
#It won't start to recognize codes until the terminal prints "start"
print("start\r\n")


while(1):
   time.sleep(0.05)
   #Detect the data contained in the scanned QR code, and return the scanned data as a character string.
   print(asr.response_payload())
   
