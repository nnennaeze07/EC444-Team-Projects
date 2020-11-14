/* Authors: Lesbeth Roque, Nnenna Eze
Skill 26 - Database (Team Skill)
Date: 11/3/2020
Team 15
*/

// Sources used:
// - http://www.tingodb.com/
// - https://github.com/sergeyksv/tingodb
// - http://whizzer.bu.edu/briefs/design-patterns/dp-db
// - https://www.toptal.com/software/definitive-guide-to-datetime-manipulation#:~:text=Getting%20the%20Current%20Timestamp,passed%20since%20January%201%2C%201970. (timestamp)
    //number of milliseconds that have passed from January 1, 1970 UTC
// - https://gist.github.com/aerrity/fd393e5511106420fba0c9602cc05d35 (button working!)

// Modules
var express = require('express');
var app = require('express')();
var http = require('http').createServer(app);
var io = require('socket.io')(http); //this didn't work for me so I put in its path
var Engine = require('tingodb')(); // added this in
var assert = require('assert');
var date = new Date(); // shows current date

//Create or open the underlying LevelDB store
var db = new Engine.Db('./', {});
var fs = require("fs");

// Required module
var dgram = require('dgram');

// Port and IP
var PORT = 3333;
var HOST = '192.168.1.74';

// Create socket
var server = dgram.createSocket('udp4');
var votedata;

// Create server
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// On connection, print out received message
server.on('message', function (message, remote) {
    console.log(remote.address + ':' + remote.port +' - ' + message);
    votedata = message;
    votedata = votedata.toString('utf-8'); // make data readable
    //console.log(votedata);

    //// not ideal
    var collection = db.collection("vote_data"); // place into database

    //for (i = 1; i < votedata.length; i++){
    var timestamp = Date.now();
    var elements = votedata.split(','); // separated by comma
    console.log(timestamp,",",votedata);
    //console.log("element 0- " + elements[0]);
    //console.log("element 1- " + elements[1]);
    collection.insert([{timestamp:timestamp,ID:elements[0],vote:elements[1]}],{w:1}, function(err, result) {
      assert.equal(null, err);
      collection.find().toArray(function(err,item){
        assert.equal(null, err);
        data = item;
        //console.log("data is: " + JSON.stringify(data));
        //db.close();
      });
    });
    //};

    // collection.drop(function(err, res) {
    //   if (err) throw err;
    //   if (res) console.log("Collection deleted");
    // //db.close();
    // });

    //query to count votes for R -- can replace with Green and Blue
    //(List of candidates with their vote counts)
    var query = {vote:'G'};
    collection.find(query).toArray(function(err, result){
      if (err) throw err;
      console.log(result);
      collection.find(query).count(function(err,countData){
        console.log(countData + " vote count(s) for G");
        //db.close();
      });
    });


    //query to list results on console (To list individual votes by ID and time)
    // collection.find({}).toArray(function(err, result){
    //   if (err) throw err;
    //   console.log(result);
    // });

    //get the vote data from the database
    app.get('/vote_data', (req, res) => {
      db.collection('vote').find().toArray((err, result) => {
        if (err) return console.log(err);
        res.send(result);
      });
    });

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

// function receives message and places into database
// function collection() {
//   //
// }
//var text = fs.readFileSync("vote.txt").toString('utf-8'); // convert text buffer to string we can read
//var textByLine = text.split("\n")
//
// // place in function and add data query, clear database before insert
// // function collection(){
// var collection = db.collection("vote_data"); // place into database
//for (i = 1; i < votedata.length; i++){
  //console.log("vote data out " + votedata);
  // var timestamp = Date.now();
  // var elements = votedata.split(','); // t is tab
  // console.log(timestamp,",",votedata);
  // collection.insert([{timestamp:timestamp,ID:parseFloat(elements[0]),vote:elements[1]}],{w:1}, function(err, result) {
  //   assert.equal(null, err);
  //   collection.find().toArray(function(err,item){
  //     assert.equal(null, err);
  //     data = item;
  //     //db.close();
  //   });
  // });
//};

// collection.drop(function(err, res) {
//   if (err) throw err;
//   if (res) console.log("Collection deleted");
// //db.close();
// });

// query to count votes for R -- can replace with Green and Blue
// (List of candidates with their vote counts)
// var query = {vote:'G'};
// collection.find(query).toArray(function(err, result){
//   if (err) throw err;
//   console.log(result);
//   collection.find(query).count(function(err,countData){
//     console.log(countData + " vote count(s) for G");
//     //db.close();
//   });
// });


// query to list results on console (To list individual votes by ID and time)
// collection.find({}).toArray(function(err, result){
//   if (err) throw err;
//   console.log(result);
// });


//Points to index.html to serve webpage
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});
//
// // add a document to the DB collection recording the click event
app.post('/clicked', (req, res) => {
  const click = {clickTime: new Date()};
  console.log(click);
  console.log(db);
  // if reset button is pushed, a new election begins, drop the database and start a new one?
  // https://www.w3schools.com/nodejs/nodejs_mongodb_drop.asp
  collection.drop(function(err, res) {
    if (err) throw err;
    if (res) console.log("Collection deleted");
  db.close();
  });
});
//
// // get the vote data from the database
// // app.get('/vote_data', (req, res) => {
// //   db.collection('vote').find().toArray((err, result) => {
// //     if (err) return console.log(err);
// //     res.send(result);
// //   });
// // });
//
// io.on('connection', function(socket){
//   console.log('a user connected');
//   io.emit('data_transmit', data);
//   socket.on('disconnect', function(){
//     console.log('user disconnected');
//   });
// });
//
// Listening on localhost:3000
http.listen(3000, function() {
  console.log('listening on *:3000');
});







////////////////////////////////////////
// const udp = require('dgram');
// const conf = require('config');

// UDP_IP = "192.168.1.74"
// UDP_PORT = 3333
// // const conf = require('./config/config')
// //config = require('../config/config');
//

//
// // const {
// //     log
// // } = require('./util/loggerTool')
//
// // creating a udp server
// const server = udp.createSocket('udp4')
//
// // emits when any error occurs
// server.on('error', (error) => {
//     console.log("udp_server", "error", error)
//     server.close()
// })
//
// // emits on new datagram msg
// server.on('message', (msg,info) => {
//     console.log("udp_server", "info", msg.toString() + ` | Received ${msg.length} bytes from ${info.address}:${info.port}`)
//
//     let timestp = new Date()
//     const response = {
//         description: 'UDP PORT TEST BY RMS Math',
//         serverPort: conf.port,
//         timestamp: timestp.toJSON(),
//         received: {
//             message: msg.toString(),
//             fromIP: info.address,
//             fromPort: info.port
//         }
//     }
//     const data = Buffer.from(JSON.stringify(response))
//   })
//
//     //sending msg
//     client.send(data, UDP_PORT, UDP_IP, (error, bytes) => {
//         if(error){
//             console.log(error)
//             client.close()
//         } else {
//             console.log('Data sent !!!')
//         }
//     })
// })  // end server.on
//
//
// //emits when socket is ready and listening for datagram msgs
// server.on('listening', () => {
//     const address = server.address()
//     const port = address.port
//     const family = address.family
//     const ipaddr = address.address
//
//     console.log("udp_server", "info", 'Server is listening at port ' + port)
//     console.log("udp_server", "info", 'Server ip :' + ipaddr)
//     console.log("udp_server", "info", 'Server is IP4/IP6 : ' + family)
// })
//
// //emits after the socket is closed using socket.close()
// server.on('close', () => {
//     console.log("udp_server", "info", 'Socket is closed !')
// })
//
// server.bind(conf.port)
