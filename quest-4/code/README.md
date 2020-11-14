# Code Readme

In this folder we present 2 different programs that work together in this system. In the udp_client.c code, we create a system that is able to use the fobs to vote and send data through UDP. Each fob was given a unique ID based on the last number of their IP address. The initial fob acts as the voter and votes a color with the press of a button. In this button task we were able to switch through button states and send this data to a second fob. This second fob then acts as an IR receiver and UDP client. Then this data is sent along to a another fob, the poll leader, which acts as UDP client and server. This is due to the fact it is able to recieve data from the initial and second fob, and is able to send this data to the node.js UDP server. This data is printed out on the console to ensure data is being sent and received through UDP.

In the index.js, we are able to use socket.io to read in the data buffer sent from the poll leader. This data is gathered and parsed to be placed in a database. Using collection.insert, we are able to insert specific data, such as the timestamp, ID, vote into the database. Moreover, a query to tell the user how many votes are counted for a particular candidate is conducted. 

In the index.html, a local host webpage displays a table. This table is used to show the collection data was inserted database. Additonally, the webpage has a button that once clicked in the webpage (html), is able to go to the index.js file to reset the vote count (through collection.drop). 

Sources
- [Election Algorithm](https://www.geeksforgeeks.org/election-algorithm-and-distributed-processing/#:~:text=The%20Bully%20Algorithm%20%E2%80%93,assumed%20that%20coordinator%20has%20failed) 
- [UDP Server/Client Implementation](https://www.geeksforgeeks.org/udp-server-client-implementation-c/)
- [Tingodb](http://www.tingodb.com/)
- [Design Patterns](http://whizzer.bu.edu/briefs/design-patterns/dp-db)
- [Timestamp](https://www.toptal.com/software/definitive-guide-to-datetime-manipulation#:~:text=Getting%20the%20Current%20Timestamp,passed%20since%20January%201%2C%201970)
- [Button](https://gist.github.com/aerrity/fd393e5511106420fba0c9602cc05d35)
