/*Quest 2 (Team 15)
Authors: Nnenna Eze, Lesbeth Roque
Date: 10/10/2020*/
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const port = new SerialPort('/dev/cu.SLAB_USBtoUART', { baudRate: 115200}) //Serial port event
const parser = new Readline()

var express = require('express');
var app = express();
var path = require('path');
var fs = require('fs');
var csv = require("csv-parse");

var count = 0;
port.pipe(parser) // this reads from the serial port

console.log('Data: Temperature in C, Ultrasonic distance(m), IR distance(m)!')
parser.on('data',function(data) {
	line => console.log(`>${line}`)
	port.write('ROBOT POWER ON\n')

	if (count > 0) {
	//var datasplit = data.split(",")  //divide file by comma
	//console.log("Temperature in C: ", datasplit[1], "\tUltrasonic distance: ", datasplit[2], "\tIR distance" , datasplit[3]);

	parser.on('data',line => console.log(`>${line}`)) // print data on console

	fs.appendFile('consoledata.txt', data, function(err){ //create a file named consoledata.txt:
	if (err) throw err;
	// console.log('Data: Temperature in C, Ultrasonic distance(m), IR distance(m)!')
	});
	}
	count++; //increase count variable
});

//jQuery timeout function
//window.setinterval
// viewed at http://localhost:8080
app.get('/', function(req, res) {
  res.sendFile(path.join(__dirname + '/quest2.html'));
});

// request data at http://localhost:8080/data or just "/data"
app.get('/data', function(req, res) {
  var data = [];  // Array to hold all csv data
  fs.createReadStream('consoledata.txt')
  .pipe(csv())
  .on('data', (row) => {
    console.log(row);
    data.push(row);  // Add row of data to array
  })
  .on('end', () => {
    console.log('CSV file successfully processed');
    res.send(data);  // Send array of data back to requestor
  });
});
console.log('Request Complete?');


app.listen(8080);
