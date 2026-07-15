# GameMenu
A C++ game built with SFML.

## Dependencies
- CMake 3.16 or higher
- A C++17 compatible compiler (g++ on Linux, MSVC or MinGW on Windows)
- SFML 2.6.2


## Linux config

### Prerequisites

Because this project automatically downloads and builds SFML from source, Linux users must install a few native development packages (such as X11, OpenGL, FreeType, and Udev) before compiling.

On **Ubuntu / Debian / Mint / WSL**, install these packages by running the following command in your terminal:

```bash
sudo apt update && sudo apt install -y \
  libudev-dev \
  libopenal-dev \
  libvorbis-dev \
  libflac-dev \
  libgl1-mesa-dev \
  libegl1-mesa-dev \
  libxrandr-dev \
  libxcursor-dev \
  libxinerama-dev \
  libxi-dev \
  libfreetype-dev
```

### Building

Make sure you are in the project root directory for these commands.

```bash
#intall cmake if not already installed
sudo apt install cmake

cmake -B build
cmake --build build
```

### Running

```bash
./Game
```


## Windows config

### Building

```bash
# Install CMake from https://cmake.org/download/ if not already installed

cmake -B build
cmake --build build --config Release
```

### Running
```bash
.\Release\Game.exe
```
