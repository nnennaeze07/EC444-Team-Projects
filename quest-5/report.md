# Quest 5 - Cruise Control
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-12-01
-----

## Summary
In this quest our goal was create a system in which our crawler could use "cruise control" to maintain a constant velocity and avoid collisions. This quest focused on creating a control loop that is able to maintain the crawler's speed, use a sensor to prevent collisions, and use UDP to send and receive data from the ESP32 server to a Node.js client in order to start or stop the crawler from an html webpage.

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



## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>


## Supporting Artifacts
- [Link to video demo](). Not to exceed 120s


## Modules, Tools, Source Used Including Attribution
- Node.js
- Socket.io
- Control loops through PID
- 

## References
- [Button on HTML](https://gist.github.com/aerrity/fd393e5511106420fba0c9602cc05d35)
- [Button formatting](https://www.w3schools.com/howto/howto_css_block_buttons.asp)

-----

