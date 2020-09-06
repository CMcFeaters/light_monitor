This project is remote sensor that monitors solar power and reports data back to a server via wifi.  
For testing purposes Tiny-VA meter was used and modified to allow for 
data extraction via pyserial and to interface with the cheap knockoff LCD I used.

Server4.py is intended to run on a separate server
client_test_photo_resist.ino is intended to run on a esp8266
Serial_monitor_power is intended to monitor the output from the tiny-va-meter
Serial_monitor_state is intended to monitor the output from the solar_chip.
The notebook file does a basic plot of the power consumption of the chip.

When I get aroudn to it I will add schematics of the board. 



Tiny VA meter was added for testing purposes.
Tiny-VA-Meter is pulled from this webiste:
https://www.instructables.com/id/Tiny-VA-Meter-With-INA219/
And this repo:
https://github.com/KarlTorp/Tiny-VA-Meter

