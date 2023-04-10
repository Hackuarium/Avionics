# Avionics
Electronic design of an avionics for mini rockets.
## Goals
- Monitor flight data
  - Acceleration X,Y,Z
  - Rotation X,Y,Z
  - Pressure (for altitude)
  - Video
- Separation sequences (separation of stage 2, of nose, ...)
  - 2-3 Pyro-channels
  - 2-3 Servos control channels
  - Electomagnets ?
  - Valves ?
## Setup
On top of an ESP32C3 board connect an **Extension Board**. Both of wich are powered by a mini lithium battery. Other battery for igniter (min 1.5v)
## Extension board for ESP32C3 (Micro Controler = µC)
- Connect all the ports from the ESP to the extension board (with pin header ?)
- LED connected to a random port for test (BLINK - also allows to see if µC bugging or lagging)
- Communication I2C with µC
- Battery powered. Option ot charge just before launch (batery pack, umbilical external connector)?
- Switch to turn ON and OFF ? Access from outside the rocket.

## Ideas
- Monitor battery voltage (voltage divider bridge with 2x 100 kohm)
- Connexion for igniter of separation system
  - Know if an igniter is connected or not (VCC to Igniter to Transistor - connexion to GND with 1 mohm and connexion to I/O with 100 kohm)
- Strong connector to avoid unwanted disconnection

## Reference

### Components and features:

- ESP32C3 from seeed https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/
![images/ESP32-C3_pin_map-2.png]
- Accelererometer GY521, MPU6050:
  - https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
  - https://parsippany.vidyalaya.org/fut102-gy521.pdf
- Power switch for igniter: ZXMS6004FF
  - https://www.diodes.com/assets/Datasheets/ZXMS6004FF.pdf
- Barometric pressure sensor: MS5637
  - https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5637-02BA03%7FB1%7Fpdf%7FEnglish%7FENG_DS_MS5637-02BA03_B1.pdf%7FCAT-BLPS0037
- pins to connect servo
- pins for testing

### Miscelaneous

- I2C protocol: https://en.wikipedia.org/wiki/I%C2%B2C
