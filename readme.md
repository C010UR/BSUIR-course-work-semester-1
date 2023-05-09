# Path finding algorithm comparison

## Overview

This project shows the difference between 2 very popular path finding algorithms: __A* and Dijkstra__

This project is built with
<img src="https://mesonbuild.com/assets/images/meson_logo.png" height="16px">
[Meson](https://mesonbuild.com/). To install meson you can visit their [GitHub page](https://github.com/mesonbuild/meson/tree/master/docs).

## Dependencies

- [ncurses](https://invisible-island.net/ncurses/announce.html)

## Compilation

To compile the project, first you need to build it using

```console
meson build ./build
```

If you want to build for relase, add `--buildtype=release` flag.<br>
You can compile the project using

```console
meson compile -C build
```

After compilation a program will be stored in `build` directory
