# Quest 1 - Fish Feeder
Authors: Nnenna Eze, Patricia Ganchozo, Lesbeth Roque

Date: 2020-09-22
-----

## <em>Summary</em>
In this quest, our team created a system that would feed fish at regular intervals. In order to do so, we used a servo to shake the fish food back and forth from a canister and created a program that would keep track of the time elapsed and display the time until the next feeding on the alphanumeric display.

Currently, there is a place in the code where the timer interval is manually set. However, this can be made more dynamic by allowing the user to input any time, and storing that value in some global variable. This variable would be integrated into the timer task function, and easily used through the rest of the code. It would be much simpler to have the user enter the time interval in minute-second format, but if it were accepted in seconds there is a function in the code that converts seconds into m-s format. 

## <em>Self-Assessment</em>

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Servo spins right then left three times without chatter at prescribed time intervals |1  |  1     | 
| Alphanumeric display indicates hours and minutes  | 1 |  1     | 
| Display shows countdown time report every second with no time loss.  | 1 |  1     | 
| Food dispensed at specified times.   | 1 |  1     | 
| Demo delivered at scheduled time and report submitted in team folder with all required components | 1 |  1     | 
| N/A | - |  -     | 
| Investigative question response | 1 |  1     | 


### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 4 |  5     | 
| Quality of report.md including use of graphics | 3 |  3     | 
| Quality of code reporting | 2 |  3     | 
| Quality of video presentation | 3 |  3     | 


## <em>Solution Design</em>
Initially, there were several issues with the servo working together with the timer, so instead of defining them as two different tasks, the servo functionality was included in the timer event function. The timer has a length of 1 second, so when it ends, the time is stored in a variable that is used in the alphanumeric display task to display the current time. Then, the timer automatically reloads and begins again. When the variable which keeps tracks of time reaches 0, then the servo function is implemented, and the timer pauses until the servo is finished moving. Then, the current time variable is reset and the timer starts again from its initial value.

Another obstacle we faced was getting the servo to rotate the correct number of times, and rotate quicker. We learned that by incrementing a count variable in the for loop by more than one (i.e count +=3), the servo would rotate faster. The code also includes a while loop that counts the number of times the servo rotates. 


## <em>Sketches and Photos</em>
<center><img src="https://github.com/BU-EC444/Team15-Eze-Ganchozo-Roque/blob/master/quest-1/images/quest1_image1.jpeg" width="45%" /></center>  
<center> </center> 
<p><em> Circuitry set up. Use of GPIO Pins GND, 3V, SCL, SDA, MO. </em></p>

<center><img src="https://github.com/BU-EC444/Team15-Eze-Ganchozo-Roque/blob/master/quest-1/images/quest1_image2.jpg" width="45%" /></center>  
<center> </center>
<p><em> Minutes and seconds display</em></p>


## <em>Supporting Artifacts</em>
- [Link to video demo](https://youtu.be/vahbK6kVZsM).


## <em>Modules, Tools, Source Used Including Attribution</em> 
- GPIO
- Servo PWM (Pulse Width Modulation)

## <em>References</em>
- [Github repository for timer](https://github.com/espressif/esp-idf/tree/17ac4bad7381e579e5a7775755cc25480da47d97/examples/peripherals/timer_group)
- [Github repository for servo control](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/mcpwm/mcpwm_servo_control)

-----

