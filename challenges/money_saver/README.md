Money Saver Challenge
=====================

###Rules
The game consists in 2 intelligent agents: (money) savers and thieves.
Savers have to collect coins. Thieves have to rob the savers.
If a saver goes to the Bank, it stores all his current money in it.
At the end of the game, if the total sum of all money on every saver and in the
bank is greater than all the money on every thief, the savers win. Otherwise,
the thieves win.

###Objective
Savers: have more money (including in the bank) than the thieves.
Thieves: have more money than the savers.

###Terrain

- Rectangular matrix (N x M)

####Tiles
- 0: Nothing
- 1: Wall
- 2: Coin
- 3: Bank
- 4: Money Saver
- 5: Thief

Example:

0000000000  
0000000050  
0222222220  
0200000020  
0200330020  
0200330020  
0200000020  
0222222220  
0400000000  
0000000000  

###Agents
Savers and thieves are intelligent agents.


Clients
-------
###Building with CMake
Install CMake
``sudo apt-get install cmake``

####Normal client
On ime_machinelearning root folder, do the following:
``
mkdir build
cd build
cmake ..
make
cd bin/challenges/money_saver
./money_saver libsaverTemplate.so libthiefTemplate.so
``

####Graphical client
#####Building with CMake
Install GLFW 3
``sudo apt-get install libglfw3``

On ime_machinelearning root folder, do the following:
``
mkdir build
cd build
cmake ..
make
cd bin/challenges/money_saver
./money_saver_gfx libsaverTemplate.so libthiefTemplate.so
``
