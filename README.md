# GameMenu
A C++ game built with SFML.

## Dependencies
- CMake 3.16 or higher
- A C++17 compatible compiler (g++ on Linux, MSVC or MinGW on Windows)
- SFML (Linux only, installed via apt)

## Building

Make sure you are in the project root directory for all commands.

### Linux
```bash
sudo apt install cmake libsfml-dev -y
cmake -B build
cmake --build build
```

### Windows
```bash
cmake -B build
cmake --build build --config Release
```

## Running

After building, the executable will be in the project root on Linux, and in `Release\` on Windows.

### Linux
```bash
./Game
```

### Windows
```bash
.\Release\Game.exe
```
