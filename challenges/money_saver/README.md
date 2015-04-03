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

```
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
```

###Agents
Savers and thieves are intelligent agents.


Clients
-------
###Building with CMake
Install CMake  
``sudo apt-get install cmake``

####Normal client
On ime_machinelearning root folder, do the following:  
>  mkdir build  
>  cd build  
>  cmake ..  
>  make money_saver  
>  cd bin/challenges/money_saver  
>  ./money_saver libsaverTemplate.so libthiefTemplate.so

####Graphical client
#####Building with CMake
Install GLFW 3  
``sudo apt-get install libglfw3``  

On ime_machinelearning root folder, do the following:  

```
mkdir build
cd build
cmake .
make money_save
cd bin/challenges/money_saver
```

###Executing the client
####Normal client
> ./money_saver (saver agent).so (thief agent).so

####Graphical client
> ./money_saver_gfx (saver agent).so (thief agent).so

Building your own intelligence
------------------------------
All savers and thieves have to inherit the Agent class interface (agent.h).  

Example:
```
&#35;include "challenges/money_saver/agent.h"

class Template : public Agent
{
public:
  virtual Direction walk(Terrain vision[], int vision_size)
  {
    return Direction::UP;  
  }
};
```

The game client will search for a function maker (signature ``Agent* maker()``).
To create your own maker function, do the following:

```
extern "C" Agent* maker()
{
  return new Template;
}
```

To compile you have two options: using cmake or g++/clang++.
####Compiling your agent via CMake
Place your source file and any header you use in the folder
``ime_machinelearning/challenges/money_saver/agents`` and
write the line below at the end of CMakeLists.txt.

> add_agent(<agent name> <sources>)

Now build the challenge (see _Building with CMake_ above).  
This will generate a file ``lib(agent name).so`` in the folder
``ime_machinelearning/build/bin/challenges/money_saver``.  
To execute the client with your agent, see _Executing the client_ above.

####Compiling with g++/clang++
On the folder your agent source files are, do the following:  
(To use clang++, change _g++_ to _clang++_)

> g++ <sources> -o <agent name>.so -fpic -shared

Place the generated lib in
``ime_machinelearning/build/bin/challenges/money_saver``.
To execute the client with your agent, see _Executing the client_ above.
