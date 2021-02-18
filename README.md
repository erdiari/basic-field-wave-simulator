# Basic Field Wave Simulator

A basic field wave generator i wrote to learn sdl and oop on cpp. Written in one day and debugged for two days while learning how stuff works. Handle with care. 

Scenes can be specified with .cpp files. sdl.cpp as name of it implies has sdl implementation to draw the field lines.

## Scene specification.
Creating a scene is actually pretty simple. First include `phy_sim.h` in to a cpp file. Then initialize the class `ConstantSpace` with respectively; size x, size y, x bias, y bias, divergence coefficient, curling coefficient, and one over epsilon value(will be used to calculate how precise calculations will be).

## Compiling
Download `SDL` , `g++` and `make` to your pc. Then clone this directory to a folder. Finally simply call `make` or `make sdl` commands in your terminal to compile main.cpp or sdl.cpp respectively. 
