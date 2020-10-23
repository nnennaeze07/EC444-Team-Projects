# Hurricane Box with Remote Access
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-10-23
-----

## Summary
This quest combined skills 19 to 24 to read data from two sensors connected to the ESP32. This data (temperature for the thermistor sensor, and pitch and roll for the accelerometer) was graphed on a remote client. The graph is updated continuously every 3 seconds. There was also an LED connected to the ESP32, which could be turned on or off from the remote web client. Finally, the raspberry pi zero camera was connected, to record that the LED was in fact toggling when the user requested it to do so. Tjos was done through the wifi access of a loaner router, provided by Professor Little. 

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Objective One | 1 |  1     | 
| Objective Two | 1 |  1     | 
| Objective Three |  |  1     | 
| Objective Four | 1 |  1     | 
| Objective Five | 1 |  1     | 
| Objective Six |  |  1     | 
| Objective Seven | 1 |  1     | 


### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution |  |  5     | 
| Quality of report.md including use of graphics |  |  3     | 
| Quality of code reporting |  |  3     | 
| Quality of video presentation |  |  3     | 


## Solution Design
In order to complete this quest we took the list of requirements and took it step by step. First, we combined the ideas from skill 13 and skill 23 to connect both the thermistor and accelerometer to the ESP32 and print out its tilt and temperature, respectively. Then, we used Node.js to graph the sensor's data periodically. Next, we configured the ESP32 to connect to WiFi. After, we used the ESP32 as a UDP client, we transferred data from the the server in order to turn an LED on or off once a button was pressed. Last we created a webcam to indicate the LED in realtime.

When working with this quest, our team ran into a couple of issues in configuring the UPD Client to control the LED and DDNS. With the UDP Client, we mapped out the steps of UPD protocol. We learned that we needed to establish a connection with the ESP and Node server, and that this communication occured in the same network behind a router. A UDP server was added as a function the the Node server and the ESP32 (client) periodically opens a connection to the server to send some information. Once the server reveices this information a confirmation is send back to the client. This two-way communication proved useful in controlling the LED. For the DDNS,...


## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>


## Supporting Artifacts
- [Link to video demo](). Not to exceed 120s


## Modules, Tools, Source Used Including Attribution
- Router Setup
- Pi Configuration
- Pi Web Camera
- LED PWM
- GPIOs
- Thermistor
- Accelerometer
- DDNS
- Port Forwarding
- IP Addresses/Port Numbers
- Node.js
- Canvasjs Graphing
- HTML
- ESP Wifi Configuration

## References
- [Accelerometer Data Sheet](https://cdn-learn.adafruit.com/assets/assets/000/070/556/original/adxl343.pdf?1549287964)
- [Tilt Sensing for the Accelerometer] (https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing)
- [Thermistor Formula] (https://learn.adafruit.com/thermistor/using-a-thermistor)

-----

