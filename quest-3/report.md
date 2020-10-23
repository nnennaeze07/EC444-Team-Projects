# Hurricane Box with Remote Access
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-10-23
-----

## Summary
This quest combined skills 19 to 24 to read data from two sensors connected to the ESP32. This data (temperature for the thermistor sensor, and pitch and roll for the accelerometer) was graphed on a remote client. The graph is updated continuously every 3 seconds. There was also an LED connected to the ESP32, which could be turned on or off from the remote web client. Finally, the raspberry pi zero camera was connected, to record that the LED was in fact toggling when the user requested it to do so. This was done through the wifi access of a loaner router, provided by Professor Little. 

#### Investigative Question Reponse: What are steps you can take to make your device and system low power? Please cite sources for you answer.

Using a WLAN (wireless local area network) technology, as opposed to WPAN (wireless personal area network) technology, is higher in power consumption. To make the device lower power, bluetooth or card readers could be used to communicate between the device and its server. Moreover, turning off the servers screen when not being viewed could decrease battery consumption.


## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Measures acceleration and temperature | 1 |  1     | 
| Displays real-time data at remote client via portal using separate IP network | 1 |  1     | 
| Controls LED on box from remote client via portal |  |  1     | 
| Sources web cam video into remote client| 1 |  1     | 
| ESP32 and Rpi are connected wirelessly to (or as) router; ESP32 sensor data are delivered to local node server (on local laptop or Rpi) | 1 |  1     | 
| Demo delivered at scheduled time and report submitted in team folder with all required components | 1 |  1     | 
| Investigative question response| 1 |  1     | 



### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 3 |  5     | 
| Quality of report.md including use of graphics | 2 |  3     | 
| Quality of code reporting | 3 |  3     | 
| Quality of video presentation | 2 |  3     | 


## Solution Design
In order to complete this quest we took the list of requirements and took it step by step. First, we combined the ideas from skill 13 and skill 23 to connect both the thermistor and accelerometer to the ESP32 and print out its tilt and temperature, respectively. Then, we used Node.js to graph the sensor's data periodically. Next, we configured the ESP32 to connect to WiFi. After, we used the ESP32 as a UDP client, we transferred data from the the server in order to turn an LED on or off once a button was pressed. Last we created a webcam to indicate the LED in realtime.

When working with this quest, our team ran into a couple of issues in configuring the UPD Client to control the LED and DDNS. With the UDP Client, we mapped out the steps of UPD protocol. We learned that we needed to establish a connection with the ESP and Node server, and that this communication occured in the same network behind a router. A UDP server was added as a function the the Node server and the ESP32 (client) periodically opens a connection to the server to send some information. Once the server receives this information a confirmation is send back to the client. This two-way communication proved useful in controlling the LED. For the DDNS, we initially had a hard time getting the data from the nodejs server to go to port 3333, and read from the given hostname. However, this issue was solved when connecting the laptop to the same wifi router as the esp32 and raspberry pi. 


## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>
<img src="https://user-images.githubusercontent.com/44929220/97051482-43420f80-154d-11eb-804b-a98759a8305c.jpg" width="250" height="250" />

<img src="https://user-images.githubusercontent.com/44929220/97051495-489f5a00-154d-11eb-9479-bfc8aec7bfa9.jpg" width="250" height="250" />

<img src="https://user-images.githubusercontent.com/44929220/97051500-4b01b400-154d-11eb-8f2b-182b7ef18d34.jpg" width="250" height="250"/>

<img src="https://user-images.githubusercontent.com/44929220/97051506-4d640e00-154d-11eb-81b4-2a971f43c6d0.jpg" width="250" height="250"/>

<img src="https://user-images.githubusercontent.com/44929220/97051510-4fc66800-154d-11eb-803f-cb99a1bfc38b.jpg" width="250" height="250"/>

<img src="https://user-images.githubusercontent.com/44929220/97051517-52c15880-154d-11eb-9f8f-8b149877b852.jpg" width="250" height="250"/>

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
- [Tilt Sensing for the Accelerometer](https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing)
- [Thermistor Formula](https://learn.adafruit.com/thermistor/using-a-thermistor)
- [LED Button](https://www.w3docs.com/snippets/html/how-to-make-button-onclick-in-html.html)
- [UDP Protocol with ESP and LEDs](https://www.allen.dj/esp8266-nodemcu-button-slider-remote-node-js-server/)

-----

