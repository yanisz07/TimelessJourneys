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


/*
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

};

*/


class NPCBehavior : public Component {
private:
    float moveDistance = 200; // Total distance to move
    float speed = 1.0; // Speed of movement
    Uint32 waitTime = 2000; // Time to wait in milliseconds (2 seconds)
    Uint32 lastTime; // Last time the NPC switched direction or started waiting
    bool movingRight = true; // Direction of movement
    bool waiting = false; // Is the NPC currently waiting?
    Vector2D originalPosition;

public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    SDL_Rect srcRect, destRect;
    SDL_Texture *texture;
    std::string bubble;

    float trigger_distance;
    bool bubble_displayed;

    NPCBehavior(float distance_1, TransformComponent* playerTrans, std::string b);

    float calculateDistanceToPlayer() const;

    void draw() override;

    void init() override;

    void update() override;


};


#endif // NPCBEHAVIOR_H
