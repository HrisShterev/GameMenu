# GameMenu

A C++ game built with SFML.

## Dependencies

- g++ with C++17 support
- SFML 2.6.1 (included in `lib/`)

## Building

> Make sure you build the game from the project root directory

Compile with:

```bash
g++ src/*.cpp -o game -std=c++17 -Ilib/include -Llib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -Wl,-rpath,'$ORIGIN/lib'
```

## Running

```bash
./game
```


