# Quest 2 - Tactile Internet
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-10-10
-----

## <em>Summary</em>
In this quest, our team recorded data from sensors, and used nodejs to continously graph the data at regular intervals. To be specific, we worked with three sensors: a thermistor, an infrared rangefinder, and an ultrasonic sensor. These sensors were wired with the esp32, and adc readings were taken to then calculate the data for each sensor in its specific unit. Once the data was calculated, it was sent to a csv file, which quest2.js read and sent to quest2.html for it to be graphed. 

Investigative Question: Tabulate and compare the accuracy and speed of the IR and ultrasonic sensors. Which one would you prefer to use to support driving a robotic car? In order to decide which sensor is more accurate, its application must be thought about. In terms of driving a robotic car, the infrared sensor seems to be much more accurate. Based on our data, as well as research into both sensors, it appears that although neither sensor is perfect, the IR sensor seems to have a lower deviation from measured distance than the ultrasonic sensor, and its more accurate on a range of different materials (for example, if the robotic car is sensing wood material or rubber). 

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Periodic reporting of ultrasonic range in m |  |  1     | 
| Periodic reporting of IR range in m |  |  1     | 
| Period reporting of temperature in C |  |  1     | 
| Results displayed at host as text |  |  1     | 
| Results graphed at host continuously |  |  1     | 
| Demo delivered at scheduled time and report submitted in team folder with all required components | 1* |  1     | 
| Investigative question response |  |  1     | 

*an extension was given to Team 15 and approved by Professor Little for Monday night

### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | ? |  5     | 
| Quality of report.md including use of graphics | 3 |  3     | 
| Quality of code reporting | 3 |  3     | 
| Quality of video presentation | ? |  3     | 


## Solution Design
//Lesbeth can speak about connection with sensors, and collecting data to put into file

The graphing portion of the quest intially proved to be very difficult. We had a lot of trouble figuring out how to read the data. We were able to solve this issue, and used a simple graphs, with multiple lines representing each sensor, as well as multiple y-axes in order to account for the different units of each sensor. 


## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>
//lesbeth can take pics and post them here


## Supporting Artifacts
- [Link to video demo](). Not to exceed 120s


## Modules, Tools, Source Used Including Attribution
ADC

Ultrasonic Sensor

IR Rangefinder

Thermistor

Canvasjs

Nodejs

## References


-----

