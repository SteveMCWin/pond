![Cool banner :D](github_stuff/pondBanner.png?raw=true)

# Pond
My first OpenGL project.\
Pond is a modified boids simulation with procedural animation.

## Features
- Transparent background
- Fishies are always there to keep you company :D (fish always visible on screen, to close the simulation focus the window and press Esc)

![Example](github_stuff/screenshot.png?raw=true)

## Dependencies
Make sure you have the following installed before compiling:
- C++ 17+
- CMake 3.0.0+
- OpenGL 4.5+
- GLFW 3.4+
- glm

## Installation
### Ubuntu based systems
Clone the repo and make a build folder in which you will compile and run the project with cmake:
```
git clone --recursive https://github.com/SteveMCWin/pond.git
cd pond
mkdir build && cd build
cmake ..
cmake --build .
```
To run the project, run `./my_opengl_project` in the build folder.

## Credits

Project inspired by [this cool video](https://www.youtube.com/watch?v=qlfh_rv6khY&list=PL5UsWWvJwYu8X9gp48RIgwl9cWiGYbwXG&index=19).\
[Background](https://wallpapercat.com/black-wallpapers) used in screenshot.
