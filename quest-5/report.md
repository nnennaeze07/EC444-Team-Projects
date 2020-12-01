# Quest 5 - Cruise Control
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-12-01
-----

## Summary
In this quest our goal was create a system in which our buggy could use "cruise control" to maintain a constant velocity and avoid collisions. This quest focused on creating a control loop that is able to maintain the buggy's speed, use a LIDAR sensor to prevent collisions, and use UDP to send and receive data from the ESP32 server to a Node.js client in order to start or stop the buggy from an html webpage. An alphanumeric display is also included in the wiring to display the distance being read by the LIDAR sensor, as well as an optical encoder to measure wheel speed through pulse counting. 

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Controls steering to maintain center of course +/- 25cm for entire length | 1 |  1     | 
| Uses PID for speed control holding a fixed speed setpoint after startup and before slowdown [0.1-0.4 m/s] |  |  1     | 
| Stops within 20 cm of end without collision | 1  |  1     | 
| Start and stop instructions issued wirelessly from phone, laptop or ESP) |  |  1     | 
| Measures wheel speed or distance | 1 |  1     | 
| Uses alpha display to show current distance or speed | 1 |  1     | 
| Successfully traverses A-B in one go, no hits or nudges |  |  1     | 


### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution |  |  5     | 
| Quality of report.md including use of graphics |  |  3     | 
| Quality of code reporting |  |  3     | 
| Quality of video presentation |  |  3     | 


## Solution Design
//talk about Lidar and detecting collisions



## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>


## Supporting Artifacts
- [Link to video demo](). Not to exceed 120s


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

-----

