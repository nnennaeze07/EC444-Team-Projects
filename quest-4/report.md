# Quest 4 - Electronic Voting
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-11-12
-----

## Summary
In this quest, we were able to use the skills from this cluster and from pasts clusters to create an electronic voting system. Notably, we used IR-TX/RX to communicate between fobs, UDP clients and servers, databases, and node.js. The system starts with one fob voting on a color (red, green, blue). Upon pushing a button, the fob ID and its vote is sent to a second fob through IR. Then, the second fob passes along this data to the poll leader, through UDP. The second fob acts as a UDP client in this case, whereas the poll leader acts as a UDP server. When the poll leader receives this data, it then becomes a udp client, and sends the data to a web server, where it is placed into a Tingo database, and displayed onto a webpage. The webpage shows a table of IDs, votes, and timestamps. Other features of the quest include electing a new fob as poll leader when the current pollleader fails, and displaying actual vote counts through querying the database. 


#### Investigative Question
List 5 different ways that you can hack the system (including influencing the vote outcome or prevenging votes via denial of service)
For each above, explain how you would mitigate these issues in your system.
1. Spoofing: When the server is down, it is possible for a local user to spoof the normal server by starting their own server. In this way the 'spoof' server can read the data and queries made. In our system, we could create sockets and only allow certain permissions from certain users.
2. Denial of Service: A denial of services attack, such as a UDP flood can be directed towards an unponed port an and make the messages sent unreachable (such as vote not sending data from a particular fob). This can be resolved by placing a firewall with UDP flood protection and by setting rate limits on the UDP traffic to be monitored and protect the network's bandwidth.
3. Denial of Service: This can also occur in a node.js file, where there is an attack on a database. In this case, you can hack the system by sending data incomprehensible to the parser of the data in the database and stop the database function from working and showing the correct amount of votes. This can be resolved by providing user authentication where only the authenticated and identified user can send data to the database to be read and stored correctly.
4. Enpoint attacks: A person could attack the wifi system in place with the ESP32's, thus not allowing them to communicate through UDP. This can be mitigated by creating a strong firewall that only allows certain users and IP addresses to pass through.
5. Data interception: Last, data interception can occur if another server is near the system and is able to read in the vote data. This would influence the vote outcomes because those fobs votes would not be counted in the final count of all of the fob votes. This can be mitigated by adding a more secure network and ensuring that data is only sent to a particular server.



## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Fob performs IR NFC data exchange of vote to another fob (LED indication) | 1 |  1     | 
| Receiving fob communicates vote to Poll Leader (LED indication) via network communication | 1 |  1     | 
| Poll leader (LED indication) is replaced if fails | 0 |  1     | 
| Poll Leader reports votes to server database. | 1 |  1     | 
| Portal allows query to database to show actual vote counts per candidate | 1 |  1     | 
| Operates over mutiple sites or with all available fobs (up to 9) | 1 |  1     | 
| Investigative question response| 1 |  1     | 

 
### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 4 |  5     | 
| Quality of report.md including use of graphics | 3 |  3     | 
| Quality of code reporting | 3 |  3     | 
| Quality of video presentation | 2 |  3     | 


## Solution Design
The testing of our code was a bit difficult because we could only test it on 3 fobs at a time. So, one fob was designated to choose the vote, a second fob received this vote through IR, and a final fob was the poll leader. It was especially difficult to test when the poll leader failed, bc this meant that if another fob was elected as poll leader, it had to also act as either an IR sender or receiver of the vote. Overall, our team did as much testing as possible with the limited resources we had. 


One interesting design aspect of our quest was programming fobs to work as both servers and clients. For example, the fob elected as poll leader had to act as a client (in order to send data to the overall server), and a server (in order to receive data from the IR receiving fob). The ID was taken into consideration when deciding which fobs should act clients, servers, or both. For example, because the initial fob sending the vote did not need to communicate through udp, the main function only creates the udp_client task for every ID except that of the initial fob. Likewise, since only the poll leader needed to act as a server, the udp_server task only ran if the current ID was classified as being the current poll leader. This precaution works well with a 3 fob system, and may need to be slightly altered to work for a system with more than 3 fobs, but it will likely still operate successfully. This however, cannot be tested, since only 3 fobs were available for testing out the code for this quest. 

Databases are a collection of information that can be easily accessed, manage, and updated. This is ideal, especially when modeling an election. Moreover, we are able to query into a database and access the amount of votes and which fob id's were used. In this manner, there is no need to manually count the number of votes paired with each candidate. In displaying the database on the local host webpage, we are able to ensure the timestamp, id, and vote associated when a user votes.

## Sketches and Photos

<i>Transfer of information: how the fobs connect</i>

<img src="https://user-images.githubusercontent.com/44929220/99035322-6c066500-254d-11eb-8dd7-5f6347e1a546.jpg" width="300" height="300">



<i>Data is received through IR, vote and fob ID sent to poll leader</i>

<img src="https://user-images.githubusercontent.com/44929220/99035348-76c0fa00-254d-11eb-83d7-f32d2dc3d036.jpeg" width="800" >



<i>Data is received by poll leader, and displayed on console in format "ID,vote"</i>

<img src="https://user-images.githubusercontent.com/44929220/99035323-6dd02880-254d-11eb-97a5-68e44fea9e78.jpg" width="800" >



<i>Table shown on webpage, based on data in TingoDB database</i>
<img src="https://user-images.githubusercontent.com/44929220/99128089-e7145d80-25d7-11eb-99dd-314ef10dcbe6.jpeg" width="800" >



## Supporting Artifacts
- [Link to video demo](https://youtu.be/nwn_TQlDHrc). 


## Modules, Tools, Source Used Including Attribution
- Node.js
- IR TX/RX
- Database
- GPIOs
- UDP Client/Server Communication 
- Bully Algorithm

## References
- [Election Algorithm](https://www.geeksforgeeks.org/election-algorithm-and-distributed-processing/#:~:text=The%20Bully%20Algorithm%20%E2%80%93,assumed%20that%20coordinator%20has%20failed) 
- [UDP Server/Client Implementation](https://www.geeksforgeeks.org/udp-server-client-implementation-c/)
- [Tingodb](http://www.tingodb.com/)
- [Design Patterns](http://whizzer.bu.edu/briefs/design-patterns/dp-db)
- [Timestamp](https://www.toptal.com/software/definitive-guide-to-datetime-manipulation#:~:text=Getting%20the%20Current%20Timestamp,passed%20since%20January%201%2C%201970)
- [Button](https://gist.github.com/aerrity/fd393e5511106420fba0c9602cc05d35)
- [Preveting server spoofing](https://www.postgresql.org/docs/9.1/preventing-server-spoofing.html)
- [UDP Floods, DOS](https://jncie.files.wordpress.com/2008/09/801003_protecting-the-network-from-denial-of-service-floods.pdf)

-----

