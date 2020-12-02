# Code Readme

In this folder we present 3 programs that work together in this system. In "     ".c, we use a LIDAR V3 that acts as a sensor to detect collisions, encoder to detect wheel speed, a control loop (PID) to keep the buggy driving at a constant speed, i2c code for the alphanumeric display to show distance, and UDP to communicate between the server and client. For the lidar sensor, conditional staments are used to stop the buggy if it detects a wall or obstruction ahead. If the distance between the buggy and obstruction is less than 20cm, the buggy automatically stops. The optical encoder is used to detect wheel speed by dividing distance over time. The PID uses this speed information, timer interrupts, and pulse control to maintain the buggy driving at a constant speed. Moreover, the alphanumeric display distance is shown and is updated every 2s seconds. Last, UDP sets the ESP as the server and the Node.js client. This is done through wifi and using the IP address of the ESP.

In index.js, we are able to use socket.io to read and send data from the ESP server to the Node.js client. The receiveData() function ensures data is sent and received by printing out a message on the console.

In index.html, a local host webpage displays two buttons, a go and stop button. Once these buttons are clicked, an event click is able to point to the index.js file to communicate with the buggy.

#### Sources
- [Button on HTML](https://gist.github.com/aerrity/fd393e5511106420fba0c9602cc05d35)
- [Button formatting](https://www.w3schools.com/howto/howto_css_block_buttons.asp)
- [Pulse Counting](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/pcnt.html)
- [Udp Example](https://www.geeksforgeeks.org/udp-server-client-implementation-c/)
- [Lidar Datasheet](http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf)
