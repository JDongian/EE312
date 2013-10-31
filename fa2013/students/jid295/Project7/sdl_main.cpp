#include "Bugs.h"
#include "SDL/SDL.h"
#include <string>

#define WIDTH 768
#define SCREEN_BPP 32

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
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
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
    if (fullscreen) { zoom_level = 2; }
    printf("Reconfiguring screen to %s at %d zoom.\n",
           (fullscreen ? "fullscreen" : "windowed"), zoom_level);
    return SDL_SetVideoMode(WORLD_SIZE*zoom_level,
                            (WORLD_SIZE+64)*zoom_level,
                            SCREEN_BPP,
                            (fullscreen ? SDL_FULLSCREEN : SDL_SWSURFACE));
}

void render_world(int world[WORLD_SIZE][WORLD_SIZE],
                  SDL_Surface* screen,
                  bool fullscreen,
                  int zoom_level) {
    for (int x = 0; x < WORLD_SIZE; ++x) {
        for (int y = 0; y < WORLD_SIZE; ++y) {
            switch(world[x][y]) {
                case EMPTY:
                    break;
                case FOOD:
                    for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                        for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                            put_pixel(screen,
                                      x*zoom_level+sub_x,
                                      y*zoom_level+sub_y,
                                      0x00FF00);
                        }
                    }
                    break;
                default:
                    for (int sub_x = 0; sub_x<zoom_level; ++sub_x) {
                        for (int sub_y = 0; sub_y<zoom_level; ++sub_y) {
                            put_pixel(screen,
                                      x*zoom_level+sub_x,
                                      y*zoom_level+sub_y,
                                      0x000000);
                        }
                    }
                    break;
            }
        }
    }
}

int main(int argc, char* args[]) {
    SDL_Surface* screen = NULL;
    SDL_Event event;
    bool quit = false;
    bool fullscreen = false;
    char zoom_level = 2;
    bool running = true;
    //createWorld();
    screen = SDL_SetVideoMode(WORLD_SIZE*zoom_level,
                              (WORLD_SIZE+64)*zoom_level,
                              SCREEN_BPP,
                              (fullscreen ? SDL_FULLSCREEN : SDL_SWSURFACE));
    if (!screen) {
        printf("Failed initializing screen. Aborting.\n");
        return 1;
    }
    if (!sdl_init()) {
        printf("Failed initializing SDL. Aborting.\n");
        return 1;
    }
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                /* Hotkey: 'q' for quit. */
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        if (zoom_level < 8) {
                            ++zoom_level;
                            screen = reconfig(fullscreen, zoom_level);
                        }
                        break;
                    case SDLK_DOWN:
                        if (zoom_level > 1) {
                            --zoom_level;
                            screen = reconfig(fullscreen, zoom_level);
                        }
                        break;
                    case SDLK_F12:
                        fullscreen = !fullscreen;
                        screen = reconfig(fullscreen, fullscreen);
                        break;
                    case SDLK_SPACE:
                        running = !running;
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
        SDL_FillRect(screen, &screen->clip_rect,
                     SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));
        render_world(world, screen, fullscreen, zoom_level);
        if (running) {
        //timeStep();
        }
        if (SDL_Flip(screen) == -1) {
            printf("Failed switching buffers. Aborting.");
            return 1;
        }
    }
    SDL_FreeSurface(screen);
    SDL_Quit();
    printf("Simulation ended.\n");
    return 0;
}
