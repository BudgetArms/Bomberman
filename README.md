# Bomberman

[![Build Status](https://github.com/BudgetArms/Bomberman/actions/workflows/cmake.yml/badge.svg)](https://github.com/BudgetArms/cmake/actions)
[![Build Status](https://github.com/BudgetArms/Bomberman/actions/workflows/emscripten.yml/badge.svg)](https://github.com/BudgetArms/emscripten/actions)


An NES Bomberman (1987) clone. <br>

Game Engine: [BudgetArmsEngine](https://github.com/BudgetArms/BudgetArmsEngine)<br>


## Design patterns

* Dirty flags
* Observers & subjects
* Event Queues (uses enum class that has to be modified in game engine, bad design)
* Sound Queues (runs on a different thread)
* Component system (each game object can have 1 of same type component)

## Features to be implemented

Visit the [Bomberman Project](https://github.com/users/BudgetArms/projects/7/) page

## How To Build Emscripten version (web)

### On Windows

The required tools

    Emscripten
    CMake
    Ninja
    Python

I used `Chocolatey` to install the required tools. You can also manually install each of the tools. <br>
I installed the latest [Chocolatey release](https://github.com/chocolatey/choco/releases), and used the following
commands:

    choco install -y cmake
    choco install -y ninja
    choco install -y python

P.S.: For me, after running a command, Chocolatey seemed to be stuck for me, pressing enter a few times seemed to fix
the problem. <br>

Chocolatey unfortunately has only support for Emscripten 3.1.40, I want the latest version of Emscripten.
Create a directory `EmscriptenSDK` in a location where it won't be deleted, and run the following commands.

    git clone https://github.com/emscripten-core/emsdk.git
    cd emsdk
    emsdk.bat install latest
    emsdk.bat activate latest --global

Then create a directory `build_web` in the root folder of your project. <br>
Open the terminal in the `build_web` folder, and run:

    emcmake cmake ..
    emmake ninja

To see the webpage, start it with a python webserver by running the following command in the `build_web` folder

    python -m http.server

Then browse to http://localhost:8000 and you're good to go. <br>
If you don't see anything, check the `Console` of your browser for any errors.
