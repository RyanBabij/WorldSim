# WorldSim
Takes WorldGen data and uses it to simulate civilisations. This can be used to create a consistent history within which a game may take place. Basically it uses the same approach as [Dwarf Fortress](http://www.bay12games.com/dwarves/).

This project is currently in development, there is no release at the moment.

## How does it work?

The "game" is divided into 3 parts: World Generator, World Simulator, and the game itself (I haven't come up with a name for that bit).

### World Generator

Generates a world using a fractal algorithm. The world has land, water, and biomes. Then it populates the world with tribes of Humans, Dwarves and Elves. There is a GUI which lets you adjust various settings and see what has been generated. This part currently generates world tiles, but I also need to add local map generation.

### World Simulator

The world will simulate the development of the three races and basically will dynamically generate a history for the world. The plan is to be able to simulate roughly 1,000 years of history in a reasonable amount of time (with a very flexible definition of "reasonable").

### The Game Bit

The plan is to allow you to create a character (or control an existing one), and then basically do whatever you want in this procedurally generated world. I guess I'll put some plot stuff in at some point. But the main purpose is mostly just an experiment in procedural generation.

## What's the goal of this project?

It's mostly just an experiment in procedurally generated worlds. I don't have plans to commercialise this project, but I would like to make a release at some point.

## Sample Output

Todo

## Dependencies

This program, like all of my programs, depends on the [Wildcat](https://github.com/RyanBabij/Wildcat) code library. You must include that if you want to compile my program. I haven't done much deployment testing so it will probably not compile on your rig. I intend to fix that at some point.

## Compilation

Todo

## More info

Todo

##Test4