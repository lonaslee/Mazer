# Mazer

An application to generate an _m_ by _n_ maze using a selectable algorithm.  

Written in C, using the SDL application library.

![Running Application](/imgs/run.gif)

A comparision of four different algorithms:  

![Comparing Algorithms](/imgs/four.gif)

There are in total 6 available algorithms, each producing a unique look of a maze and in varying amounts of time:  

1) Alduous Broder - high randomness, _very_ slow for dense mazes
2) Binary Tree - branch-like walls stemming from the bottom right corner
3) Recursive Backtracker - snakelike generation sequence
4) Sidewinder - long horizontal paths
5) Eller's - most even algorithm
6) Hunt & Kill - similar to 1, but generates much faster  

Also in the source code is the implementation of array, linkedlist, stack, and tree utilized by the maze generating algorithms, which are standalone files one can use elsewhere.

## Installation

### Windows

__Install__:  
Download the release, run the .exe  

__Build__:  

1. Download SDL, SDL_image, and SDL_ttf from libsdl.org, place them in the root folder
2. `mv makefile.win64 makefile`, or `mv makefile.win32 makefile`
3. `make a`

### Linux

__Install__:

1. Install dependencies: ```sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev```
2. Download & run the release

__Build__:

1. Install dependencies as above
2. `mv makefile.linux makefile`
3. `make a`

### MacOS

__Install__:  

1. Install dependencies: ```brew install sdl2 sdl2_image sdl2_ttf```
2. Download & run the release

__Build__:  

1. Install dependencies as above
2. `mv makefile.mac makefile`
3. `make a`
