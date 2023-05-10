# Path finding algorithm comparison

## Overview

This project visually compares path finding algorithms in a maze.

https://github.com/C010UR/ncurses-maze-comparison/assets/95462776/9da0b87c-7bf3-495b-889a-c6ab98bdc6c7

https://github.com/C010UR/ncurses-maze-comparison/assets/95462776/4756e5e9-9237-438f-aa1f-66cf725434aa

This project is built with
<img src="https://mesonbuild.com/assets/images/meson_logo.png" height="16px">
[Meson](https://mesonbuild.com/). To install meson you can visit their [GitHub page](https://github.com/mesonbuild/meson/tree/master/docs).

## Dependencies

- [ncurses](https://invisible-island.net/ncurses/announce.html)

## Compilation

To compile the project, first you need to build it using

```console
meson build
```

If you want to build for relase, add `--buildtype=release` flag.<br>
You can compile the project using

```console
meson compile -C {{ build directory }}
```

After compilation a program will be stored in `build` directory

## Running

To see help message, add `-h` or `--help` flag

```console
./pathfinder-comparison.exe -h
```

To run a program you need to specify at least one of provided pathfinder algorithms (They are marked `[PATHFINDER]` in options list).
Example command

```console
./pathfinder-comparison -t 100 -d 0 -p --breadth-first-search --dijkstra --a-star
```
