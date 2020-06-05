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

If the project goes well I would like it to be a kind of base which other developers could use to make their own worlds. The code is all there for people to mess around with. I mean, theoretically you could even swap out my 2D renderer for a 3D one, but I don't have the time or skill to do such a thing myself. The algorithms are high-performance out of necessity, however I try to make the overall design fairly simple.

### Sample Output

Here's the world generator GUI right now:

![Screenshot of WorldGen](https://raw.githubusercontent.com/RyanBabij/WorldSim/master/doc/2018-06-26-Example01.png)

It doesn't look too pretty because it's basically just biome data. However each tile is its own map which can be explored. The graphics are quite bad but if you've played DF or other roguelikes I think you'll have no problem here.

### Dependencies

This program, like all of my programs, depends on the [Wildcat](https://github.com/RyanBabij/Wildcat) code library. You must include that if you want to compile my program. I haven't done much deployment testing so it will probably not compile on your rig. I intend to fix that at some point.

### Build instructions (Windows 10)

I don't have a makefile or anything so you have to do it manually.

I do a lot of compilation so I use LLVM clang++ for quick test builds, and MingW-W64 g++ for release builds. Ideally you will want to set up both of these, but you can install just MingW-W64 if you prefer.

I have tested these instructions on a fresh Windows 10 install, so hopefully they should work for everyone. Setting up the environment on Linux is probably much easier but I do most of my development on Windows.

1. Clone the WorldSim repo to your computer.

2. Download latest WorldSim release and copy the textures into the source folder. (The game should start without textures but obviously you will want them).

3. Clone the Wildcat repo to your computer.

4. Create an environment variable called WILDCAT which points to wherever you put the Wildcat code libraries.

5. clang++:

Install LLVM Clang Windows 64 bit binary from https://releases.llvm.org/download.html

Make sure you set the PATH for the current user. You should be able to call clang++ from this point. The PATH should be something like C:\Program Files\LLVM\bin

NOTE: LLVM Clang doesn't come with the basic libstdc++ libraries. The best way to deal with this is to also install MingW g++ and let it use those libraries.

6. g++

Using standard MinGW will cause problems because it lacks threading support. mingw-w64 is required instead. mingw-w64 is an updated MinGW with support for new APIs which this program uses, mostly just threading. Despite the name, it's for both 32 and 64 bit Windows.

https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download

Install for i686 POSIX. Install to C:/mingw-w64. Add C:/mingw-w64/mingw32/bin to PATH. This should allow LLVM to access the libstdc++ libraries and compile correctly.

7. Compilation

With any luck, both clang++ and g++ should now correctly compile the project. Here are my compilation commands:

Testing build:

clang++ -target i686-pc-windows-gnu Driver.cpp -I %WILDCAT%\ -I %WILDCAT%\Lib\ -L %WILDCAT%\Lib\ -std=c++17 -lglew32 -lfreeglut_static -lopengl32 -lglu32 -m32 -lwinmm -lgdi32 -pthread -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-const-variable -Wno-unused-value -Wl,--large-address-aware

Release build:

g++ Driver.cpp -I %WILDCAT%\ -I %WILDCAT%\Lib\ -L %WILDCAT%\Lib\ -std=c++17 -lglew32 -lfreeglut_static -lopengl32 -lglu32 -m32 -lwinmm -lgdi32 -pthread -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-const-variable -Wno-unused-value -static-libgcc -static-libstdc++ -fno-strict-overflow -flto -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -pipe -fshort-enums -O3 -s -Wl,--gc-sections -Wl,--large-address-aware

I don't use IDEs for my work, I currently just use [Notepad++](https://notepad-plus-plus.org/) for writing code, and [ConsoleZ](https://github.com/cbucher/console) as my command line.

### Releases

I have some pre-alpha [releases](https://github.com/RyanBabij/WorldSim/releases) you can play around with. Hey it's better than most of the other stuff on github.

### Contributions

All contributions are welcome.

### License

It's currently open source and public domain. Once the project becomes viable, I will probably change to a more restrictive license, but all old releases keep their old license.

### Style guide

I prefer the Allman style. Column width is 120. Function names should be descriptive rather than short.

Classes are UpperCamelCase, objects are camelCase, functions are camelCase. Iterators/indexes start with i, arrays start with a, vectors start with v, global variables/defines are UPPERCASE, local shadow variables start with an \_underscore.

Indentation is tabbed, best viewed at 3 spaces per tab.

Indentation is a mess. I'm in the process of moving from spaced indentation to tabbed indentation, because I discovered that you can set Github's tab width with .editorconfig.

### Documentation

I'll be working on adding Doxygen documentation, which can be found at https://codedocs.xyz/RyanBabij/WorldSim/

Non-code documentation can be found on the [wiki](https://github.com/RyanBabij/WorldSim/wiki).
