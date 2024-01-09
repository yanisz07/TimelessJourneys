#include "SpriteComponent.hpp"

SpriteComponent::SpriteComponent(std::string id)
{
    setTex(id);
}

SpriteComponent::SpriteComponent(bool isAnimated, std::string type)
{
    animated = isAnimated;
    this->type = type;
}

void SpriteComponent::setActions()
{
    //type = entity->type;
    std::map<std::string, Action> actions;
    actions = (entity->manager.getAssetManager()->world.Characters[type].Actions);

    for (auto it = actions.begin(); it != actions.end(); it++)
    {
        animations[it->first] =  Animation(it->second.y_0, 0, it->second.number_frames, 100, it->second.spriteName);
    }

    if (type=="player")
    {
        Play("Idle_Down");
    }
    else
    {
        Play("Idle");
    }
}

void SpriteComponent::addAnimation(std::string animName, Animation animation)
{
    animations[animName] = animation;
}

void SpriteComponent::setTex(std::string id)
{
    texture = Game::assets->GetTexture(id);
}

void SpriteComponent::init()
{

    transform = &entity->getComponent<TransformComponent>();

    srcRect.x = srcRect.y = 0;
    srcRect.w = transform->width;
    srcRect.h = transform->height;
}

void SpriteComponent::update()
{
    if(animated)
    {
        if(animations[currentAction].repeat==-1)
        {
            frame = static_cast<int>((animations[currentAction].timer.getTimeStart() / animations[currentAction].speed) % animations[currentAction].frames);
            srcRect.x = srcRect.w * frame; //update x index in the sprites sheet


            if(animations[currentAction].timer.getTimeStart() > animations[currentAction].frames*animations[currentAction].speed)
            {
                animations[currentAction].timer.start();
            }
        }
        else
        {
            if (animations[currentAction].repeat!=0)
            {
                frame = static_cast<int>((animations[currentAction].timer.getTimeStart() / animations[currentAction].speed) % animations[currentAction].frames);
                srcRect.x = srcRect.w * frame; //update x index in the sprites sheet
                if(animations[currentAction].timer.getTimeStart() > animations[currentAction].frames*animations[currentAction].speed-50)
                {
                    animations[currentAction].timer.start();
                    animations[currentAction].repeat-=1;
                }
            }
            else
            {
                if (type=="player")
                {
                    Play("Idle_Down");
                }
                else
                {
                    Play("Idle");
                }
            }
        }
    }

    destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
    destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
    destRect.w = transform->width * transform->scale;
    destRect.h = transform->height * transform->scale;
}

void SpriteComponent::draw()
{
    //SDL_Rect rectangle{0,0,128,128};
    TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
}

void SpriteComponent::Play(const char *animName, bool flip, const int repeat, int speed)
{
    if (flip)
    {
        spriteFlip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        spriteFlip = SDL_FLIP_NONE;
    }
    currentAction = animName;
    //animIndex = animations[animName].index;

    setTex(animations[animName].spriteName);
    srcRect.x = srcRect.y = 0;
    srcRect.h = animations[animName].wh;
    srcRect.w = animations[animName].wh;
    animations[currentAction].repeat = repeat;
    animations[currentAction].speed = speed;
    animations[currentAction].timer.start();
}
