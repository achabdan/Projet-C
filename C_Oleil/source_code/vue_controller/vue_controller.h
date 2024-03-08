//
// Created by DUMOND on 25/09/2023.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
#  include "SDL.h"
#  include "../../SDL2_GFX/SDL2_framerate.h"
#  include "../../SDL2_GFX/SDL2_gfxPrimitives.h"
#  include "../../SDL2_GFX/SDL2_gfxPrimitives_font.h"
#  include "../../SDL2_GFX/SDL2_imageFilter.h"
#  include "../../SDL2_GFX/SDL2_rotozoom.h"
#else
#  include <SDL2/SDL.h>
#  include <SDL2/SDL2_gfxPrimitives.h>
#  include <SDL2/SDL2_framerate.h>
#endif

/// @brief Player infos
typedef struct Player_s {
    // Starting point coordinates
    struct {
        int cord_x;
        int cord_y;
    } start;
    // Ending point coordinates
    struct {
        int cord_x;
        int cord_y;
    } end;

    // Player coordinates
    struct {
        int cord_x;
        int cord_y;
    } self;
    // Player velocity
    struct {
        int velocity_x;
        int velocity_y;
    } velocity;
    // Player mass
    int mass;
} Player;

/// @brief Planet infos
typedef struct Planet_s {
    struct {
        int cord_x;
        int cord_y;
    }self;
    // Planet radius (=mass) (=time for a revolution)
    int radius;
    // Planet orbit (the sign indicate the direction of rotation)
    int orbit;
    // Planet angle
    float angle;
} Planet;


/// @brief Solar System infos
typedef struct SSystem_s {
    // Star coordinates
    struct {
        int cord_x;
        int cord_y;
    } self;

    // Star radius
    int radius;

    // Array of <nb_planet> planet(s)
    int nb_planet;
    Planet* planets;
} SSystem;

typedef struct Keys
{
    bool leftArrow;
    bool rightArrow;
    bool space;
    bool escape;
    bool vectors;
}keys;

int Render_All(SDL_Renderer* renderer, SSystem* SSYSTEM, Player* PLAYER, int Nb_sol, int SCREEN_WIDTH, int SCREEN_HEIGHT, keys* input);

void Key_Update(keys* self);
