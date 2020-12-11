/* Authors: Lesbeth Roque, Nnenna Eze
Quest 6 - Petsitter ™
Date: 12/8/2020
Team 15
*/

// Modules
var express = require('express');
var app = require('express')();
var http = require('http').createServer(app);
var io = require('socket.io')(http);
var dgram = require('dgram');
var Engine = require('tingodb')(); // added this in
var assert = require('assert');
var date = new Date(); // shows current date
var command = "nothing yet"; // no command yet

app.use(express.static('public'));

//Create or open the underlying LevelDB store
var db = new Engine.Db('./', {});
var fs = require("fs");
var collection = db.collection("pet_data");

// Port and IP
var PORT = 3333;
var HOST = '192.168.1.74';

// Create socket
var server = dgram.createSocket('udp4');
var data;

// Create server
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// Initialize functions
//dpData(command);
//Database();
//dropDatabase();

///// Need to talk to Nnenna to figure out what message will be sent
// var splitData =
// {
//   timestamp: [],
//   ultrasonic: [],
//   irRangeFinder: [],
//   ultrasonic2: []
// }


// Function to send and receive data from the ESP32 through UDP
//function udpData(command){

  // get data
  server.on('message',function(message,remote){
    console.log(remote.address + ':' + remote.port +' - ' + message);
    data = message;
    data = data.toString();
    console.log('Data received from server: ' +  data);
    //console.log('Received %d bytes from %s:%d\n',data.length, info.address, info.port);
    // parse data
    var timestamp = date.toLocaleString("en-US", { timeZone: "America/New_York" });
    //var timestamp = new Date().toLocaleString("en-US", { timeZone: "America/New_York" });
    var elements = data.split(','); // comma separated values
    //console.log(timestamp,",",data);
    collection.insert([{timestamp:timestamp,ultrasonic:elements[0],irRangeFinder:elements[1],ultrasonic2:elements[2]}],{w:1}, function(err, result) {
      assert.equal(null, err);
      collection.find().toArray(function(err,item){
        assert.equal(null, err);
        data = item;
    // splitData.timestamp.push(timestamp);
    // splitData.ultrasonic.push(elements[0]);
    // splitData.irRangeFinder.push(elements[1]);
    // splitData.ultrasonic2.push(elements[2]);
    // Database();
    });
  });

  app.get('/pet_data', (req, res) => {
    db.collection('vote').find().toArray((err, result) => {
      if (err) return console.log(err);
      res.send(result);
    });
  });

  // send data
  var message = new Buffer.from(command);
  //server.send('message', 0, message.length, PORT, HOST, function(err,bytes){
  server.send('message', remote.port, remote.address, function(err,bytes){
    if (err) throw err;
    //console.log('UDP message ' + message + ' sent to ' + HOST + ':' + PORT);
    console.log('Sent: Ok!');
  });
});
//}


//Points to index.html to serve webpage
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

// Bind server to port and IP
server.bind(PORT, HOST);

// socket between html and js
io.on('connection',function(socket){
  //io.emit('splitData', splitData);
  io.emit('splitData', data)
  socket.on('reset', function(message){
    //dropDatabase();
  })
  //udpData(message);
});
setInterval(function(){
        io.emit('splitData', data);
    }, 1000); //give it some time

// Button Stuff
// io.on('connection', function(socket){
//   socket.on('reset', function(message){
//     // udpData(message);
//     // dropDatabase();
//     //console.log("database has been " + message);
//   });
// });

// OLD BUTTON STUFF
app.post('/clicked', (req, res) => {
  const click = {clickTime: new Date()};
  console.log(click);
  console.log(db);
  // if reset button is pushed, a new election begins, drop the database and start a new one?
  // https://www.w3schools.com/nodejs/nodejs_mongodb_drop.asp
  // collection.drop(function(err, res) {
  //   if (err) throw err;
  //   if (res) console.log("Collection deleted");
  // db.close();
  // });
  dropDatabase();
});

// Listening on localhost:3000
http.listen(3000, function() {
  console.log('listening on *:3000');
});

// Function to push data to database
// async function Database(){
//   //store formatted data into the Database
//   let promise = new Promise((resolve, reject) => {
//     setTimeout(() => resolve("done!"), 500)
//   });
//
//   let result = await promise;
//
//   // var collection = db.collection("cat_data");
//   collection.insert([{splitData}],{w:1}, function(err, result) {
//     assert.equal(null, err);
//     collection.find().toArray(function(err,item){
//       assert.equal(null, err);
//       data = item;
//     });
//   });
// }

// Function to drop database
function dropDatabase() {
    collection.drop(function(err, res) {
      if (err) throw err;
      if (res) console.log("Collection deleted");
  });
}
