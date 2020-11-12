# Quest 4 - Electronic Voting
Authors: Nnenna Eze, Lesbeth Roque

Date: 2020-11-12
-----

## Summary
In this quest, we were able to use the skills from this cluster and from pasts clusters to create an electronic voting system. Notably, we used IR-TX/RX to communicate between fobs, UDP clients and servers, databases, and node.js. 

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



## Sketches and Photos
<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>


## Supporting Artifacts
- [Link to video demo](). Not to exceed 120s


## Modules, Tools, Source Used Including Attribution
- Node.js
- TX/RX
- Database
- GPIOs

## References

-----

