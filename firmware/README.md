Wireless sensor module firmware
===============================

**Features**:

* well-commented
* easy to extend
* configurable through serial terminal
* optimized for power consumption (deep sleep mode)
* monitors actual voltage values
* compatible with ATmega328P @ 16MHz / 8MHz 


Requirements
------------

* [Arduino IDE](https://www.arduino.cc/)
* [Sparkfun BMP180 library](https://github.com/sparkfun/BMP180_Breakout)
* [Sparkfun HTU21D library](https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library)
* [Sparkfun TSL2561 library](https://github.com/sparkfun/SparkFun_TSL2561_Arduino_Library)
* "Minimal circuit" hardware configuration from [Arduino website](https://www.arduino.cc/en/Tutorial/ArduinoToBreadboard)*
* chip programmer* (like USBap)
* serial converter* (some USB TTL Serial)

*) Only if you are going to used it with provided PCB board


Installing on ATmega328p 
------------------------

1. prepare ATmega328p chip fused to run at 8Mhz internal oscillator
2. install required libraries through Arduino IDE's library manager 
3. compile and upload sketch to ATmega328p using your favourite chip programmer 
4. put programmed ATmega328p into socket on the board. 


Configuring the module
----------------------

1. power off the board
2. connect serial converter to JP2 connector on the board
3. setup up and open serial connection 
  
  * boud rate: 19200
  * parity: none
  * bits: 8
  * stopbits: 1
  * flow control: none
  * echo: off
  * immediate send characters 
  
4. power on the board
5. within next 5 seconds, press any key in serial terminal to enter setup mode


[screen z terminalu]
 

** Available commands **

|  Key  | Description                                                                                | Expected value     | Note                                                                                             |
|-------|--------------------------------------------------------------------------------------------|--------------------|--------------------------------------------------------------------------------------------------|
| **I** | Set Device ID. The ID will be send to the server with every message.                       | Integer (0-255)    | Value should be unique for every module.                                                         |
| **1** | Set radio channel number to communicate with server.                                       | Integer (1-125)    | Value should be the same for every module and the server.                                        |
| **2** | Set module radio address that NFR24L01 will use as a source address for each message.      | 3x Integer (0-255) | Value should be unique for every module.                                                         |
| **3** | Set server radio address that NFR24L01 will use as a destination address for each message. | 3x Integer (0-255) | Value should be the same for every module.                                                       |
| **4** | Set reading interval in seconds.                                                           | Integer (1-255)    | This affects most energy consumption.                                                            |
| **5** | Set input voltage reading correction factor.                                               | Float              | *factor = (voltage that you measure with multimeter on VCC @ JP1) / (voltage measured by module)*|
| **6** | Set regulated voltage reading correction factor.                                           | Float              | *factor = (voltage that you measure with multimeter on VCC @ U4) / (voltage measured by module)* |
| **7** | Enable/disable BMP180                                                                      | -                  | If you connect BMP180 to the module, set it to *Y*                                               |
| **8** | Enable/disable TSL2561                                                                     | -                  | If you connect TSL2561 to the module, set it to *Y*                                              |
| **9** | Enable/disable HTU21                                                                       | -                  | If you connect HTU21 to the module, set it to *Y*                                                |
| **0** | Set altitude in meters (from sea level).                                                   | Integer (0-65535)  | Required if you connected BMP180. Use GPS to determine the altitude.                             |
| **R** | Reset settings to defaults.                                                                |                    |                                                                                                  |
| **S** | Write settings to EEPROM.                                                                  |                    |                                                                                                  |
| **X** | Exit setup mode.                                                                           |                    |                                                                                                  |
 
Each next digit of value that you entering must be entered within 5 seconds after previous one.
Each number can be ended/confirmed by any non-digit character (or you can wait 5 seconds). 
If selected command requires a few numbers, then separate them with any non-digit character.
Floating-point number can be entered as usual - with dot between digits. 

When you exit setup mode, the module will start its normal work cycle and will print on serial terminal current measurements.
When you power on the module and do not enter setup mode, serial terminal will be disabled.

[screen z terminalu]