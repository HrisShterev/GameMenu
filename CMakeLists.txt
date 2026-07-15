cmake_minimum_required(VERSION 3.16)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------
# 1. AUTOMATICALLY FETCH AND INSTALL SFML 2.6.2
# ---------------------------------------------------------
include(FetchContent)

# We force BUILD_SHARED_LIBS to OFF so SFML is built statically.
# This compiles SFML directly into your executable, meaning 
# you won't need to copy or distribute any SFML .dll files!
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.2
)

# Enable the specific SFML modules you need
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
set(SFML_BUILD_SYSTEM ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

# ---------------------------------------------------------
# 2. BUILD YOUR GAME
# ---------------------------------------------------------
file(GLOB SOURCES "src/*.cpp")

add_executable(Game ${SOURCES})

# Output the executable directly to the project root
set_target_properties(Game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# 3. LINK SFML
# FetchContent automatically handles all header include paths, 
# library paths, and dependencies for these targets.
target_link_libraries(Game
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
)
