# Control-the-postion-of-linear-actuator
In this test case, we are considering STM32(Blue pill) as a microcontroller to control the actuator via  a relay and use push button switches to enable the extend, shrink, stop, and homing functions.
In this test case, we are considering STM32(Blue pill) as a microcontroller to control the actuator via 
a relay and use push button switches to enable the extend, shrink, stop, and homing functions.
Port ‘B’ Pin of 5, 6, 4, and 7 are used as input pins.
Port ‘B’ Pin of 8 and 9 is used as output pin.
Port ‘A’ Pin of 1 is used as an analog input to receive potentiometer value from the actuator to 
determine the position.
There are four functions written to control the microcontroller.
1. extend(void).
2. shrink(void)
3. stop(void)
4. homing(void).
In actuator potentiometer values are ranging from 0 to 10K Ohm resistance, these resistances give a 
voltage range of 0 to 5V. The value 0 to 5V range is mapped to a value of 0 to 4095 (2^12 bit 
resolution). STM32 has a 12-bit resolution ADC. The value between 2042 and 2052 is mid-range.
![Screenshot 2023-06-07 112401](https://github.com/gauman/Control-the-postion-of-linear-actuator/assets/78633686/02fba615-e244-4901-84d7-42b26d85ae1f)
![9-STM32-pinout-diagram](https://github.com/gauman/Control-the-postion-of-linear-actuator/assets/78633686/0daac3d5-db35-46be-a3cc-bf080f6737b5)
