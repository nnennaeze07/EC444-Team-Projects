# Quest 2 - Tactile Internet
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-10-10
-----

## <em>Summary</em>
In this quest, our team recorded data from sensors, and used nodejs to continously graph the data at regular intervals. To be specific, we worked with three sensors: a thermistor, an infrared rangefinder, and an ultrasonic sensor. These sensors were wired with the esp32, and adc readings were taken to then calculate the data for each sensor in its specific unit. Once the data was calculated, it was sent to a csv file, which quest2.js read and sent to quest2.html for it to be graphed. 

Investigative Question: In order to decide which sensor is more accurate, its application must be thought about. In terms of driving a robotic car, the infrared sensor seems to be much more accurate. Based on our data, as well as research into both sensors, it appears that although neither sensor is perfect, the IR sensor seems to have a lower deviation from measured distance than the ultrasonic sensor, and its more accurate on a range of different materials (for example, if the robotic car is sensing wood material or rubber). 

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Periodic reporting of ultrasonic range in m |  1|  1     | 
| Periodic reporting of IR range in m | 1 |  1     | 
| Period reporting of temperature in C | 1 |  1     | 
| Results displayed at host as text | 1 |  1     | 
| Results graphed at host continuously | 1 |  1     | 
| Demo delivered at scheduled time and report submitted in team folder with all required components | 1* |  1     | 
| Investigative question response | 1 |  1     | 

*an extension was given to Team 15 and approved by Professor Little for Monday night

### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 4 |  5     | 
| Quality of report.md including use of graphics | 3 |  3     | 
| Quality of code reporting | 3 |  3     | 
| Quality of video presentation | 2 |  3     | 


## Solution Design
In order to connect the sensors, we built off our wiring designs in skill 13 (thermistor), skill 14 (ultrasonic sensor), and skill 15 (IR rangefinder). In all of these skills, we used GPIO34 (Pin A2) on the ESP32. However, in order to wire multiple sensors and collect its data, we needed to connect each sensor to its own pin and obtain ADC reading separately. Thus, we connected the thermistor to pin A2, the ultrasonic sensor to pin A3, and the IR rangefinder to pin A4 (GPIO34, GPIO39, and GPIO36, respectively). We used these analog inputs due to the fact they use ADC#1 and were non-output capable pins. Then, we calculated the needed temperature and distances in engineering units from the formulas programmed in the aformentioned skills. 

Moreover, to collect data and graph it into our localhost, we programmed a javascript file to collect data from the serial port, place the collected data into a textfile, and converted the textfile into a csv file for the graphing portion.

The graphing portion of the quest initially proved to be very difficult. We had a lot of trouble figuring out how to read the data. We were able to solve this issue, and used a simple graph, with multiple lines representing each sensor, as well as multiple y-axes in order to account for the different units of each sensor. 


## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>

<img src="https://user-images.githubusercontent.com/44929220/95805116-a4a6ea80-0cd2-11eb-874c-921cc0483238.jpg" height="250" width="650">

<img src="https://user-images.githubusercontent.com/44929220/95805098-9bb61900-0cd2-11eb-873f-aebd69e1af99.jpg" height="250" width="650">


## Supporting Artifacts
- [Link to video demo](https://youtu.be/2GZDOt8i8uY). 


## Modules, Tools, Source Used Including Attribution
- ADC
- Ultrasonic Sensor
- IR Rangefinder
- Thermistor
- Canvasjs
- Nodejs

## References
https://iopscience.iop.org/article/10.1088/1757-899X/149/1/012141/pdf --> to help answer investigative question

https://canvasjs.com/forums/topic/multiple-chart-with-data-from-csv-file/

https://canvasjs.com/forums/topic/jquery-on-local-host-not-working-want-to-read-local-csv-file/

https://github.com/BU-EC444/code-examples/blob/master/DataLoadingTest/index.html

https://canvasjs.com/docs/charts/how-to/create-charts-from-csv/

https://github.com/serialport/website/blob/master/docs/api-serialport.md

https://github.com/espressif/esp-idf/blob/8bc19ba/components/soc/soc/esp32/include/soc/adc_channel.h

-----

