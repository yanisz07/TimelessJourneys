#ifndef NPCBEHAVIOR_H
#define NPCBEHAVIOR_H


#include "ECS.hpp"
#include <stdlib.h>
#include <math.h>
#include "../Vector2D.hpp"
#include "../TextureManager.hpp"
#include "../game.hpp"
#include "TransformComponent.hpp"
#include "../AssetManager.hpp"



//forward decleration
class TransformComponent;
//end;

class NPCBehavior : public Component
{
public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    SDL_Rect srcRect, destRect;
    SDL_Texture *texture;
    std::string bubble;

    float trigger_distance;
    bool bubble_displayed;

    NPCBehavior(float distance_1, TransformComponent* playerTrans, std::string b);

    void init() override;

    void update() override;

    float calculateDistanceToPlayer() const;

    void draw() override;

    ~NPCBehavior() override;

};







#endif // NPCBEHAVIOR_H
