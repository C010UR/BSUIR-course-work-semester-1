# Path finding algorithm comparison

## Overview

This project visually compares path finding algorithms in a maze.

https://github.com/C010UR/ncurses-maze-comparison/assets/95462776/64a4826c-2614-4cc3-bab8-a3c33bb00cdb

This project is built with
<img src="https://mesonbuild.com/assets/images/meson_logo.png" height="16px">
[Meson](https://mesonbuild.com/). To install meson you can visit their [GitHub page](https://github.com/mesonbuild/meson/tree/master/docs).

## Dependencies

- [ncurses](https://invisible-island.net/ncurses/announce.html)

## Compilation

To compile the project, first you need to build it using

```console
meson setup {{ build directory }}
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
./build/pathfinder-comparison.exe -h
```

![image](https://github.com/C010UR/ncurses-maze-comparison/assets/95462776/b2401872-7daf-46bb-a805-1be3475a6fc3)

To run a program you need to specify at least one of provided _pathfinder algorithms_ (They are marked as `[pathfinder]` in options list) and one of provided _maze generation algorithms_ (They are marked as `[maze generator]` in options list).
Example command

```console
.\pathfinder-comparison.exe -p -t 1 -d 0 --dijkstra --a-star --breadth-first-search --maze-block --maze-depth-first-search
```
