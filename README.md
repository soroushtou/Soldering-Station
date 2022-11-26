# Soldering-Station
This is the Arduino code for my DIY Soldering Station based on an ATmega8.

I needed a temperature-controlled soldering iron for my projects and for places where no power outlets are available. So I made this one; it is portable, easily controlled, and is powered by a wide range of 12-24V DC supplies.

![1](https://github.com/soroushtou/Soldering-Station/blob/main/images/1.jpg)

An Atmega8 is used for the brain part. The program utilizes a PID algorithm to control the temperature by reading the voltage from the thermocouple of the iron and the temperature we set with a rotary encoder.

The power parts of the circuit are mounted on a heatsink. A small 30x30mm fan blows the air to cool the heatsink. The soldering iron I used is a Hakko936 one.
It has a wide temperature range of 200-350 °C. You can power it with a car battery; It draws about 1-4 Amps depending on the source's voltage and the temperature difference.

![2](https://github.com/soroushtou/Soldering-Station/blob/main/images/2.jpg)

I apologize for my terrible soldering and wiring; back then, my old soldering iron was malfunctioning, and I didn't know much about making a PCB, so it turned out like this XD. However it is fully functional and meets my requirements.
