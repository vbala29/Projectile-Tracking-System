# SPLAT-CIWS
Simple Projectile Locating and Aiming Tower (SPLAT) - a Close in Weapons System (CIWS) by Vikram Bala and Andy Liu. ESE350 Spring 2022 Final Project.


# Media

**S.P.L.A.T CIWS Full Demo Video**
https://youtu.be/s5mFUPCWUQY

**S.P.L.A.T CIWS Turret In Action** *(Short Video)*
https://youtu.be/M7niDRb0H9w

*The Turret System, Cameras, and Radar Operator LCD*
<img width="1427" alt="The Turret System with Cameras and Radar LCD" src="https://user-images.githubusercontent.com/56012430/165188330-ddabab07-831e-414f-8a18-d6097b6f1c5d.png">

*The Remote Command Website used to control Turrert Laser and Monitor Real Time Position of Turret*
<img width="1439" alt="The Remote Command Website for the Turret" src="https://user-images.githubusercontent.com/56012430/165188180-9a2c7414-455c-43a6-b066-fe21175eeee3.png">

# Components
**Servo Turret System (Relevant Files: src/Custom_Servo.c, include/Custom_Servo.h, src/main.c)**

We wrote a servo motor control library for the SG90 servo motor; the datasheet can be accesed [here](http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf). Note that this library was limited to the use of one 8-bit and one 16-bit timer on the Atmega328P microcontroller (see header file for the specific output PWM pins for the servos). This meant that both servos were not able to operate with the same level of precision in movements. Using this library, in main.c the system clock was prescaled by 1/2 to bring the frequency to 8MHz; however the library can operate at any CPU frequency so long CPU_FREQ and PRESCALE macros in it's header are changed. 
  
  
**262k-color TFT-LCD Radar Display (Relevant Files: LCD_GFX.h, LCD_GFX.c, LCD_main.c)**

We wrote a LCD graphics library for the ST7735R. The datasheet can be accessed [here](https://www.crystalfontz.com/controllers/Sitronix/ST7735R/). This library uses a standard Serial Peripheral Interface (SPI) communication protocol to write real time positional data about the tracked object to our LCD, thus making it akin to a "radar display" of objects around the turret. The red dot displayed in the demo video at the bottom of the LCD is to depict the position of the turret, while the blue dot shown is the position of the object being tracked relative to the turret, as if someone was facing in the direction that the cameras point at. We used a *separate* Atmega328p to control the LCD, due to the latency that writing to the LCD would cause if done on the same Atmega328p being used to control the servo motor turret. The LCD controlling Atmga328p received the positional data from the computer vision system using a UART based serial communication protocol (see Serial Library Component). 
