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

Inside the _agent.h_ there's a enum that abstract this:

```cpp
enum class Terrain
{
  NONE,
  WALL,
  COIN,
  BANK,
  SAVER,
  THIEF,
  NUM_TERRAINS // Internal usage
};
```

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
Agents have a limited vision range. The vision of the agent is a square
matrix around the agent position, in which the agent is in the center of the
matrix. The matrix dimensions are always
(2 * vision_size + 1) x (2 * vision_size + 1).

Example:

> vision_size = 1

| 0 | 1 | 2 |
|---|---|---|
| 3 | 4 | 5 |
|---|---|---|
| 6 | 7 | 8 |

- Position 4: Agent

Valid vision:

| WALL | NONE  | BANK  |
|------|-------|-------|
| WALL | SAVER | COIN  |
|------|-------|-------|
| COIN | WALL  | THIEF |


All agents inherit from the same base class (Agent).  
Every turn, the client will call the the method _walk_ of every
agent (savers first, thieves last).  
The walk method is the intelligence of the agent. It will receive the
vision around him and have to return one of the possible directions to
walk.

Walk method signature

```cpp
Direction walk(Terrain vision[])
```

Directions:

```cpp
enum class Direction
{
  NONE,   // Stay still
  UP,     // Walk up
  DOWN,   // Walk down
  LEFT,   // Walk left
  RIGHT,  // Walk right
  NUM_DIRECTIONS // Internal usage
};
```


Clients
-------
###Building with CMake
Install CMake

``sudo apt-get install cmake``

####Normal client
On ime_machinelearning root folder, do the following:

```sh
mkdir build
cd build
cmake .
make money_save
cd bin/challenges/money_saver
```

####Graphical client
#####Building with CMake
Install GLFW 

``sudo apt-get install libglfw3``

On ime_machinelearning root folder, do the following:

```sh
mkdir build
cd build
cmake .
make money_save
cd bin/challenges/money_saver
```

###Executing the client
####Normal client
```
./money_saver (saver agent).so (thief agent).so
```

####Graphical client
```
./money_saver_gfx (saver agent).so (thief agent).so
```

Building your own intelligence
------------------------------
All savers and thieves have to inherit the Agent class interface (agent.h).

Example:
```cpp
#include "challenges/money_saver/agent.h"

class Template : public Agent
{
public:
  virtual Direction walk(Terrain vision[]) override
  {
    return Direction::UP;
  }
};
```

The game client will search for a function maker (signature ``Agent* maker()``).
To create your own maker function, do the following:

```c
extern "C" Agent* maker(const int vision_size)
{
  return new Template {vision_size};
}
```

(See _agents/template.h_, a random walker, to better understand agents).

To compile you have two options: using cmake or g++/clang++.
####Compiling your agent via CMake
Place your source file and any header you use in the folder
``ime_machinelearning/challenges/money_saver/agents`` and
write the line below at the end of CMakeLists.txt.

```
add_agent(<agent name> <sources>)
```

Now build the challenge (see _Building with CMake_ above).  
This will generate a file ``lib(agent name).so`` in the folder
``ime_machinelearning/build/bin/challenges/money_saver``.  
To execute the client with your agent, see _Executing the client_ above.

####Compiling with g++/clang++
On the folder your agent source files are, do the following:  
(To use clang++, change _g++_ to _clang++_)

```
g++ <sources> -o <agent name>.so -fpic -shared -std=c++11
```

Place the generated lib in
``ime_machinelearning/build/bin/challenges/money_saver``.
To execute the client with your agent, see _Executing the client_ above.
