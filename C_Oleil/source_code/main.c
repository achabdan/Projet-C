#include "model/model.h"

//gcc main.c ./model/model.c ./vue_controller/vue_controller.c -o ../test.out -I/Library/Frameworks/SDL2.framework/Headers -lSDL2

int main(int argc, char* argv[]) {
    srand(time(NULL));
    // Check the filename argument is missing.
    if (argc < 2) {
        printf("ERROR: Missing argument");
        return EXIT_FAILURE;
    }

    int WIN_SIZE_W = -1;
    int WIN_SIZE_H = -1;
    int NB_SOLAR_SYSTEM = -1;
    Player* PLAYER = NULL;
    SSystem* SSYSTEMS = NULL;
    // Open the config file.
    FILE* config = fopen(argv[1], "r");
    // Check if the config file il well opened.
    if (!config) {
        printf("ERROR: Can't open file");
        return EXIT_FAILURE;
    }

    // Parse the config file and check if it's well parsed.
    if (parseFile(config, &WIN_SIZE_W, &WIN_SIZE_H, &NB_SOLAR_SYSTEM, &PLAYER, &SSYSTEMS)) {
        printf("ERROR: Wrong config file format");
        fclose(config);
        return EXIT_FAILURE;
    }
    fclose(config);


    // Initialition of the SDL2 lib.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("ERROR - SDL_Init %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }


    // Creation of the rendering window.
    SDL_Window* window = SDL_CreateWindow("Basecode SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_SIZE_W, WIN_SIZE_H, 0);
    if (!window) {
        printf("ERROR - Create window %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Creation of the renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, WIN_SIZE_W, WIN_SIZE_H);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    if (!renderer) {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create the user inputs recorder.
    keys* input = (keys*)calloc(1, sizeof(keys));
    if (!input) return EXIT_FAILURE;
    input->vectors = false;

    // Initialisation of the player position
    PLAYER->self.cord_x = PLAYER->start.cord_x;
    PLAYER->self.cord_y = PLAYER->start.cord_y;
    FPSmanager* fps=malloc(sizeof(FPSmanager));
    SDL_initFramerate(fps);
    SDL_setFramerate(fps, 60);
    int score = 0;

    //end variables
    bool end = false;
    int timoutend = 90;

    //mainloop
    while (true) {
        if (!end) {
            Key_Update(input);

            if (input->escape) break;
            int f = SDL_getFramerate(fps);
            if ((input->leftArrow) || (input->rightArrow)) score++;
            char name[256] = { 0 };
            sprintf(name, "ProjetC_oleil | FPS : %d.0 | Score : %d", f, score);
            SDL_SetWindowTitle(window, name);
            if (updateEntities(WIN_SIZE_W, WIN_SIZE_H, NB_SOLAR_SYSTEM, SSYSTEMS, PLAYER, input) == 2) {
                end = true;
            }
        }
        else {
            if (!timoutend--) break;
        }
        Render_All(renderer, SSYSTEMS, PLAYER, NB_SOLAR_SYSTEM, WIN_SIZE_W, WIN_SIZE_H, input);
        SDL_RenderPresent(renderer);
    }

    free(input);
    free(PLAYER);
    /*
    for (int i = 0; i < NB_SOLAR_SYSTEM; ++i) {
        free(SSYSTEMS[i].planets);
    }*/
    free(SSYSTEMS);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
