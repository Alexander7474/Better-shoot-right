#include "bullet.h"
#include "../game/game.h"

using namespace std;

Bullet::Bullet(Texture *texture, Vector2f _inertie) : Sprite(*texture) {
        used = false;
        inertie = _inertie;
        setOrigin(4, 4);
}

void Bullet::update() { move(inertie.x * DELTA_TIME, inertie.y * DELTA_TIME); }

bool Bullet::hit() { return used; }

void Bullet::setused() { used = true; }