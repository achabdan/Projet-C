//
// Created by DUMOND on 25/09/2023.
//

#include "vue_controller.h"
int Render_All(SDL_Renderer* renderer, SSystem* SSYSTEM, Player* PLAYER, int Nb_sol, int SCREEN_WIDTH, int SCREEN_HEIGHT,keys * input) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect S;
    S.x = 0;
    S.y = 0;
    S.w = SCREEN_WIDTH;
    S.h = SCREEN_HEIGHT;
    SDL_RenderFillRect(renderer, &S);
    SDL_Rect p;
    p.x = PLAYER->self.cord_x - 5;
    p.y = PLAYER->self.cord_y - 5;
    p.w = 10;
    p.h = 10;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);;
    SDL_RenderFillRect(renderer, &p);
    SDL_Rect end;
    end.w = 10;
    end.h = 10;
    end.x = PLAYER->end.cord_x - 5;
    end.y = PLAYER->end.cord_y - 5;
    SDL_Rect start;
    start.w = 10;
    start.h = 10;
    start.x = PLAYER->start.cord_x - 5;
    start.y = PLAYER->start.cord_y - 5;
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderDrawRect(renderer, &end);
    SDL_RenderDrawRect(renderer, &start);
    for (int i = 0; i < Nb_sol; i++) {
        
        filledCircleRGBA(renderer, SSYSTEM[i].self.cord_x, SSYSTEM[i].self.cord_y, SSYSTEM[i].radius, 255, 255, 0, 255);
        for (int j = 0; j < SSYSTEM[i].nb_planet; j++) {
            filledCircleRGBA(renderer, SSYSTEM[i].planets[j].self.cord_x, SSYSTEM[i].planets[j].self.cord_y, SSYSTEM[i].planets[j].radius, 0, 0, 255, 255);
        }
    }
    if (input->vectors) {
        SDL_RenderDrawLine(renderer, PLAYER->self.cord_x, PLAYER->self.cord_y, PLAYER->self.cord_x + PLAYER->velocity.velocity_x, PLAYER->self.cord_y + PLAYER->velocity.velocity_y);
    }
    return EXIT_SUCCESS;
}

void Key_Update(keys* self)
{
    self->escape = false;
    self->leftArrow = false;
    self->space = false;
    self->rightArrow = false;
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            self->escape = true;
            break;

        case SDL_KEYDOWN:
            if (evt.key.repeat)
                break;

            switch (evt.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                self->escape = true;
                break;
            case SDL_SCANCODE_LEFT:
                self->leftArrow = true;
                break;
            case SDL_SCANCODE_RIGHT:
                self->rightArrow = true;
                break;
            case SDL_SCANCODE_SPACE:
                self->space = true;
                break;
            case SDL_SCANCODE_V:
                self->vectors = !self->vectors;
                break;
            default:
                break;
            }
            break;

        case SDL_KEYUP:
            if (evt.key.repeat)
                break;

            switch (evt.key.keysym.scancode)
            {
            default:
                break;
            }
            break;
        }
    }
}