//
// Created by DUMOND on 25/09/2023.
//

#include "model.h"
int parseFile(FILE* config, int* WIN_SIZE_W, int* WIN_SIZE_H, int* NB_SOLAR_SYSTEM, Player** PLAYER_I, SSystem** SSYSTEMS_I) {
    char bin[128];

    fscanf(config, "%s %d %d", bin, WIN_SIZE_W, WIN_SIZE_H);
    if (strcmp(bin, "WIN_SIZE")) return EXIT_FAILURE;
    if (*WIN_SIZE_W < 0 || *WIN_SIZE_H < 0) return EXIT_FAILURE;

    Player* PLAYER = calloc(1, sizeof(Player));
    if (!PLAYER) return EXIT_FAILURE;

    fscanf(config, "%s %d %d", bin, &PLAYER->start.cord_x, &PLAYER->start.cord_y);
    if (strcmp(bin, "START")) return EXIT_FAILURE;
    if (PLAYER->start.cord_x < 0 || PLAYER->start.cord_x > *WIN_SIZE_W) return EXIT_FAILURE;
    if (PLAYER->start.cord_y < 0 || PLAYER->start.cord_y > *WIN_SIZE_H) return EXIT_FAILURE;
    fscanf(config, "%s %d %d", bin, &PLAYER->end.cord_x, &PLAYER->end.cord_y);
    if (strcmp(bin, "END")) return EXIT_FAILURE;
    if (PLAYER->end.cord_x < 0 || PLAYER->end.cord_x > *WIN_SIZE_W) return EXIT_FAILURE;
    if (PLAYER->end.cord_y < 0 || PLAYER->end.cord_y > *WIN_SIZE_H) return EXIT_FAILURE;
    PLAYER->mass = 2;

    fscanf(config, "%s %d", bin, NB_SOLAR_SYSTEM);
    if (strcmp(bin, "NB_SOLAR_SYSTEM")) return EXIT_FAILURE;
    if (*NB_SOLAR_SYSTEM < 0) return EXIT_FAILURE;
    SSystem* SSYSTEMS = calloc(*NB_SOLAR_SYSTEM, sizeof(SSystem));
    if (!SSYSTEMS) return EXIT_FAILURE;
    for (int i = 0; i < *NB_SOLAR_SYSTEM; ++i) {
        fscanf(config, "%s %d %d", bin, &SSYSTEMS[i].self.cord_x, &SSYSTEMS[i].self.cord_y);
        if (strcmp(bin, "STAR_POS")) return EXIT_FAILURE;
        if (SSYSTEMS[i].self.cord_x < 0 || SSYSTEMS[i].self.cord_x > *WIN_SIZE_W) return EXIT_FAILURE;
        if (SSYSTEMS[i].self.cord_y < 0 || SSYSTEMS[i].self.cord_y > *WIN_SIZE_H) return EXIT_FAILURE;

        fscanf(config, "%s %d", bin, &SSYSTEMS[i].radius);
        if (strcmp(bin, "STAR_RADIUS")) return EXIT_FAILURE;
        if (SSYSTEMS[i].radius < 1) return EXIT_FAILURE;
        if (SSYSTEMS[i].self.cord_x - SSYSTEMS[i].radius < 0 || SSYSTEMS[i].self.cord_x + SSYSTEMS[i].radius > *WIN_SIZE_W) return EXIT_FAILURE;
        if (SSYSTEMS[i].self.cord_y - SSYSTEMS[i].radius < 0 || SSYSTEMS[i].self.cord_y + SSYSTEMS[i].radius > *WIN_SIZE_H) return EXIT_FAILURE;

        fscanf(config, "%s %d", bin, &SSYSTEMS[i].nb_planet);
        if (strcmp(bin, "NB_PLANET")) return EXIT_FAILURE;
        if (SSYSTEMS->nb_planet < 0) return EXIT_FAILURE;

        SSYSTEMS[i].planets = calloc(SSYSTEMS[i].nb_planet, sizeof(Planet));
        if (!SSYSTEMS[i].planets) return EXIT_FAILURE;

        for (int j = 0; j < SSYSTEMS[i].nb_planet; ++j) {
            fscanf(config, "%s %d", bin, &SSYSTEMS[i].planets[j].radius);
            if (strcmp(bin, "PLANET_RADIUS")) return EXIT_FAILURE;
            if (SSYSTEMS[i].planets[j].radius < 1) return EXIT_FAILURE;
            if (SSYSTEMS[i].self.cord_x - SSYSTEMS[i].planets[j].orbit - SSYSTEMS[i].planets[j].radius < 0 || SSYSTEMS[i].self.cord_x + SSYSTEMS[i].planets[j].orbit + SSYSTEMS[i].planets[j].radius > *WIN_SIZE_W) return EXIT_FAILURE;
            if (SSYSTEMS[i].self.cord_y - SSYSTEMS[i].planets[j].orbit - SSYSTEMS[i].planets[j].radius < 0 || SSYSTEMS[i].self.cord_y + SSYSTEMS[i].planets[j].orbit + SSYSTEMS[i].planets[j].radius > *WIN_SIZE_H) return EXIT_FAILURE;

            fscanf(config, "%s %d", bin, &SSYSTEMS[i].planets[j].orbit);
            if (strcmp(bin, "PLANET_ORBIT")) return EXIT_FAILURE;
        }
    }
    if (!feof(config)) return EXIT_FAILURE;
    *PLAYER_I = PLAYER;
    *SSYSTEMS_I = SSYSTEMS;
    return EXIT_SUCCESS;
}
double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void Rotate_Velocity(Player * player,char direction) {
    //pass in circular coordonate from carthesian ones
    double teta, r;
    teta = atan((double)player->velocity.velocity_y / (double)player->velocity.velocity_x);
    r = sqrt(player->velocity.velocity_y * player->velocity.velocity_y + player->velocity.velocity_x * player->velocity.velocity_x);
    //handle the direction of rotation
    switch (direction)
    {
    case -1:
        teta -= M_PI / 2;
        break;
    case 1:
        teta += M_PI / 2;
        break;
    default:
        break;
    }

    //pass in carthesian coordonate from circular ones
    player->velocity.velocity_x = r * cos(teta);
    player->velocity.velocity_y = r * sin(teta);

}
int updateEntities(int SCREEN_WIDTH, int SCREEN_HEIGHT, int NB_SOLAR_SYSTEM, SSystem* solarSystem, Player* player, keys* input) {
    double accelx = 0, accely = 0;
    for (int j = 0; j < NB_SOLAR_SYSTEM; j++) {
        // Update planet positions based on their orbits
        for (int i = 0; i < solarSystem->nb_planet; i++) {
            Planet* planet = &(solarSystem[j].planets[i]);
            double angle = planet->angle; // Angle in radians, initialize it according to your logic
            double radius = planet->orbit; // Orbit radius

            // Calculate the new x and y coordinates of the planet
            // Utilize the correct members: planet->radius instead of planet->cord_x and planet->orbit instead of planet->cord_y
            int new_y;
            int new_x = solarSystem[j].self.cord_x + (int)(radius * cos(angle));
            if (planet->orbit < 0) {

                new_y = solarSystem[j].self.cord_y + (int)(radius * sin(-angle));
            }
            else {

                new_y = solarSystem[j].self.cord_y + (int)(radius * sin(angle));
            }
            // Update the planet's coordinates with the new values
            planet->self.cord_x = new_x;
            planet->self.cord_y = new_y;
            
            planet->angle += M_PI/(float)(30*planet->radius);
        }

        for (int i = 0; i < solarSystem[j].nb_planet; i++) {
            Planet* planet = &(solarSystem[j].planets[i]);

            // Calculate the distance between the player and the current planet
            double dist = distance(player->self.cord_x, player->self.cord_y, planet->self.cord_x, planet->self.cord_y);

            // If the distance is less than the sum of the radii of the player and the planet, there is a collision
            if (dist < player->mass + planet->radius) {
                // Handle collision here, e.g. reset player position
                player->self.cord_x = player->start.cord_x;
                player->self.cord_y = player->start.cord_y;
            }
            else {
                //Handle gravity 
                double local_atract = (double)(planet->radius * player->mass) / (double)(dist * dist);
                local_atract *= 100000.0f;
                double gx = solarSystem[j].planets[i].self.cord_x - player->self.cord_x;
                double gy = solarSystem[j].planets[i].self.cord_y - player->self.cord_y;
                double lg = sqrt(solarSystem[j].planets[i].self.cord_x * solarSystem[j].planets[i].self.cord_x + solarSystem[j].planets[i].self.cord_y * solarSystem[j].planets[i].self.cord_y);
                double ngx = gx / lg;
                double ngy = gy / lg;
                accelx += ngx * local_atract;
                accely += ngy * local_atract;
            }
        }double dist = distance(player->self.cord_x, player->self.cord_y, solarSystem[j].self.cord_x, solarSystem[j].self.cord_y);
        if (dist < player->mass + solarSystem[j].radius) {
            // Handle collision here, e.g. reset player position
            player->self.cord_x = player->start.cord_x;
            player->self.cord_y = player->start.cord_y;
        }
        //Handle gravity 
        double local_atract = (double)(solarSystem[j].radius * player->mass) / (double)(dist * dist);
        local_atract *= 100000.0f;
        double gx = solarSystem[j].self.cord_x - player->self.cord_x;
        double gy = solarSystem[j].self.cord_y - player->self.cord_y;
        double lg = sqrt(solarSystem[j].self.cord_x * solarSystem[j].self.cord_x + solarSystem[j].self.cord_y * solarSystem[j].self.cord_y);
        double ngx = gx / lg;
        double ngy = gy / lg;
        accelx += ngx * local_atract;
        accely += ngy * local_atract;

    }
    //acceleration to velocity update
    player->velocity.velocity_x += accelx * (float)1 / (float)60;
    player->velocity.velocity_y += accely * (float)1 / (float)60;
    //player handling
    if (input->leftArrow) {
        Rotate_Velocity(player, -1);
    }
    if (input->rightArrow) {
        Rotate_Velocity(player,  1);
    }
    //Handle Start
    if ((player->start.cord_x == player->self.cord_x)&&(player->start.cord_y == player->self.cord_y)) {
        player->velocity.velocity_x = 0;
        player->velocity.velocity_y = 0;
        if (input->space) {
            float angle = (float)rand() / (float)100;
            int dist = 200+rand() % 100;
            player->velocity.velocity_x = cos(angle)*dist;
            player->velocity.velocity_y = sin(angle)*dist;
        }
    }
    //velocity to postion update
    player->self.cord_x += player->velocity.velocity_x * (float)1 / (float)60;
    player->self.cord_y += player->velocity.velocity_y * (float)1 / (float)60;
    //handle end
    if (distance(player->end.cord_x, player->end.cord_y, player->self.cord_x, player->self.cord_y) <= 5) {
        return 2;
    }


    if (player->self.cord_x > SCREEN_WIDTH) {
        player->self.cord_x = 0; // If the player moves to the right, reappears on the left
    }
    else if (player->self.cord_x < 0) {
        player->self.cord_x = SCREEN_WIDTH; // If the player passes left, reappears on the right
    }

    if (player->self.cord_y > SCREEN_HEIGHT) {
        player->self.cord_y = 0; // If the player goes to the bottom, reappears at the top
    }
    else if (player->self.cord_y < 0) {
        player->self.cord_y = SCREEN_HEIGHT; // If player moves to the top, reappears at the bottom
    }
    return EXIT_SUCCESS;
}