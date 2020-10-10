var express = require('express');
var app = express();
var path = require('path');
var fs = require('fs');
var csv = require("csv-parse");

//jQuery timeout function
//window.setinterval
// viewed at http://localhost:8080
app.get('/', function(req, res) {
  res.sendFile(path.join(__dirname + '/quest2.html'));
  console.log('Data been got');
});

// request data at http://localhost:8080/data or just "/data"
app.get('/data', function(req, res) {
  var data = [];  // Array to hold all csv data
  fs.createReadStream('stocks.csv')
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
