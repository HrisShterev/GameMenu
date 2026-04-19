# GameMenu

A C++ game built with SFML.

## Dependencies

- g++ with C++17 support
- SFML 2.6.1 (included in `lib/`)

## Building

Make sure the `lib/` folder contains the following files:

```
lib/
├── libsfml-graphics.so.2.6.1
├── libsfml-window.so.2.6.1
├── libsfml-system.so.2.6.1
├── libsfml-audio.so.2.6.1
└── libsfml-network.so.2.6.1
```

Then compile with:

```bash
g++ src/*.cpp -o Game -std=c++17 -Isfml/include -Llib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -Wl,-rpath,'$ORIGIN/lib'
```

## Running

```bash
./Game
```

> Make sure to run the game from the project root directory so it can find the `assets/` folder.

## Project Structure

```
GameMenu/
├── Game              ← compiled executable
├── assets/           ← game assets (textures, fonts, config)
├── src/              ← source files
├── sfml/
│   └── include/      ← SFML headers
└── lib/              ← SFML shared libraries
```
