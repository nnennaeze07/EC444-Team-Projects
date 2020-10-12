# Code Readme

This folder contains both html and js code, as well as C code. The C code contains calculations for all of the sensors in their specified units. The js code takes these calculations and uses the serialport module to print these values onto the port, as well as save them to a file called "consoledata.txt". The values are saved onto the text file and are separated by commas so the data can be taken in csv formatting. Finally, the html code in the folder takes in the data (sent from the js file) and graphs these values, with a periodic interval of 3 seconds. 
