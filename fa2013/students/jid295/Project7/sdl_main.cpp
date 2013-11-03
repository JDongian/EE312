#include "Bugs.h"
#include "SDL/SDL.h"
#include <time.h>
#include <string>

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1366
#define SCREEN_BPP 32

#define COLOR_FOOD 0x11DD22
#define COLOR_BUG 0x000000
#define COLOR_EMPTY 0xEEEEEE

int world[WORLD_SIZE][WORLD_SIZE]; 
std::vector<Bug> bug_list;

void put_pixel(SDL_Surface *surface, int x, int y, int pixel) {
    int bpp = surface->format->BytesPerPixel;
    char *p = (char*)surface->pixels + y*surface->pitch + x*bpp;
    switch(bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xFF;
                p[1] = (pixel >> 8) & 0xFF;
                p[2] = pixel & 0xFF;
            } else {
                p[0] = pixel & 0xFF;
                p[1] = (pixel >> 8) & 0xFF;
                p[2] = (pixel >> 16) & 0xFF;
            }
            break;
        case 4:
            *(int *)p = pixel;
            break;
    }
}

int sdl_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) { return false; }
    if(SDL_EnableUNICODE(1) == -1) { return false; }
    SDL_WM_SetCaption("Bug's Life", NULL);
    return true;
}

SDL_Surface* reconfig(bool fullscreen, char zoom_level) {
    if (fullscreen) {
        //printf("Reconfiguring screen to fullscreen mode.\n");
        return SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                                SCREEN_BPP, SDL_FULLSCREEN);
    } else {
        //printf("Reconfiguring windowed screen to at %d zoom.\n",
        //        zoom_level);
        return SDL_SetVideoMode(WORLD_SIZE*zoom_level,
                                (WORLD_SIZE+42)*zoom_level,
                                SCREEN_BPP, SDL_SWSURFACE);
    }
}

void render_world(int world[WORLD_SIZE][WORLD_SIZE],
                  SDL_Surface* screen,
                  bool fullscreen,
                  int zoom_level) {
    if (fullscreen) {
        zoom_level = SCREEN_HEIGHT/WORLD_SIZE;
        int offset_x = SCREEN_WIDTH/2-WORLD_SIZE*zoom_level/2;
        int offset_y = 16;
        for (int x = 0; x < WORLD_SIZE; ++x) {
            for (int y = 0; y < WORLD_SIZE; ++y) {
                switch(world[x][y]) {
                    case EMPTY:
                        for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                            for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                                put_pixel(screen,
                                        x*zoom_level+sub_x+offset_x,
                                        y*zoom_level+sub_y+offset_y,
                                        COLOR_EMPTY);
                            }
                        }
                        break;
                    case FOOD:
                        for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                            for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                                put_pixel(screen,
                                        x*zoom_level+sub_x+offset_x,
                                        y*zoom_level+sub_y+offset_y,
                                        COLOR_FOOD);
                            }
                        }
                        break;
                    default:
                        for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                            for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                                put_pixel(screen,
                                        x*zoom_level+sub_x+offset_x,
                                        y*zoom_level+sub_y+offset_y,
                                        COLOR_BUG);
                            }
                        }
                        break;
                }
            }
        }
    } else {
        for (int x = 0; x < WORLD_SIZE; ++x) {
            for (int y = 0; y < WORLD_SIZE; ++y) {
                switch(world[x][y]) {
                    case EMPTY:
                        for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                            for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                                put_pixel(screen,
                                        x*zoom_level+sub_x,
                                        y*zoom_level+sub_y,
                                        COLOR_EMPTY);
                            }
                        }
                        break;
                    case FOOD:
                        for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                            for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                                put_pixel(screen,
                                        x*zoom_level+sub_x,
                                        y*zoom_level+sub_y,
                                        COLOR_FOOD);
                            }
                        }
                        break;
                    default:
                        for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                            for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                                put_pixel(screen,
                                        x*zoom_level+sub_x,
                                        y*zoom_level+sub_y,
                                        COLOR_BUG);
                            }
                        }
                        break;
                }
            }
        }
    }
}

