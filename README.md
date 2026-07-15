# GameMenu
A C++ game built with SFML.

## Dependencies
- CMake 3.16 or higher
- A C++17 compatible compiler (g++ on Linux, MSVC or MinGW on Windows)
- SFML 2.6.2


## Linux config

### Building

Make sure you are in the project root directory for all commands.

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
