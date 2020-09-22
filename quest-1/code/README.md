# Code Readme

### Collaborators: Nnenna Eze, Lesbeth Roque

### Date: 09/22/2020

The code for Quest 1 utilized skills learned from the individual assignments we completed during the week. When built and run, the Quest 1 code starts a countdown timer from 1 minute and 5 seconds. When the timer reaches 0, the connected servo motor rotates three times between left and right, to portray the fish feeding task. After the servo has completed its 3 rotations, the timer starts again at the intial time (1 minute and 5 seconds), and the process repeats itself. The current time of the timer is shown on the Alphanumeric Display, and is updated every second. Sources of code were taken from examples in the espressif/esp-idf github, as well as examples from the official EC444 website. 

Reference code can be found here: https://github.com/espressif/esp-idf/tree/17ac4bad7381e579e5a7775755cc25480da47d97/examples/peripherals/timer_group

Reference code can be found here: https://github.com/espressif/esp-idf/tree/master/examples/peripherals/mcpwm/mcpwm_servo_control
