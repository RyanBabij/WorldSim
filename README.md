# WorldSim
Generates a fantasy world and uses it to simulate civilizations. This can be used to create a consistent history within which a game may take place. Basically it uses the same approach as [Dwarf Fortress](http://www.bay12games.com/dwarves/).

This project is currently in development, there is no release at the moment. Also it won't compile without some DLLs. More information can be found on the [wiki](https://github.com/RyanBabij/WorldSim/wiki).

## How does it work?

The "game" is divided into 3 parts: World Generator, World Simulator, and the game itself (I haven't come up with a name for that bit).

### World Generator

Generates a world using a fractal algorithm. The world has land, water, and biomes. Then it populates the world with tribes of Humans, Dwarves and Elves. There is a GUI which lets you adjust various settings and see what has been generated.

### World Simulator

The world will simulate the development of the three races and basically will dynamically generate a history for the world.

### The Game Bit

The plan is to allow you to create a character (or control an existing one), and then basically do whatever you want in this procedurally generated world. I guess I'll put some plot stuff in at some point. But the main purpose is mostly just an experiment in procedural generation.

## What's the goal of this project?

It's mostly just an experiment in procedurally generated worlds. I don't have plans to commercialise this project, but I would like to make a release at some point.

### Sample Output

Here's the world generator GUI right now:

![Screenshot of WorldGen](https://raw.githubusercontent.com/RyanBabij/WorldSim/master/doc/2018-06-26-Example01.png)

### Dependencies

This program, like all of my programs, depends on the [Wildcat](https://github.com/RyanBabij/Wildcat) code library. You must include that if you want to compile my program. I haven't done much deployment testing so it will probably not compile on your rig. I intend to fix that at some point.

### Compilation

You will need some DLLs in order to compile this. I will supply them later.

I make builds with:

g++ Driver.cpp  -o a.exe -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -I G:\c\lib\ -L G:\c\lib\zzzlibs\ -std=c++17 -m32 -O0 -lopengl32 -lfreeglut -lglu32 -s -Wl,--stack,16777216 -pipe -Wl,--large-address-aware -lstdc++fs -fshort-enums 2>ccOutput.txt

I'm not sure if you actually need the included freeglut.dll but too bad cos you're getting it anyway.

-ffast-math seems to cause instability when I run my builds on other computers. Clearly they can't handle my quick maths.

### Releases

I have a very early dev [release](https://github.com/RyanBabij/WorldSim/releases) you can play around with. Hey it's better than most of the other stuff on github.
