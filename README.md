# mktest
simple tool for faster c++ project and test on terminal

![](https://i.ibb.co/R3NnhGq/mktest-Terminal-Example.png)

## purpose:
This tools is designed to create a fast c++ project on your terminal with your terminal text editor, by default NANO editor.

This tool take care of create your main function, generate and manage a Makefile, compile, and execute your program for you.

This tool generate templates from some c/c++ libraries and also generate a Makefile based on your project files.

## templates avaliable:
- sdl2
- sdl2_image
- imlib2
- curl
- xlib
- qt6

For more information type mkest --help.

## requirement to compile:
- a compiler that support c++17
- Make

## usage requirements:
- terminal base editor `nano`, `vim`, `emacs`, `vi`, `nvim`.
- any template require you have installed the lib itself.
- Make
- g++ or clang++.

## config file:
since mktest 3.0, a config language called testScript was added to mktest.\
sintax:
```bash
environment sdl2[
 libs = "/home/user/SDL2/lib"
 includes = "/home/user/SDL2/includes:/home/user/3partyLib/includes/SDL2"
 paths = "/home/user/SDL2/bin"
 args = "-lSDL2 -lSDL2main"
]

default myConfig[
  editor = nvim
  compiler = clang++
  args = "--testing"
]
```
#### default config file paths:
- Windows: `C:\Users\yourUser\appdata\roaming\mktest\config.conf`
- Linux: `$HOME/.config/mktest/config.conf`
- Android: `/data/data/com.yourApplication/files/usr/tmp`
#### testScript objects:
- environment
> This object allow to set up a custom environment for compile your project\
> under the hood this replaces environment variables like `LD_LIBRARY_PATH`,
> `LIBRARY_PATH`, `CPATH` and `PATH` to achive this goal.\
> Also receive args to pass directly to the compiler.
- default
> This object is the actual default config for mktest, with it, you can\
> decide which editor use, which compiler and it receive args to pass\
> to your already compiled program about to be executed.
