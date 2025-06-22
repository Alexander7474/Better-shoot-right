#include "dynamicSprite.h"
#include "box2d-bbop-link.h"
#include <box2d/b2_world.h>

DynamicSprite::DynamicSprite(Texture texture) : Sprite(texture) {}

void DynamicSprite::computePhysic(b2World *world) {
  setOrigin(getSize().x / 2, getSize().y / 2);
  // une fois ajouter a umonde box2d c'est la galère pour récupérer la taille
  // donc faut faire attention
  entityBody = addDynamicBox(world, &getCollisionBox(), 1.f, 0.f, false);
}

void DynamicSprite::updatePhysic() {
  setPosition(entityBody->GetPosition().x * PIXEL_PER_METER,
              entityBody->GetPosition().y * PIXEL_PER_METER);
  setRotation(entityBody->GetAngle());
}
