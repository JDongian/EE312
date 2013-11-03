#include "Bugs.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
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

TTF_Font *font = NULL;

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

void apply_surface(int x, int y,
                   SDL_Surface* source, SDL_Surface* destination) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, destination, &offset);
}

void render_text(int x, int y, SDL_Surface *surface,
                 TTF_Font* font, char buff[], SDL_Color c) {
    int lineskip = TTF_FontLineSkip(font);
    int line = 0;
    int i = 0;
    char* begin = buff;
    while (buff[i]) {
        if (buff[i] == '\n') {
            buff[i++] = '\0';
            apply_surface(x, y + line++*lineskip,
                          TTF_RenderText_Blended(font, begin, c),
                          surface);         
            while(buff[i] == '\n') {
                ++i;
            }
            begin = buff+i;
        }
        ++i;
    }
    apply_surface(x, y + line++*lineskip,
                  TTF_RenderText_Blended(font, begin, c),
                  surface);
}

int sdl_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) { return false; }
    SDL_WM_SetCaption("Bug's Life", NULL);
    return true;
}

TTF_Font* ttf_init() {
    if(TTF_Init() == -1) { return NULL; }  
    if(SDL_EnableUNICODE(1) == -1) { return NULL; }
    return TTF_OpenFont("Ubuntu-L.ttf", 12);
}

SDL_Surface* config(bool fullscreen, char zoom_level) {
    if (fullscreen) {
        //printf("configuring screen to fullscreen mode.\n");
        return SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                                SCREEN_BPP, SDL_FULLSCREEN);
    } else {
        //printf("configuring windowed screen to at %d zoom.\n",
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

void tick(unsigned int n) {
    for (int i = n; i > 0; --i) {
        timeStep();
    }
}

int main(int argc, char* args[]) {
    SDL_Surface* screen = NULL;
    TTF_Font *font = NULL;
    SDL_Event event;
    Uint8* keystates;
    bool quit = false;
    bool fullscreen = false;
    char zoom_level = 2;
    bool running = false;
    int tick_once = 0;
    int step_size = 1;
    int t_prev = 0;
    bool cap = true;
    char run_stats[64];
    char world_stats[256];
    unsigned int FPS_CAP = 60;
    double measured_fps = 0;

    /* Seed random to time. */
    srand(time(NULL));
    /* Initialize the game world. */
    createWorld();
    /* Create the default screen. */
    screen = config(fullscreen, zoom_level);
    if (!screen) {
        printf("Failed initializing screen. Aborting.\n");
        return 1;
    }
    /* Initialize the SDL window and title. */
    if (!sdl_init()) {
        printf("Failed initializing SDL. Aborting.\n");
        return 1;
    }
    /* Load the font and SDL_ttf. */
    if (!(font = ttf_init())) {
        printf("Failed initializing TTF. Aborting.\n");
        return 1;
    }
    /* Render and display one frame for aesthetics. */
    SDL_FillRect(screen, &screen->clip_rect,
                 SDL_MapRGB(screen->format, 0x77, 0x77, 0x77));
    render_world(world, screen, fullscreen, zoom_level);
    if (SDL_Flip(screen) == -1) {
        printf("Failed switching buffers. Aborting.");
        return 1;
    }
    /* Simulation loop. */
    while (!quit) {
        /* Event handler */
        keystates = SDL_GetKeyState(NULL);
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
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    /* Hotkey: '+' for expand window. */
                    case SDLK_EQUALS:
                        /* Only change window size if not fullscreen. */
                        if (!fullscreen) {
                            if(keystates[SDLK_LSHIFT] ||
                               keystates[SDLK_RSHIFT]) {
                                if (zoom_level < 4) {
                                    ++zoom_level;
                                    screen = config(fullscreen,
                                                      zoom_level);
                                    tick_once = 1;
                                }
                            }
                        }
                        break;
                    /* Hotkey: '-' to contract window. */
                    case SDLK_MINUS:
                        /* Only change window size if not fullscreen. */
                        if (!fullscreen) {
                            if (zoom_level > 1) {
                                --zoom_level;
                                screen = config(fullscreen, zoom_level);
                                tick_once = 1;
                            }
                        }
                        break;
                    /* Hotkey: <F12> to toggle fullscreen. */
                    case SDLK_F12:
                        fullscreen = !fullscreen;
                        screen = config(fullscreen, zoom_level);
                        tick_once = 1;
                        break;
                    /* Hotkey: <SPACE> to pause/unpause. */
                    case SDLK_SPACE:
                        running = !running;
                        break;
                    /* Hotkey: <ENTER> to tick once.
                     * Hotkey: <CTRL>+<ENTER> to tick 1000.*/
                    case SDLK_RETURN:
                        tick_once = 1;
                        if(keystates[SDLK_LSHIFT] ||
                           keystates[SDLK_RSHIFT]) {
                            tick_once = 1000;
                        } else if(keystates[SDLK_LCTRL] ||
                                  keystates[SDLK_RCTRL]) {
                            tick_once = 100000;
                        }
                        break;
                    /* Hotkey: 'q' to quit. */
                    case SDLK_q:
                        printf("Quit received.\n");
                        quit = true;
                        break;
                    /* Hotkey: <ESC> to quit. */
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
        /* Set time before render to t_prev. */
        t_prev = SDL_GetTicks();
        SDL_FillRect(screen, &screen->clip_rect,
                     SDL_MapRGB(screen->format, 0x77, 0x77, 0x77));
        if (running) {
            tick(step_size);
        } else if (tick_once > 0) {
            tick(tick_once);
            tick_once = -1;
        }
        if (running || tick_once) {
            render_world(world, screen, fullscreen, zoom_level);
            /* Render running statistics at the bottom. */
            sprintf(run_stats, "FPS: %d, STEP: %d",
                    (int)measured_fps, step_size);
            render_text((fullscreen ?
                            SCREEN_WIDTH/2 :
                            WORLD_SIZE*zoom_level/2),
                        (fullscreen ?
                            WORLD_SIZE*(SCREEN_HEIGHT/WORLD_SIZE)+16 :
                            WORLD_SIZE*zoom_level+3),
                        screen, font, run_stats, {0x00, 0x00, 0x00});
            sprintf(world_stats, "Time: %d\n"
                                 "Pop: %d\n"
                                 "Average age: %d\n"
                                 "Average generation: %d",
                    time_step, (int)bug_list.size(),
                    average_age, average_generation);
            render_text((fullscreen ?
                            (SCREEN_WIDTH-
                             WORLD_SIZE*(SCREEN_HEIGHT/WORLD_SIZE))/2 + 6:
                            6),
                        (fullscreen ?
                            WORLD_SIZE*(SCREEN_HEIGHT/WORLD_SIZE)+16 :
                            WORLD_SIZE*zoom_level+3), screen,
                        font, world_stats, {0x00, 0x00, 0x00});
            if (SDL_Flip(screen) == -1) {
                printf("Failed switching buffers. Aborting.");
                return 1;
            }
        }
        /* Frame rate limit */
        if((cap == true) && (SDL_GetTicks()-t_prev < 1000/FPS_CAP)) {
            SDL_Delay((1000/FPS_CAP) - (SDL_GetTicks()-t_prev));
        }
        measured_fps = 1000./(SDL_GetTicks()-t_prev);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_FreeSurface(screen);
    SDL_Quit();
    printf("Simulation ended.\n");
    return 0;
}
