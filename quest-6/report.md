# Quest 6: Pet Babysitter
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-12-08
-----

## Summary
In this self-defined quest, we use 3 sensors, 2 actuators, 1 camera connected to a raspberry pi, and multiple esps that communicate through UDP to send information that is displayed on a webpage. The camera feed is used to watch a cat (or any pet) while its owner is not home. Sensors are placed near the cat bed, cat food tin, and cat litter box, so that when the cat the appraoches these areas, the owners knows that it is napping, eating, or using the bathroom, respectively. The specific sensors used in this quest are the IR rangefinder, the ultrasonic sensor, and the HC-SR04 ultrasonic sensor. A timer is used to keep track of the cat's feeding time, and this time is displayed on the alphanumeric display. When it is time for the cat to eat, the alphanumeric displays will show "FOOD", and then a servo will rotate, shaking food into the cat's food tin. The timer will not start again until the pet has begun eating, which is determined by measuring the distance of the IR sensor. Each sensor is connected to a different ESP, and data from the sensor is sent through UDP to a main webpage, where the cat message is determined ("Cat is napping", "Cat is eating"). This webpage will also show the camera feed of the cat.

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| At least 3 working sensors | 1 |  1     | 
| Timer that counts down next feeding time | 1 |  1     | 
| Alphanumeric display shows time until next feeding | 1 |  1     | 
| Servo successfully shakes so that food is released | 1 |  1     | 
| Each esp sends a message to a Node js server | 1 |  1     | 
| Webpage that displays table of messages from each sensor | 1 |  1     | 
| A camera feed is displayed on the webpage | 1 |  1    | 



### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 4 |  5     | 
| Quality of report.md including use of graphics | 3 |  3     | 
| Quality of code reporting | 3 |  3     | 
| Quality of video presentation | 3 |  3     | 


## Solution Design
One of the more complex components of this project was the feeding aspect. A timer is used to count down, and when the timer ends, it is paused by the timer event. A flag is then set to signify that the servo should go off. After the servo finishes rotating, the program checks if the pet has begun eating the food by measuring the distance of the IR rangefinder. If this distance is below 30, the pet is assumed to be eating, and the timer is started again in the ir_sensor task. The timer begins again, and the countdown is shown on the alphanumeric display. 

One unique solution design for this quest involved connecting multiple udp clients to one udp server. The esp that is connected to the IR rangefinder acts as a server, and the other two esps send data from the ultrasonic sensors. The server esp then combines the data of all sensors into one long string, and sends this data altogether to the Nodejs server. In order to do this, the esp server needed to figure out which IP address was sending data from each ultrasonic sensor. This was done using an if statement in the udp server task. Since we know the IP address of each esp, we were able to check a variable that figures out and stores theIP address of the esp that sent the data. This variable is compared to each esp IP address, and depending on which esp the data came from, we were able to figure out whether the data corresponded to the first or second ultrasonic sensor. 

Moreover, the node.js server receives a data string with the distances of the ultrasonic sensor, ir range finder, and ultrasonic 2 sensor, respectively. This data string is parsed in order to gather the distances from the string. These distances are then stored in a tingodb database, along with the timestamp (date and time in EST) of the measured distances. Using socket.io, we were able to show these distances in a table format in localhost webpage. This can help the pet owner reference when their pet last had a meal, used the bathroom, or took a nap. And last, using using an image reference on the html file for the rasbperry pi camera, we are able to display a livestream of the pet in action. 

## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>

<img src = "https://user-images.githubusercontent.com/44929220/101855012-b967f900-3b30-11eb-910f-bb037efdfddb.png" height="300" width="300">

<img src = "https://user-images.githubusercontent.com/44929220/101860469-e1a92500-3b3b-11eb-9a77-fc6a3355b96f.jpg" height="300" width="450">
<img src = "https://user-images.githubusercontent.com/44929220/101860477-e2da5200-3b3b-11eb-8de3-137d195c09a4.jpg" height="300" width="450">


## Supporting Artifacts
- [Link to video demo](https://youtu.be/SQtA3hluXjU). Not to exceed 120s


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
[Timer Info](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/timer.html#_CPPv411timer_start13timer_group_t11timer_idx_t)

[Ultrasonic Sensor Datasheet](https://esp32.com/viewtopic.php?t=5787)

[Pi Webcam](https://pimylifeup.com/raspberry-pi-webcam-server/)

[Database](http://whizzer.bu.edu/briefs/design-patterns/dp-db)

-----

