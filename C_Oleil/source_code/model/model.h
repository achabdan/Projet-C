//
// Created by DUMOND on 25/09/2023.
//

#include "../vue_controller/vue_controller.h"
/// @brief Parsing of the configuration file.
/// @param config Pointer on the opened configuration file.
/// @param WIN_SIZE_W Pointer on the window with variable.
/// @param WIN_SIZE_H Pointer on the window height variable.
/// @param NB_SOLAR_SYSTEM Pointer on the solar systems counter.
/// @param PLAYER_I Pointer on the player structure.
/// @param SSYSTEMS_I Pointer on the solar systems structure.
/// @return 0 if the file is well parsed, 1 otherwise.
int parseFile(FILE* config, int* WIN_SIZE_W, int* WIN_SIZE_H, int* NB_SOLAR_SYSTEM, Player** PLAYER_I, SSystem** SSYSTEMS_I);
int updateEntities(int SCREEN_WIDTH, int SCREEN_HEIGHT, int NB_SOLAR_SYSTEM, SSystem* solarSystem, Player* player, keys* input);
void Rotate_Velocity(Player* player, char direction);