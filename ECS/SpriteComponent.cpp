#include "SpriteComponent.hpp"
#include "armor.hpp""

SpriteComponent::SpriteComponent(std::string id)
{
    setTex(id);
    this->type = id;
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
    else if(type == "chest")
    {
        Play("Inactive");
    }
    else if(type == "archer")
    {
        Play("Idle_Down",false,-1,150);
    }
    else if(type == "Canon")
    {
        Play("Right");
    }
    else if(type == "spawner")
    {
        Play("Idle");
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
            animations[currentAction].index = srcRect.w * frame;//update x index in the sprites sheet
            srcRect.x = animations[currentAction].index;

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
                else if(type == "chest")
                {
                    Play("Inactive");
                }
                else if (type == "archer")
                {
                    Play("Idle_Down",false,-1,150);
                }
                else if(type == "Canon")
                {
                    Play("Right");
                }
                else
                {
                    Play("Idle");
                }
            }
        }
    }

    if (type != "canon_bullet" && type != "spawner")
    {
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }
    destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
    destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
}

void SpriteComponent::draw()
{
    if (angle==0)
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }
    else
    {
        TextureManager::Draw_rotation(texture, srcRect, destRect, spriteFlip, angle);
    }
}

void SpriteComponent::Play(const std::string animName, bool flip, const int repeat, int speed, bool armorchange)
{
    if (flip)
    {
        spriteFlip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        spriteFlip = SDL_FLIP_NONE;
    }

    if (armorchange)
    {
        animations[animName].index = animations[currentAction].index;
    }
    else
    {
        animations[animName].index = 0;
    }

    currentAction = animName;

    if (type!="spawner")
    {
        setTex(animations[currentAction].spriteName);
        srcRect.y = 0;
        srcRect.x = animations[currentAction].index;
        srcRect.h = animations[currentAction].wh;
        srcRect.w = animations[currentAction].wh;
        animations[currentAction].repeat = repeat;
        animations[currentAction].speed = speed;
        animations[currentAction].timer.start();
    }

    if (type=="spawner")
    {
        setTex(animations[currentAction].spriteName);
        srcRect.y = 0;
        srcRect.x = animations[currentAction].index;
        srcRect.h = 500;
        srcRect.w = animations[currentAction].wh;
        animations[currentAction].repeat = repeat;
        animations[currentAction].speed = speed;
        animations[currentAction].timer.start();
    }

}


void SpriteComponent::setAngle(double angle)
{
    this->angle=angle;
}

void SpriteComponent::Set_Dest_Rect(int w, int h)
{
    destRect.h = h;
    destRect.w = w;
}
