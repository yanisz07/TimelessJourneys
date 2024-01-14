#include "sword.h"
#include "TransformComponent.hpp"

void Sword::draw()
{
    if (is_attacking_melee)
    {
        if (!timer.timedOut())
        {
            entityTransform = &(entity->getComponent<TransformComponent>());
            std::cout << "draw attack" << std::endl;
            srcR.y = 0;
            frame = static_cast<int>((timer.getTimeOutStart() / speed) % frames);
            index = width*frame;
            srcR.x = index;
            Vector2D entityPos = entityTransform->position;
            int scale = entityTransform->scale;
            if (entityTransform->x_direction == 0)
            {
                if (entityTransform->y_direction == -1)
                {
                    destR.x = entityPos.x+18*scale+6 - Game::camera.x;
                    destR.y = entityPos.y+13*scale-15 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_NONE);
                }
                else
                {
                    destR.x = entityPos.x+18*scale+6 - Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_VERTICAL);
                }
            }
            if (entityTransform->y_direction == 0)
            {
                if (entityTransform->x_direction == -1)
                {
                    Vector2D center = Vector2D(entityPos.x+27,entityPos.y+13*scale+29);
                    destR.x = center.x - 14 - Game::camera.x;
                    destR.y = center.y - 27 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-90,NULL,SDL_FLIP_NONE);
                }
                else
                {
                    Vector2D center = Vector2D(entityPos.x+(18+13)*scale+27,entityPos.y+13*scale+29);
                    destR.x = center.x - 14 - Game::camera.x;
                    destR.y = center.y - 27 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,90,NULL,SDL_FLIP_NONE);
                }
            }
            if (entityTransform->x_direction == 1)
            {
                if (entityTransform->y_direction == -1)
                {
                    destR.x = entityPos.x+(13+18)*scale-10-Game::camera.x;
                    destR.y = entityPos.y-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,45,NULL,SDL_FLIP_NONE);
                }
                if (entityTransform->y_direction == 1)
                {
                    destR.x = entityPos.x+(13+18)*scale-10-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,135,NULL,SDL_FLIP_NONE);
                }
            }
            if (entityTransform->x_direction == -1)
            {
                if (entityTransform->y_direction == -1)
                {
                    destR.x = entityPos.x-10-Game::camera.x;
                    destR.y = entityPos.y-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-45,NULL,SDL_FLIP_NONE);
                }
                if (entityTransform->y_direction == 1)
                {
                    destR.x = entityPos.x-10-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,225,NULL,SDL_FLIP_NONE);
                }
            }

        }
        else
        {
            update_sword();
        }
    }
}

void Sword::update_sword()
{
    is_attacking_melee = false;
}
