# League of Legends Championship Management System - Version 3
This is an implementation of a program that maintains a database for a League of Legends Championship using a simulated data file as a string and a Hashtable as the primary indexing structure. It was implemented as part of an exercise for a Data Structure class.

There are two implementations:
- Implementation A uses linear probing as the collision resolution strategy. 
- Implementation B uses chaining with linked lists as the collision resolution strategy. 

There are also some differences in the way both implementations list their entries using the Primary Key, as seen below:

Version A:
```
[0] Ocupado: QLTN3005
[1] Ocupado: SLPY0809
[2] Livre
[3] Livre
[4] Ocupado: CBMA0309
[5] Ocupado: FIRE2204
[6] Ocupado: TRLI0309
[7] Ocupado: QTQG3101
[8] Ocupado: CKNA1309
[9] Livre
[10] Ocupado: PKBR1911
```

Version B:
```
[0]
[1]
[2]
[3]
[4] CBMA0309
[5] FIRE2204 TRLI0309
[6]
[7] QTQG3101
[8] CKNA1309
[9]
[10] PKBR1911 QLTN3005 SLPY0809
```

The database contains the following fields:

- Primary Key: Composed by the first letter of Blue Team, first letter of Red Team, first two letters of the MVP (Most Valuable Player), and the date of the match composed by day and month. Ex: FTFE0205 (8 bytes)
- Blue Team: Name of the blue team, ex: Fnatic (Variable Size)
- Red Team: Name of the red team, ex: Team Solo Mid (Variable Size)
- Match Date: Date of the match, ex: DD/MM/AAAA (10 bytes)
- Match Duration: Duration of the match in minutes and seconds, ex MM:SS (5 bytes)
- Winner Team: Name of the winner team. (Variable Size)
- Match Score: Score of the match, ex: 15, 06 (4 bytes) 
- MVP Nickname: Nickname of the Most Valuable Player, ex: Febiven (Variable Size)

The hash function is:
```
h(k) = (1 ∗ k1 + 2 ∗ k2 + 3 ∗ k3 + 4 ∗ k4 + 5 ∗ k5 + 6 ∗ k6 + 7 ∗ k7 + 8 ∗ k8) mod T

k = primary key with 8 characters
ki = i-th character of primary key
T = Hashtable size
```

Each entry in the database will have a maximum storage of 192 bytes per entry. These fields are stored in a data file as shown below:

```
FTFE0705@Fnatic@Team SoloMid@07/05/2015@31:47@Fnatic@15@06@Febiv
en@#############################################################
################################################################
SEDE1005@SKTelecom T1@Edward Gaming@10/05/2015@37:38@Edward Gami
ng@09@25@Deft@##################################################
################################################################
AHWE2607@ahq e-Sports Club@HongKong Esports@26/07/2015@39:09@ahq
e-Sports Club@20@14@westdoor@##################################
################################################################
PIMY0808@PaiN Gaming@INTZ@08/08/2015@42:55@PaiN Gaming@10@07@Myl
on@#############################################################
################################################################
JNCH0209@Jin Air Green Wings@NaJin e-mFire@02/09/2015@39:51@Jin
Air Green Wings@08@03@Chei@#####################################
################################################################
```

This solution implements three indexes:

- Primary Index containing the Primary Key and offset of the entry in the file
- Secondary Winner Index relating a Winner Team to its Primary Key
- Secondary MVP Index relating a MVP to its Primary Key

Inside the program, the following options are provided to the final user:
- 1. Create new match
- 2. Remove match using Primary Key
- 3. Update Match Duration using Primary Key 
- 4. Search matches:
  - Using Primary Key
  - Using Winner
  - Using MVP
- 5. List all matches:
  - Using Primary Key
  - Using Winner
  - Using MVP
- 6. Free Memory

This repository includes .in files for testing purposes.
