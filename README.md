# Ars Magica Helper Application

## Overview
This project is meant to create a GUI interface that makes tracking
character information for the tabletop [Ars Magica](https://www.atlas-games.com/arsmagica/) much easier.
The goals are:
- To handle all the calculation of abilities, arts, and various formulae within the game.
- Have a season tracker within which the player can view events, both for character progression and for story beats.
- Easy to use. The user shouldn't be confused about how to do things.
- Extensibility. Should the user desire, they should be able to add their own formulae, edit existing formulae, and configure their view

## How to Contribute
Clone the repo to some workspace folder. Within that workspace folder,
open up a terminal and run the following commands in the 
root of the workspace.
```
mkdir build
cd build
cmake ..
```

If you are on windows, use the following cmake command instead:
```
cmake -A Win32 ..
```

And if you would like to edit in visual studio, use this command to generate the sln file:
```
cmake -G "Visual Studio 17 2022" -A Win32 ..
```
(If you are not using Visual Studio 2022 edition, make sure to change the -G flag appropriately. Here's the cmake docs on how to pick the correct flag based on your version: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)