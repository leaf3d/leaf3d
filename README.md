# [![leaf3d](https://rawgit.com/leaf3d/leaf3d/master/logo.svg)](https://leaf3d.github.io)

A _lightweight_ **3D rendering engine** based on **modern OpenGL**.

[![Overview](https://rawgit.com/leaf3d/leaf3d.github.io/master/assets/images/overview.jpg)](https://leaf3d.github.io)

## Build

The project uses **CMake** for building:

https://cmake.org/

### Ubuntu (16.04 or later)

```
# Install dependencies

$ sudo apt install git build-essential cmake libpng12-dev

# Clone repo

$ git clone git@github.com:leaf3d/leaf3d.git

# Build project

$ cd leaf3d
$ mkdir Build
$ cmake ..
$ make
```

### Windows 10

On Windows, first of all you need to manually install these tools:

* Install [git](https://git-scm.com/download/win)
* Install [Visual Studio Build Tools 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=15)
* Install [CMake](https://cmake.org/files/v3.11/cmake-3.11.0-win64-x64.msi)

Then you can proceed with building:

```
# Clone repo

$ git clone git@github.com:leaf3d/leaf3d.git

# Build project

$ cd leaf3d
$ mkdir Build
$ cmake ..
$ nmake
```

## License

[MIT License] © Emanuele Bertoldi


[MIT License]: https://github.com/leaf3d/leaf3d/blob/master/LICENSE
