# GameMenu
A C++ game built with SFML.

## Dependencies
- CMake 3.16 or higher
- A C++17 compatible compiler (g++ on Linux, MSVC or MinGW on Windows)
- SFML (Linux only, installed via apt)


## Linux config

### Building

Make sure you are in the project root directory for all commands.

```bash
#intall cmake and sfml if not already installed
sudo apt install cmake libsfml-dev -y

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
# Install CMake if not already installed
choco install cmake

cmake -B build
cmake --build build --config Release
```

### Running
```bash
.\Release\Game.exe
```
