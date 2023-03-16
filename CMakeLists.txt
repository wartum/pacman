project(pacman)

Set(SOURCES
    src/pacman.c
    )

add_executable(pacman ${SOURCES})
target_link_libraries(pacman SDL2)