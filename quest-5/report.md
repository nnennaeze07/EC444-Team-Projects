# Quest 5 - Cruise Control
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-12-01
-----

## Summary
In this quest, our goal was to create a system in which our buggy could use "cruise control" to maintain a constant velocity and avoid collisions. This quest focused on creating a control loop that is able to maintain the buggy's speed, use a LIDAR sensor to prevent collisions, and use UDP to send and receive data from the ESP32 server to a Node.js client in order to start or stop the buggy from an HTML webpage. An alphanumeric display is also included in the wiring to display the distance being read by the LIDAR sensor, as well as an optical encoder to measure wheel speed through pulse counting. 

#### Investigative Question: How would you change your solution if you were asked to provide ‘adaptive’ cruise control?
In order to provide 'adaptive cruise control,' we would add on to our current solution by adding a function that is able to set and detect a distance between our car and another moving object. By detecting the distance, we could also attempt to detect the speed of the object moving in front of the buggy. If the speed of the moving object in front of the buggy is within a certain range of values (say between 0.1-0.4 m/s), we could match the speed of the object in front of it. However, if the speed is too slow (less than 0.1 m/s), then our buggy could try to "change lanes" as long as the wall/objects to the left or right of it are not within 20cm of distance with the buggy.

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Controls steering to maintain center of course +/- 25cm for entire length | 1 |  1     | 
| Uses PID for speed control holding a fixed speed setpoint after startup and before slowdown [0.1-0.4 m/s] | 1 |  1     | 
| Stops within 20 cm of end without collision | 1  |  1     | 
| Start and stop instructions issued wirelessly from phone, laptop or ESP) | 1 |  1     | 
| Measures wheel speed or distance | 1 |  1     | 
| Uses alpha display to show current distance or speed | 1 |  1     | 
| Successfully traverses A-B in one go, no hits or nudges | 1 |  1     | 


### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 3 |  5     | 
| Quality of report.md including use of graphics | 2 |  3     | 
| Quality of code reporting | 2 |  3     | 
| Quality of video presentation | 2 |  3     | 


## Solution Design
There were multiple important components attached to the buggy. Notably, the optical encoder, lidar sensor, and the alphanumeric display. 

The optical encoder is able to use a black and white pattern displayed on a circle to detect pulses. Each pulse is measured and in terms of distance. Using ADC, this distance is read and is divided by time to obtain wheel speed. Once this wheel speed is detected, this information is sent to a control loop (PID) to adjust the set-speed of the buggy.

In order for the ESP to understand data from the lidar and alphanumeric display, I2C is used. The lidar sensor is placed in the front of the buggy and is used to detect the distance between the buggy and a wall/obstruction. Because we used Lidar v3, in order to initialize we had to write to reg 0x00 with the value 0x04, and then, repeatedly read register 0x01 until the least significant bit went low. The alphanumeric display is able to show the distance between the buggy and obstruction ahead. To use two I2C sensors connected to one ESP, we needed to change the address of the alphanumeric display.

Moreover, to get more power, the Lidar v3 sensor and steering servo are both wired up to the H-bridge. The high voltage of the H-bridge comes from the Electronic Speed Control, where this control gives the H-bridge a power of 5V.

Last, UDP was incredibly important in allowing the ESP to send and receive data. This is where the user is able to control the car through node.js from a webserver. In this system, the node.js acts as a client that is connected to the ESP32 server. When the green 'Go' button is pressed, the command "go" is sent to the ESP32 to move forward and when the red 'Stop' button is pressed, the command "stop" is sent to the ESP32 to stop the car.


## Sketches and Photos
<i>Cruise Control System</i>
<p align="left">
<img src="https://github.com/BU-EC444/Team15-Eze-Ganchozo-Roque/blob/master/quest-5/images/Quest5_System.jpg" width=600>
</p>

<i>Meet Patricia, our Buggy :)</i>
<p align="left">
<img src="https://github.com/BU-EC444/Team15-Eze-Ganchozo-Roque/blob/master/quest-5/images/quest5_buggy.jpg" width=600>
</p>

<i>Optical Encoder and Black and White Wheel</i>
<p align="left">
<img src="https://github.com/BU-EC444/Team15-Eze-Ganchozo-Roque/blob/master/quest-5/images/quest5_encoder.jpg" width=600>
</p>

<i>Alphanumeric Display</i>
<p align="left">
<img src="https://github.com/BU-EC444/Team15-Eze-Ganchozo-Roque/blob/master/quest-5/images/quest5_alphanumeric.jpg" width=600>
</p>


## Supporting Artifacts
- [Link to video demo](https://youtu.be/PG7yi2zLw0A).


## Modules, Tools, Source Used Including Attribution
- Node.js
- Socket.io
- Control loops through PID
- Pulse Counting
- Alphanumeric Display
- Optical Encoder
- LIDAR v3
- I2C buses and connections

## References
- [Button on HTML](https://gist.github.com/aerrity/fd393e5511106420fba0c9602cc05d35)
- [Button formatting](https://www.w3schools.com/howto/howto_css_block_buttons.asp)
- [Pulse Counting](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/pcnt.html)
- [Optical Encoder Wiring](https://learn.sparkfun.com/tutorials/qrd1114-optical-detector-hookup-guide#example-circuit)
- [Udp Example](https://www.geeksforgeeks.org/udp-server-client-implementation-c/)
- [Lidar Datasheet](http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf)
- [Adaptive Cruise Control](https://www.extremetech.com/extreme/157172-what-is-adaptive-cruise-control-and-how-does-it-work#:~:text=To%20use%20adaptive%20cruise%20control,1%20or%205%20mph%20increments.)

-----

