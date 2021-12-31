
/*!
 *@file detect_uart.ino
 *@brief get the qr code information
 *@details  Scan QR codes continuously and serial output the information contained in codes.
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-29
 *@https://github.com/DFRobot/DFRobot_GM60
*/

#include <DFRobot_GM60.h>
DFRobot_GM60_UART  gm60;
/*Use software serial when using UNO or NANO*/
#if ((defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO))
    #include <SoftwareSerial.h>
    SoftwareSerial Serial1(2, 3);  //RX, TX
    #define FPSerial Serial1
    
#else
    #define FPSerial Serial1
#endif

void setup(){
  //Rx and tx should be mapping into D2 and D3 when using Firebeetle-ESP32
  #if (defined ESP32)
     Serial1.begin(9600, SERIAL_8N1, /*rx =*/D2, /*tx =*/D3);
  #else
  Serial1.begin(9600);
  #endif
  Serial.begin(115200);
  //Init chip
  gm60.begin(Serial1);
  //Restore to factory settings
 // gm60.reset();

  /**
  Read the data encoding mode
  encode Encoding mode
           eGBK,//gbk encoding
           eUTF8,//utf8 encoding
   */
  gm60.encode(/*encode = */gm60.eUTF8);
  
  /*! Set code config, the value of chip register can be changed by using the module to scan QR code
  on true (Enable setting code)/false (Disable setting code)
  content true (Output the set code content)/false (Not output the set code content)
  */
  gm60.setupCode(/*on =*/true,/*content=*/true);
  
  /**
    Set the available types of QR code for recognition
    berCode:
    eForbidAllBarcode, //Disable all the QR code recognition
    eEnableAllBarcode, // Enable all the QR code recognition
    eEnableDefaultcode, // Enable the default QR code recognition
  */
  gm60.setIdentify(/*berCode = */gm60.eEnableAllBarcode);
  Serial.println("Start to recognize");
}

void loop(){
  delay(50);
  // Detect the data contained in the scanned QR code, and return the scanned data as a character string.
  Serial.println(gm60.detection());

}
