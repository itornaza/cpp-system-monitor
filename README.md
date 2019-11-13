# Linux System Monitor

A system monitor for Linux developed in C++ 17 like the htop utility based on the ncurses display.

This implementation is based on the starter code for System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

Follow along with the classroom lesson to complete the project!

![System Monitor](images/monitor.png)

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

You can install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## How to build
1. Clone the project repository: `git clone https://github.com/itornaza/cpp-system-monitor.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## System information
System information for the process manager is derived from the following system files:

1. Kernel information - `/proc/version`
2. Operating system - `/etc/os-release`
3. Memory utilization - `/proc/meminfo`
4. Total processes - `/proc/meminfo` 
5. Running processes - `/proc/meminfo`
6. Up time - `/proc/uptime`

