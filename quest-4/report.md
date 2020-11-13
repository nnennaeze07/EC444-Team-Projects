# Quest 4 - Electronic Voting
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-11-12
-----

## Summary
In this quest, we were able to use the skills from this cluster and from pasts clusters to create an electronic voting system. Notably, we used IR-TX/RX to communicate between fobs, UDP clients and servers, databases, and node.js. The system starts with one fob voting on a color (red, green, blue). Upon bushing a button, the fob ID and its vote is sent to a second fob through IR. Then, the second fob passes along this data to the poll leader, through UDP. The second fob acts as a udp client in this case, whereas the poll leader acts as a udp server. When the poll leader receives this data, it then becomes a udp client, and sends the data to another server, where it is placed into a tingo database, and displayed onto a webpage. The webpage shows a table of IDs, votes, and timestamps. Other features of the quest include electing a new fob as poll leader when the current pollleader fails, and displaying actual vote counts through querying database. 


(Still editing -- took this from the site and planning to reword)
Complete the IR-TX/RX Skill: build fobs and bring up TX/RX code to demonstrate it works: Adapt code to transmit vote (R, B, G) using IR
Set up UDP message passing on your local wireless network: build a method to exchange vote payloads to one or more destinations (e.g., point-to-point or point-multipoint). Make sure it works from fob to leader and leader to node.js
Complete the Database Skill: set up tingodb or equivalent on your server for the vote data and be able to receive and save data from the leader to the node.js and to the DB.
Interconnect your node.js server to the DB and host the queries for the vote data on a web page.
Integrate these steps into as single application that is loaded onto all of the fobs.
Do this last, once the above works: Implement a failover method if the Poll Leader is shut down. A new Poll Leader must be selected within 45 sec.

#### Investigative Question
List 5 different ways that you can hack the system (including influencing the vote outcome or prevenging votes via denial of service)
For each above, explain how you would mitigate these issues in your system.
1. A
2. B
3. C
4. D
5. E

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Fob performs IR NFC data exchange of vote to another fob (LED indication) |  |  1     | 
| Receiving fob communicates vote to Poll Leader (LED indication) via network communication |  |  1     | 
| Poll leader (LED indication) is replaced if fails |  |  1     | 
| Poll Leader reports votes to server database. |  |  1     | 
| Portal allows query to database to show actual vote counts per candidate |  |  1     | 
| Operates over mutiple sites or with all available fobs (up to 9) |  |  1     | 
| Investigative question response|  |  1     | 

 
### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution |  |  5     | 
| Quality of report.md including use of graphics |  |  3     | 
| Quality of code reporting |  |  3     | 
| Quality of video presentation |  |  3     | 


## Solution Design
// talk about how we could only test with 3 fobs
The testing of our code was a bit difficult because we could only test it on 3 fobs at a time. So, one fob was designated to choose the vote, a second fob received this vote through IR, and a final fob was the poll leader. It was especially difficult to test when the poll leader failed, bc this meant that if another fob was elected as poll leader, it had to also act as either an IR sender or receiver of the vote. Overall, our team did as much testing as possible with the limited resources we had. 

// talk about how fobs were used as both client and server
One interesting design aspect of our quest was programming fobs to work as both servers and clients. For example, the fob elected as poll leader had to act as a client (in order to send data to the overall server), and a server (in order to receive data from the IR receiving fob). The ID was taken into consideration when deciding which fobs should act clients, servers, or both. For example, because the initial fob sending the vote did not need to communicate through udp, the main function only creates the udp_client task for every ID except that of the initial fob. Likewise, since only the poll leader needed to act as a server, the udp_server task only ran if the current ID was classified as being the current poll leader. This precaution works well with a 3 fob system, and may need to be slightly altered to work for a system with more than 3 fobs, but it will likely still operate successfully. This however, cannot be tested, since only 3 fobs were available for testing out the code for this quest. 

//lesbeth talk about database stuff


## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>


## Supporting Artifacts
- [Link to video demo](). Not to exceed 120s


## Modules, Tools, Source Used Including Attribution
- Node.js

- IR TX/RX

- Database

- GPIOs

- UDP Client/Server Communication 

- Bully Algorithm

## References
https://www.geeksforgeeks.org/election-algorithm-and-distributed-processing/#:~:text=The%20Bully%20Algorithm%20%E2%80%93,assumed%20that%20coordinator%20has%20failed. 

https://www.geeksforgeeks.org/udp-server-client-implementation-c/

-----

