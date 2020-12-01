/* Authors: Lesbeth Roque, Nnenna Eze
Quest 5 - Cruise Control
Date: 12/1/2020
Team 15
*/

// Sources used:
// - https://gist.github.com/aerrity/fd393e5511106420fba0c9602cc05d35 (button working!)

// Modules
var express = require('express');
var app = require('express')();
var http = require('http').createServer(app);
var io = require('socket.io')(http);
var dgram = require('dgram');

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

// On connection, print out received message
server.on('message', function (message, remote) {
    console.log(remote.address + ':' + remote.port +' - ' + message);
    data = message;
    data = data.toString('utf-8'); // make data readable

    io.on('connection', function(socket){
      console.log('a user connected');
      io.emit('data_transmit', data);
      socket.on('disconnect', function(){
        console.log('user disconnected');
      });
    });

    // Send Ok acknowledgement
    server.send("Ok!",remote.port,remote.address,function(error){
      if(error){
        console.log('MEH!');
      }
      else{
        console.log('Sent: Ok!');
      }
    });

});

// Bind server to port and IP
server.bind(PORT, HOST);

//Points to index.html to serve webpage
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

// Button click
app.post('/clicked', (req, res) => {
  const click = {clickTime: new Date()};
  console.log(click);
  //console.log(db);
});


// Listening on localhost:3000
http.listen(3000, function() {
  console.log('listening on *:3000');
});