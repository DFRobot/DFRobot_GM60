# DFRobot_GM60

GM60 是一款高集成、性能优良的二维码扫描器；外形小巧；可以识别的条码和二维<br>
码格式有 QR Code, Data Matrix, PDF417,EAN13,UPC,Code 39,Code 93,Code<br>
 128,UCC/EAN 128 等<br>

![Product Image](./resources/images/SEN0486.png)

## 产品链接（https://www.dfrobot.com.cn/goods-3398.html)
    SEN0486：Gravity：环形二维码扫描识别传感器
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary


Provide an Arduino library to get Humidity and Temperature by reading data from dht20.

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods
```C++

  DFRobot_GM60();
  
  /**
   * @brief 初始化芯片
   * @return true(初始化成功)/false
   */
  bool begin();
  
  /**
   * @brief 读出数据的编码方式
   * @param encode 编码方式
                     eGBK,
                     eUTF8,
   * @return true(设置成功)/false(设置失败)
   */
  bool encode(eEncode_t encode);
  
  /**
   * @brief 设置码配置,可以通过使用模块扫描二维码,更改芯片寄存器的值
   * @param on true(打开设置码)/false(关闭设置码)
   * @param content true(输出设置码内容)/false(不输出设置码内容)
   * @return true(设置成功)/false(设置失败)
   */
  bool setupCode(bool on,bool content);
  
  /**
   * @brief 在输出的数据中增加包头数据
   * @param head 包头的具体数据 e3(0x03)/e4(0x04)
   * @return true(设置成功)/false(设置失败)
   */
  bool setHeadData(eHead_t head);
  
  /**
   * @brief 设置二维码识别的种类设置
   * @param berCode 
            eForbidAllBarcode, //禁止所有的二维码识别
            eEnableAllBarcode, // 使能所有的二维码识别
            eEnableDefaultcode, // 使能默认的二维码识别
   * @return true(设置成功)/false(设置失败)
   */
  bool setIdentify(eBarCode_t berCode);
  // bool start();
  
  /**
   * @brief 恢复出厂设置
   * @return true(设置成功)/false(设置失败)
   */
  bool reset();
  
  /**
   * @brief 检测扫描到的二维码包含的数据
   * @return  以字符串的方式返回扫描到的数据
   */
  String detection();
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
FireBeetle-ESP8266        |      √       |              |             | 
FireBeetle-ESP32        |      √       |              |             | 
Arduino MEGA2560        |      √       |              |             | 
Arduino Leonardo|      √       |              |             | 
Micro:bit        |      √       |              |             | 
FireBeetle-M0        |      √       |              |             | 
Raspberry Pi      |      √       |              |             | 


## History

- 2020/07/02 - Version 1.0.0 released.

## Credits

Written by fengli(li.feng@dfrobot.com), 2021.7.2 (Welcome to our [website](https://www.dfrobot.com/))





