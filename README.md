This project was created as my capstone project for the Udacity C++ Nanodegree I'm taking. My objective is to create a game.

## Dependencies
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0, SDL_TTF, SDL_Image
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

I was only able to run this project on Windows Subsystem for Linux (WSL) and the Udacity-provided environment. I was never able to run this project on Windows because I couldn't figure out how to make a CMakeLists.txt file that could successfully run on Windows. I never tried to run this project on Mac.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./z`.

## Project Selection

I decided to build a game.

The player has a single survivor, as indicated by a blue token. The player must survive for an unknown number of days by scavenging for food and materials in buildings throughout the map.

## File and Class Structure

This game uses the same model as the Udacity Snake Game project. It uses the input, update, render model. The main classes are Game, Controller, and Renderer. The main.cpp file creates instances of these objects and then Game.run method is run with the other two instsances as parameters. The Game.run method has the main while loop, during which user input is detected, game attributes are updated, and graphics are rendered.

Many of the Game class's attributes are instances of many of the other classes found in the other files of the project, such as Player, Overlay, a vector of Buildings, a vector of Buttons, etc.

Upon startup, a window should open. This window should have four parts. The left panel, the top panel, the bottom panel, and the map on which buildings and player tokens are visible. All input is handled via clicks.

The left panel shows information about the clicked on building, the player, and the buttons.

The top panel shows information about day, time, and base resources.

The bottom panel shows information about the most recently performed action.

Upon game termination, a message ????????????

## Rubric Points

As per the project requirements, this section of the README will indicate (at least) five rubric points' criteria that have been satisfied.

1. The project uses Object Oriented Programming techniques. 
 * The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
 * example: game.h and game.cpp. The Game class contains many attributes (some of which are instances of other classes from this project) to hold data and methods to perform tasks.

2. Classes use appropriate access specifiers for class members.
 * All class data members are explicitly specified as public, protected, or private.
 * example: All class data members are explicitly specified as public, protect, or private.

3. Classes follow an appropriate inheritance hierarchy.
 * Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.
 * example: In sprites.h and sprites.cpp, the Building class and Button class both inherit from Sprite class. ( spites.h, lines 85 and 105 )

 4. The project makes use of references in function declarations.
 * At least two variables are defined as references, or two functions use pass-by-reference in the project code.
 * example 1: The method Controller.handleEvent takes references as arguments. See controller.h, line 11.
 * example 2: The method Renderer.renderAll takes references as arugments. See renderer.h, line 20.

 5. The project uses smart pointers instead of raw pointers.
 * The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.
 * example: The Game class has an attribute _buildings (buildings.h, line 45) that uses a vector of shared pointers.
 * The project does use some raw pointers, but only when using the SDL Library. 

## Attributions

As a starting point for this project, I looked at the Udacity Snake Game and kept the core structure of Game, Controller, Renderer. I deleted eveything I thought I didn't need and then built up from there.

To learn how to use the SDL library, I looked at the SDL tutorials from Lazy Foo. His tutorials are pure quality and far superior to anything else I saw.

I used a font called BlackCapsRegular-wgA2. It's licensed under the CC Attribution license. Per the license, I must provide the name of its designer, G369.
https://www.fontspace.com/black-caps-font-f11717