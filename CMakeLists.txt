cmake_minimum_required(VERSION 3.16)
project(Game)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/lib/include)

if(WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/lib/windows)
endif()

file(GLOB SOURCES "src/*.cpp")

add_executable(Game ${SOURCES})

set_target_properties(Game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

target_link_libraries(Game
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
)

if(WIN32)
    file(GLOB SFML_DLLS "${CMAKE_SOURCE_DIR}/lib/windows/*.dll")
    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLLS}
        $<TARGET_FILE_DIR:Game>
    )
endif()
