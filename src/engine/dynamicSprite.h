#pragma once 

#include <box2d/box2d.h>

#include "../../Bbop-Library/include/BBOP/Graphics.h"
#include "../game/entity.h"

class DynamicSprite : public Entity, public Sprite
{
public: 
  DynamicSprite(Texture texture, b2World *world);

  void updatePhysic() override;
};
