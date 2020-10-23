/*Quest 3 (Team 15)
Authors: Nnenna Eze, Lesbeth Roque
Date: 10/23/2020*/
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const port = new SerialPort('COM10', { baudRate: 115200}) //Serial port event
const parser = new Readline()

var express = require('express');
var app = express();
var path = require('path');
var fs = require('fs');
var csv = require("csv-parse");
var dgram = require('dgram');

//Port and IP
var PORT = 3333;
var HOST = '168.122.152.6';

//var server = dgram.createSocket('udp4');
var count = 0;

// server.on('listening',function(){
// 	var address = server.address();
// 	console.log('UDP Server listening on ' + address.address + ":" + address.port);
// });
//
// server.on('message', function(message,remote){
// 	console.log(remote.address + ':'+ remote.port + '-' + message);
//
// 	//send acknowledgement
// 	server.send("Ok!", remote.port, remote.address, function(error){
// 		if(error){
// 			consoe.log('Error!');
// 		}
// 		else{
// 			console.log('Send: Ok!');
// 		}
// 	});
// });
// //bind server to port and IP
// server.bind(PORT, HOST);

port.pipe(parser) // this reads from the serial port

console.log('Data: Roll, Pitch, Temperature (C)!')
parser.on('data',function(data) {
	line => console.log(`>${line}`)
	port.write('ROBOT POWER ON\n')

	if (count > 0) {
	//var datasplit = data.split(",")  //divide file by comma
	//console.log("Temperature in C: ", datasplit[1], "\tUltrasonic distance: ", datasplit[2], "\tIR distance" , datasplit[3]);

	parser.on('data',line => console.log(`>${line}`)) // print data on console

	fs.writeFile('quest3data.txt', data, function(err){ //create a file named consoledata.txt:
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
  res.sendFile(path.join(__dirname + '/quest3.html'));
});

// request data at http://localhost:8080/data or just "/data"
app.get('/data', function(req, res) {
  var data = [];  // Array to hold all csv data
  fs.createReadStream('quest3data.txt')
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
console.log('Request Complete');


app.listen(3333); //change to 3333
