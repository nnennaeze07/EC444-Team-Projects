# Quest 6: Cat Babysitter (Rename dog so Myles can make an appearance)
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-12-08
-----

## Summary
In this self-defined quest, we use 3 sensors, 2 actuators, 1 camera connected to a raspberry pi, and multiple esps that communicate through UDP to send information that is displayed on a webpage. The camera feed is used to watch the cat while its owner is not home. Sensors are placed near the cat bed, cat food tin, and cat litter box, s that when the cat the appraoches these areas, the owners knows that it is napping, eating, or using the bathroom, respectively. A timer is used to keep track of the cat's feeding time, and this time is displayed on the alphanumeric display. When it is time for the cat to eat, the alphanumeric displays will show "FOOD", and then a servo will rotate, shaking food into the cat's food tin. Each sensor is connected to a different ESP, and information from the sensors regarding what the cat is doing is sent as a message through UDP to a main webpage. this webpage will also show the camera feed of the cat.

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| At least 3 working sensors |  |  1     | 
| Timer that counts down next feeding time |  |  1     | 
| Alphanumeric display shows time until next feeding |  |  1     | 
| Servo successfully shakes so that food is released |  |  1     | 
| Each esp sends a message to a Node js server |  |  1     | 
| Webpage that displays table of messages from each sensor |  |  1     | 
| A camera feed is displayed on the webpage |  |  1     | 



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
Alphanumeric Display

UDP Protocol

ESP Wifi

Timer

Ultrasonic Sensor

Infrared Sensor

Nodejs/ HTML

Servo

Raspberry Pi Camera

## References

-----

