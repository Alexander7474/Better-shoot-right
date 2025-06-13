#include "dynamicSprite.h"
#include "box2d-bbop-link.h"
#include <box2d/b2_world.h>

DynamicSprite::DynamicSprite(Texture texture, b2World *world)
  :  Sprite(texture)
{
  addDynamicBox(world, &getCollisionBox());
}

void DynamicSprite::updatePhysic()
{

}
