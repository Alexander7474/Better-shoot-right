#include "dynamicSprite.h"
#include "physic.h"
#include <box2d/b2_world.h>

DynamicSprite::DynamicSprite(const Texture &texture) : Sprite(texture) {}

void DynamicSprite::computePhysic(b2World *world) {
        setOrigin(getSize().x / 2, getSize().y / 2);
        // une fois ajouter a umonde box2d c'est la galère pour récupérer la
        // taille donc faut faire attention
        entityBody =
            addDynamicBox(world, &getCollisionBox(), 0.f, 1.f, 1.f, 1.f, false, false);
}

void DynamicSprite::updatePhysic() {
        setPosition(entityBody->GetPosition().x * PIXEL_PER_METER,
                    entityBody->GetPosition().y * PIXEL_PER_METER);
        setRotation(entityBody->GetAngle());
}
