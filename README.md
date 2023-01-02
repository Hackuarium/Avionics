# Avionics
Electronic design of an avionics for mini rockets.
## Goals
- Monitor flight data
  - Acceleration X,Y,Z
  - Rotation X,Y,Z
  - Pressure (for altitude)
  - Video
- Separation sequences (separation of stage 2, of nose, ...)
## Setup
On top of an ESP32C3 board connect an **Extension Board**. Both of wich are powered by a mini lithium battery. Other battery for igniter (min 1.5v)
## Extension board for ESP32C3 (Micro Controler = µC)
- Connect all the ports from the ESP to the extension board (with pin header ?)
- LED donnected to a random port for test (BLINK - also alow to see if µC bugging or lagging)
- Communication I2C with µC
- Battery powered. Option ot charge just before launch (batery parck, umbilical external connector)?
- Switch to turn ON and OFF ? Access from outside the rocket.

## Ideas
- Monitor battery voltage (voltage divider bridge with 2x 100 kohm)
- Connexion for igniter of separation system
  - Know if an igniter is connected or not (VCC to Igniter to Transistor - connexion to GND with 1 mohm and connexion to I/O with 100 kohm)
 

