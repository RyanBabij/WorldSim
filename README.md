# WorldSim
Generates a fantasy world and uses it to simulate civilizations. This can be used to create a consistent history within which a game may take place. Basically it uses the same approach as [Dwarf Fortress](http://www.bay12games.com/dwarves/). More information can be found on the [wiki](https://github.com/RyanBabij/WorldSim/wiki).

## How does it work?

The "game" is divided into 3 parts: World Generator, God Mode and Adventure Mode.

### World Generator

Generates a world using a fractal algorithm. The world has land, water, and biomes. Then it populates the world with tribes of Humans, Dwarves and Elves. There is a GUI which lets you adjust various settings and see what has been generated. I think it's pretty cool and mostly done at this point, just needs some better graphics.

### God Mode

The world will simulate the development of the three races and basically will dynamically generate a history for the world. I'll probably put in some extra features like being able to modify Characters and stuff, but for now all you can do is watch. At the moment all that happens is Tribes wander around randomly and claim territory and fight each other. The game does track family trees but you can't do much with it right now.

The current focus is getting Tribes to do interesting things, and eventually advance to Civilizations. Civilizations will be able to build armies and cities, conduct trade and diplomacy, etc, etc.

### Adventure Mode

You can select a Character and then walk around the world with them. Currently there is very little interaction between God Mode and Adventure Mode. I'm working on improving that. Right now you can walk around and explore the world but there's little to see.

## What's the goal of this project?

It's mostly just an experiment in procedurally generated worlds. I don't have plans to commercialise this project, but I would like to make a release at some point. I'm trying to get to a minimum viable product, but I have no idea when I'll get there. When I do, I'll release it as 0.1.

I'm trying to make something on the scale of DF, but honestly it's quite different in the implementation. For a start my game implements a reasonably modern interface. None of this fixed-window, keyboard-only, ASCII business. Secondly the focus is on the Adventure Mode. Strategy elements are planned but will be optional. Thirdly the game tries to avoid unnecessary complexity. The game is hard enough to write without worrying about left and right socks.

Finally there's an emphasis on creating a "living world". Where possible, I am trying to simulate things happening off-screen. This is a much more complex goal than one might assume. Sure, you can run A* pathfinding for a thousand entities, but when you're trying to do it for millions, you start running into interesting problems. One of the few games I know that did it well is Falcon 4.0, and the programmer Kevin Klemmick said it was a hell of a job. Many developers talk about implementing such a thing but it never actually happens. I want to make it happen, even if only for a 2D tile-based game.

If the project goes well I would like it to be a kind of base which other developers could use to make their own worlds. The code is all there for people to mess around with. I mean, theoretically you could even swap out my 2D renderer for a 3D one, but I don't have the time or skill to do such a thing myself.

### Sample Output

Here's the world generator GUI right now:

![Screenshot of WorldGen](https://raw.githubusercontent.com/RyanBabij/WorldSim/master/doc/2018-06-26-Example01.png)

It that doesn't look amazing to you then go play CoD or something.

### Dependencies

This program, like all of my programs, depends on the [Wildcat](https://github.com/RyanBabij/Wildcat) code library. You must include that if you want to compile my program. I haven't done much deployment testing so it will probably not compile on your rig. I intend to fix that at some point.

### Compilation

The project isn't easy to build because it's just my personal project. I don't use make. However if you know what you're doing you could probably figure it out. Currently I use MingW GCC for builds, and Clang for development. I have compiled for both Windows and Linux, however currently I only use Windows, so there is probably some Windows-only code in the current commits.

Builds as of 0.0.121 use:

-Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -std=c++17 -m32 -O3 -lopengl32 -lfreeglut -lglu32 -s -static-libgcc -static-libstdc++ -fno-strict-overflow -flto -fno-rtti -pipe -Wl,--large-address-aware -fno-exceptions -fshort-enums info.res

Just as a side-note, -mtune=native -march=native -ffast-math together seem to increase performance by about 3%. I guess it's not a major issue. I also tried the following crazy-ass flags: -fassociative-math -freciprocal-math -fno-signed-zeros -fno-trapping-math -frename-registers -fopenmp -D_GLIBCXX_PARALLEL -fgraphite-identity -floop-interchange -floop-block -floop-parallelize-all -ftree-loop-distribution -ftree-parallelize-loops=4 and got absolutely no gain in performance so whatevs.

-Os is causing some kind of linker issues, but I don't really use -Os so it doesn't bother me.

I would like to use AddressSanitiser but apparently it's designed for Linux so I might take a while to get around to it.

### Releases

I have some pre-alpha [releases](https://github.com/RyanBabij/WorldSim/releases) you can play around with. Hey it's better than most of the other stuff on github.
