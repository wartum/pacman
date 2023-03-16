#include <stdio.h>
#include <SDL2/SDL.h>

#define FPS_CAP 32

SDL_Window *game_window = NULL;
uint8_t fps_counter = 0;

uint8_t initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL. %s\n", SDL_GetError());
        return 0;
    }

    game_window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (game_window == NULL) {
        printf("Could not create window. %s\n", SDL_GetError());
        return 0;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(game_window);
    SDL_UpdateWindowSurface(game_window);

    return 1;
}

void cleanup() {
    SDL_DestroyWindow(game_window);
}

void toggle_rectangle() {
    static uint8_t f = 0;
    static SDL_Rect rect = {50, 50, 200, 200};
    SDL_Surface *surface = SDL_GetWindowSurface(game_window);

    if (f == 0) {
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0));
    } else {
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 255, 0));
    }
    f = !f;
}

void draw_frame() {
    static uint32_t next_frame_countdown = 0;
    
    if (next_frame_countdown <= SDL_GetTicks()) {
        next_frame_countdown = SDL_GetTicks() + (1000 / FPS_CAP);
        if (SDL_UpdateWindowSurface(game_window) < 0) {
            printf("Updating window surface failed. %s\n", SDL_GetError());
        }
        fps_counter += 1;
    }
}

void calculate_fps() {
    static uint32_t fps_calculation_countdown = 0;
    
    if (fps_calculation_countdown <= SDL_GetTicks()) {
        printf("Current fps:%d\n", fps_counter);
        fps_calculation_countdown = SDL_GetTicks() + 1000;
        fps_counter = 0;
    }
}

void main_loop() {
    SDL_Event e;
    uint8_t quit_game = 0;

    while (quit_game == 0) {
        // Handle all queued events
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit_game = 1;
                    break;
                case SDL_KEYDOWN:
                    toggle_rectangle();
                    break;
            }
        }

        // Handle physics

        // Handle display
        draw_frame();
        calculate_fps();

        // Don't waste CPU cycles
        SDL_Delay(1);
    }
}

int main(void) {
    if (!initialize()) {
        printf("Initialization failed. Exitting\n");
        cleanup();
        return -1;
    }
    main_loop();    
    cleanup();
    return 0;
}