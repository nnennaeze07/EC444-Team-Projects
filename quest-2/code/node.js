/* Authors: Nnenna Eze, Lesbeth Roque
Quest 2: Tactile Internet
Team 15
10-07-20 */

// api-serialport; SOURCE: https://github.com/serialport/website/blob/master/docs/api-serialport.md
// api-parser-readline source: https://serialport.io/docs/api-parser-readline

const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const port = new SerialPort('/dev/cu.SLAB_USBtoUART', { baudRate: 115200}) //Serial port event
const parser = new Readline()
var fs = require('fs')
var count = 0;
port.pipe(parser) // this reads from the serial port
//parser.on('data',line => console.log(`>${line}`))
// port.write('ROBOT POWER ON\n')


console.log('Data: Temperature in C, Ultrasonic distance(m), IR distance(m)!')
parser.on('data',function(data) {
	line => console.log(`>${line}`)
	port.write('ROBOT POWER ON\n')

	if (count > 0) {
	//var datasplit = data.split(",")  //divide file by comma
	//console.log("Temperature in C: ", datasplit[1], "\tUltrasonic distance: ", datasplit[2], "\tIR distance" , datasplit[3]);

	parser.on('data',line => console.log(`>${line}`)) // print data on console

	fs.writeFile('consoledata.txt', data, function(err){ //create a file named consoledata.txt:
	if (err) throw err;
	// console.log('Data: Temperature in C, Ultrasonic distance(m), IR distance(m)!')
	});
	}
	count++; //increase count variable
});
