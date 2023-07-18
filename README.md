# mktest
simple tool for faster c++ project and test on terminal

## purpose:
This tools is designed to create a fast c++ project on your terminal with your terminal text editor, by default NANO editor.
This tool take care of create your project, generate and manage Makefile, detecting compile arguments, compile code and execute it for your self.
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
- terminal base editor `nano`, `vim`, `emacs`, `vi`, `nvim`, etc..
- any template require you have installed the required libs itself.
- Make
- g++ or clang++.

![](https://i.ibb.co/R3NnhGq/mktest-Terminal-Example.png)

## testScript
this is a config language designed to get easy to configure custom development envitonment with mktest
- `linux`: /home/User/.config/mktest/config.conf
- `windows`: c:\\\Users\User\appdata\local\mktest\config.conf
```bash
environment SKIA[
	libs = "/home/Monsterduty/Frameworks/SKIA/lib"
	includes = "/home/Monsterduty/Frameworks/SKIA/include"
]

environment SDL2[
	libs = "/home/Monsterduty/Frameworks/SDL2-2.26.5/lib"
	includes = "/home/Monsterduty/Frameworks/SDL2-2.26.5/include"
]

default myConfig[
	editor = nvim
	compiler = g++
]
```
#### types and members:
`environment`: describe an environment.
- `libs`: paths to the static or dynamic libs ( .dll files on windows ).
- `includes`: paths to the headers files for those libs.
- `args`: custom compile arguments.
  
`defeault`: declare the default configuration
- `compiler`: the compiler by default to use.
- `editor`: the code editor to use.
- `args`: default arguments passed to your program on runtime.
