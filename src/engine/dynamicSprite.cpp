#include "dynamicSprite.h"
#include "box2d-bbop-link.h"
#include <box2d/b2_world.h>

DynamicSprite::DynamicSprite(Texture texture, b2World *world)
  :  Sprite(texture)
{

  setSize(100,100);
  setOrigin(getSize().x/2,getSize().y/2);
  setPosition(10, -200);
  //une fois ajouter a umonde box2d c'est la galère pour récupérer la taille donc faut faire attention
  body = addDynamicBox(world, &getCollisionBox());
}

void DynamicSprite::updatePhysic()
{
  setPosition(body->GetPosition().x*PIXEL_PER_METER, body->GetPosition().y*PIXEL_PER_METER);
  setRotation(body->GetAngle());
}
