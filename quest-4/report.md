# Quest 4 - Electronic Voting
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-11-12
-----

## Summary
In this quest, we were able to use the skills from this cluster and from pasts clusters to create an electronic voting system. Notably, we used IR-TX/RX to communicate between fobs, UDP clients and servers, databases, and node.js. The system starts with one fob voting on a color (red, green, blue). Upon pushing a button, the fob ID and its vote is sent to a second fob through IR. Then, the second fob passes along this data to the poll leader, through UDP. The second fob acts as a UDP client in this case, whereas the poll leader acts as a UDP server. When the poll leader receives this data, it then becomes a udp client, and sends the data to a web server, where it is placed into a Tingo database, and displayed onto a webpage. The webpage shows a table of IDs, votes, and timestamps. Other features of the quest include electing a new fob as poll leader when the current pollleader fails, and displaying actual vote counts through querying the database. 


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
| Fob performs IR NFC data exchange of vote to another fob (LED indication) | 1 |  1     | 
| Receiving fob communicates vote to Poll Leader (LED indication) via network communication | 1 |  1     | 
| Poll leader (LED indication) is replaced if fails |  |  1     | 
| Poll Leader reports votes to server database. | 1 |  1     | 
| Portal allows query to database to show actual vote counts per candidate |  |  1     | 
| Operates over mutiple sites or with all available fobs (up to 9) | 1 |  1     | 
| Investigative question response|  |  1     | 

 
### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 4 |  5     | 
| Quality of report.md including use of graphics | 3 |  3     | 
| Quality of code reporting | 3 |  3     | 
| Quality of video presentation |  |  3     | 


## Solution Design
The testing of our code was a bit difficult because we could only test it on 3 fobs at a time. So, one fob was designated to choose the vote, a second fob received this vote through IR, and a final fob was the poll leader. It was especially difficult to test when the poll leader failed, bc this meant that if another fob was elected as poll leader, it had to also act as either an IR sender or receiver of the vote. Overall, our team did as much testing as possible with the limited resources we had. 


One interesting design aspect of our quest was programming fobs to work as both servers and clients. For example, the fob elected as poll leader had to act as a client (in order to send data to the overall server), and a server (in order to receive data from the IR receiving fob). The ID was taken into consideration when deciding which fobs should act clients, servers, or both. For example, because the initial fob sending the vote did not need to communicate through udp, the main function only creates the udp_client task for every ID except that of the initial fob. Likewise, since only the poll leader needed to act as a server, the udp_server task only ran if the current ID was classified as being the current poll leader. This precaution works well with a 3 fob system, and may need to be slightly altered to work for a system with more than 3 fobs, but it will likely still operate successfully. This however, cannot be tested, since only 3 fobs were available for testing out the code for this quest. 


//lesbeth talk about database stuff


## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>

<i>Transfer of information: how the fobs connect</i>

<img src="https://user-images.githubusercontent.com/44929220/99035322-6c066500-254d-11eb-8dd7-5f6347e1a546.jpg" width="300" height="300">



<i>Data is received through IR, vote and fob ID sent to poll leader</i>

<img src="https://user-images.githubusercontent.com/44929220/99035348-76c0fa00-254d-11eb-83d7-f32d2dc3d036.jpeg" width="600" height="300">



<i>Data is received by poll leader, and displayed on console in format "ID,vote"</i>

<img src="https://user-images.githubusercontent.com/44929220/99035323-6dd02880-254d-11eb-97a5-68e44fea9e78.jpg" width="600" height="300">



<i>Table shown on webpage, based on data in TingoDB database</i>
<img src="https://user-images.githubusercontent.com/44929220/99128089-e7145d80-25d7-11eb-99dd-314ef10dcbe6.jpeg" width="600" height="300">



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

