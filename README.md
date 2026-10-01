# Dodge Ball 2D

A simple 2D arcade game built in C++ using OpenGL and GLFW. The player controls a blue ball and must avoid incoming red balls that fall from the top of the screen. The longer you survive, the higher your score climbs.

## Gameplay

- Move the player with the arrow keys or WASD.
- Avoid colliding with the falling objects.
- Survive as long as possible.
- Press Enter to restart after a game over.
- The game tracks the score and keeps a high score in a file.

## Features

- Smooth player movement with keyboard input
- Randomized falling object spawn positions and speeds
- Score counting over time
- High-score saving using a text file
- Restart support after losing

## Controls

- Up: W / Up Arrow
- Down: S / Down Arrow
- Left: A / Left Arrow
- Right: D / Right Arrow
- Restart: Enter

## Project Files

- `SpaceRaiders.cpp` — original version of the game
- `SpaceRaiders2.cpp` — improved version with score handling and high score persistence
- `README.md` — project overview

## Requirements

This project depends on:

- C++ compiler
- OpenGL
- GLFW
- GLM
- Project-specific headers such as `mesh.h`, `extraFunctions.h`, `meshDrawTools.h`, and `relativeResPath.h`

## Running the Game

1. Open the project in your preferred C++ IDE or build environment.
2. Make sure all required libraries and headers are available.
3. Compile and run either `SpaceRaiders.cpp` or `SpaceRaiders2.cpp`.
4. If you are using the improved version, the high score will be saved to a file in the resources folder.

## Notes

This repository contains the game source code and does not include a prebuilt executable or full project setup files. You may need to configure the include paths and library linking for your development environment.

## License

This project is provided as a learning/demo project and is not currently associated with a specific open-source license.
