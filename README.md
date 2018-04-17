# ![leaf3d](https://rawgit.com/leaf3d/leaf3d/master/logo.svg 'leaf3d')

A _lightweight_ **rendering engine** based on **modern OpenGL**.

![Overview](http://leaf3d.github.io/website/images/l3d_overview.png 'leaf3d')

## Build

The project uses **CMake** for building:

https://cmake.org/

### Ubuntu (16.04 or later)

```
# Install dependencies

$ sudo apt install git build-essential cmake libassimp-dev libglfw3-dev libpng12-dev

# Clone repo

$ git clone git@github.com:leaf3d/leaf3d.git

# Build project

$ cd leaf3d
$ mkdir Build
$ cmake ..
$ make
```

### Windows 10

* Install [git](https://git-scm.com/download/win)
* Install [Visual Studio Build Tools 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=15)
* Install [CMake](https://cmake.org/files/v3.11/cmake-3.11.0-win64-x64.msi)
* Install [Assimp SDK](https://github.com/assimp/assimp/releases/download/v3.3.1/assimp-sdk-3.3.1-setup_vs2017.exe)
* Install [GLFW SDK](https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.bin.WIN64.zip)

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
