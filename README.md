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

The plan is to allow you to create a character (or control an existing one), and then basically do whatever you want in this procedurally generated world. I guess I'll put some plot stuff in at some point.

## What's the goal of this project?

It's mostly just an experiment in procedurally generated worlds. I don't have plans to commercialise this project, but I would like to make a release at some point.

### Sample Output

Here's the world generator GUI right now:

![Screenshot of WorldGen](https://raw.githubusercontent.com/RyanBabij/WorldSim/master/doc/2018-06-26-Example01.png)

It that doesn't look amazing to you then go play CoD or something.

### Dependencies

This program, like all of my programs, depends on the [Wildcat](https://github.com/RyanBabij/Wildcat) code library. You must include that if you want to compile my program. I haven't done much deployment testing so it will probably not compile on your rig. I intend to fix that at some point.

### Compilation

You can't. I think you need dlls or something to compile this properly. I'll figure it out later.

After 0.0.008 I make builds with:

-Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -std=c++17 -m32 -O3 -lopengl32 -lfreeglut -lglu32 -s -static-libgcc -static-libstdc++ -fno-strict-overflow -flto -fno-rtti -pipe -Wl,--large-address-aware -fno-exceptions -fshort-enums

Just as a side-note, -mtune=native -march=native -ffast-math together seem to increase performance by about 3%. I guess it's not a major issue. I also tried the following crazy-ass flags: -fassociative-math -freciprocal-math -fno-signed-zeros -fno-trapping-math -frename-registers -fopenmp -D_GLIBCXX_PARALLEL -fgraphite-identity -floop-interchange -floop-block -floop-parallelize-all -ftree-loop-distribution -ftree-parallelize-loops=4 and got absolutely no gain in performance so whatevs.

-Os is causing some kind of linker issue with GUI/GUI.hpp. I noticed there is a lot of dependency problems in that code so I will need to first resolve that.

### Releases

I have a very early dev [release](https://github.com/RyanBabij/WorldSim/releases) you can play around with. Hey it's better than most of the other stuff on github.
