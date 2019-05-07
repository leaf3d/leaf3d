# [![leaf3d](https://rawgit.com/leaf3d/leaf3d/master/logo.svg)](https://leaf3d.github.io)

A _lightweight_ **3D rendering engine** based on **modern OpenGL**.

[![Overview](https://rawgit.com/leaf3d/leaf3d.github.io/master/assets/images/overview.jpg)](https://leaf3d.github.io)

## Requirements

The project uses **CMake** for building:

https://cmake.org/

### Ubuntu (16.04 or later)

```bash
$ sudo apt install git build-essential cmake xorg-dev libgl1-mesa-dev
```

### MacOSX

```bash
$ brew install cmake
```

### Windows 10

You need to manually install these tools:

* [git](https://git-scm.com/download/win)
* [Visual Studio Build Tools 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=15)
* [CMake](https://cmake.org/files/v3.11/cmake-3.11.0-win64-x64.msi)

Now open a **Developer Command Prompt for VS 2017** and you are ready to build the project.

## Building

```bash
# Clone repo
$ git clone git@github.com:leaf3d/leaf3d.git

# Build project
$ cd leaf3d
$ mkdir Build
$ cd Build
$ cmake ..
$ cmake --build .
```

## License

[MIT License] © Emanuele Bertoldi

[mit license]: https://github.com/leaf3d/leaf3d/blob/master/LICENSE
