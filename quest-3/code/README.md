# Code Readme

This code folder contains an html file, a node js file, and finally a quest3.c file. The C file calculates and reads the data from both the thermistor and the accelerometer. This data is organized in csv-format to make graphing the data much simpler. After the sensor data is retrieved by the ESP, it is sent to the node server on a laptop. This nodejs file also uses the serial port module (refer to skill 16), and organizes the data into a csv file that is then sent to an html file for graphing. The html file graphs the sensor data, updating with an interval of 3 seconds. It displays the temperature of the thermistor, as well as the pitch roll values from the accelerometer to signify vibrations (hence the "hurricane" box). 

*References for code specified in report.md file for Quest 3