int main(int argc, char* args[]) {
    SDL_Surface* screen = NULL;
    SDL_Event event;
    Uint8* keystates;
    bool quit = false;
    bool fullscreen = false;
    char zoom_level = 2;
    bool running = false;
    int tick_once = 0;
    int step_size = 1;
    int step = 0;
    int t_prev = 0;
    int FPS = 20;
    srand(time(NULL));
    /* Initialize the game world. */
    createWorld();
    /* Create the default screen. */
    screen = SDL_SetVideoMode(WORLD_SIZE*zoom_level,
            (WORLD_SIZE+42)*zoom_level,
            SCREEN_BPP,
            SDL_SWSURFACE);
    if (!screen) {
        printf("Failed initializing screen. Aborting.\n");
        return 1;
    }
    if (!sdl_init()) {
        printf("Failed initializing SDL. Aborting.\n");
        return 1;
    }
    /* Render 1 frame for aesthetics. */
    SDL_FillRect(screen, &screen->clip_rect,
                 SDL_MapRGB(screen->format, 0x77, 0x77, 0x77));
    render_world(world, screen, fullscreen, zoom_level);
    if (SDL_Flip(screen) == -1) {
        printf("Failed switching buffers. Aborting.");
        return 1;
    }
    while (!quit) {
        keystates = SDL_GetKeyState(NULL);
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                /* Hotkey: 'q' for quit. */
                if(keystates[SDLK_LEFT]) {
                    if (step_size > 1) {
                        --step_size;
                    }
                }
                if(keystates[SDLK_RIGHT]) {
                    if (step_size < 1024) {
                        ++step_size;
                    }
                }
                switch(event.key.keysym.sym) {
                    case SDLK_EQUALS:
                        if(keystates[SDLK_LSHIFT] ||
                           keystates[SDLK_RSHIFT]) {
                            if (zoom_level < 3) {
                                ++zoom_level;
                                if (!fullscreen) {
                                    screen = reconfig(fullscreen, zoom_level);
                                    tick_once = 1;
                                }
                            }
                        }
                        break;
                    case SDLK_MINUS:
                        if (zoom_level > 1) {
                            --zoom_level;
                            if (!fullscreen) {
                                screen = reconfig(fullscreen, zoom_level);
                                tick_once = 1;
                            }
                        }
                        break;
                    case SDLK_F12:
                        fullscreen = !fullscreen;
                        screen = reconfig(fullscreen, zoom_level);
                        tick_once = 1;
                        break;
                    case SDLK_SPACE:
                        running = !running;
                        break;
                    case SDLK_RETURN:
                        tick_once = 1;
                        if(keystates[SDLK_LSHIFT] ||
                                keystates[SDLK_RSHIFT]) {
                            tick_once = 1000;
                        }
                        break;
                    case SDLK_q:
                        printf("Quit received.\n");
                        quit = true;
                        break;
                    case SDLK_ESCAPE:
                        printf("Quit received.\n");
                        quit = true;
                        break;
                    default:
                        break;
                } 
            } else if (event.type == SDL_QUIT) {
                printf("Quit received.\n");
                quit = true;
            }
        }
        t_prev = SDL_GetTicks();
        SDL_FillRect(screen, &screen->clip_rect,
                     SDL_MapRGB(screen->format, 0x77, 0x77, 0x77));
        if (running) {
            for (step = step_size; step > 0; --step) {
                timeStep();
            }
            render_world(world, screen, fullscreen, zoom_level);
            if (SDL_Flip(screen) == -1) {
                printf("Failed switching buffers. Aborting.");
                return 1;
            }
        } else if (tick_once) {
            while (tick_once > 0) {
                --tick_once;
                timeStep();
            }
            render_world(world, screen, fullscreen, zoom_level);
            if (SDL_Flip(screen) == -1) {
                printf("Failed switching buffers. Aborting.");
                return 1;
            }
        }
        if(t_prev % 16 == 0) {
            printf("FPS: %d            \r",
                   (int)(1000.0/(SDL_GetTicks()-t_prev)));
            fflush(stdout);
        }
    }
    SDL_FreeSurface(screen);
    SDL_Quit();
    printf("Simulation ended.\n");
    return 0;
}
