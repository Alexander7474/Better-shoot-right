#pragma once 

#include <box2d/box2d.h>

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "../game/entity.h"

class DynamicSprite : public Entity, public Sprite
{
public: 
  explicit DynamicSprite(const Texture& texture);

  void computePhysic(b2World *world) override;
  void updatePhysic() override;
};
