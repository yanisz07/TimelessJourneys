#include "NPCBehavior.h"
#include <SDL.h>.
#include "TransformComponent.hpp"

NPCBehavior::NPCBehavior(float distance_1, TransformComponent *playerTrans, std::string b)
{
    playerTransform=playerTrans;
    trigger_distance= distance_1;
    bubble_displayed=0;
    bubble=b;
}

void NPCBehavior::init()
{
    transform = &entity->getComponent<TransformComponent>();
    srand(time(NULL));

    texture = Game::assets->GetTexture(bubble);
    srcRect.x = srcRect.y = 0;
    srcRect.w = 140;
    srcRect.h = 100;

}



void NPCBehavior::update()
{

    float d=calculateDistanceToPlayer();
    bool display=(d<trigger_distance);
    if (display) {

        bubble_displayed=1;

    } else {
        bubble_displayed=0;
    }


    destRect.x = static_cast<int>(transform->position.x) +80 - Game::camera.x;
    destRect.y = static_cast<int>(transform->position.y) -40 - Game::camera.y;
    destRect.w = 140;
    destRect.h = 100;

}

float NPCBehavior::calculateDistanceToPlayer() const
{
    return transform->position.distance(playerTransform->position);
}

void NPCBehavior::draw()
{
    //SDL_Rect rectangle{0,0,128,128};
    if (bubble_displayed) {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
    //SDL_RenderCopy(Game::renderer,texture,NULL,destRect);
}
