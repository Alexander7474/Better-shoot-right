#include "bullet.h"
#include "../game/game.h"

using namespace std;

Bullet::Bullet(Texture *texture, Vector2f _inertie)
  : Sprite(*texture)
{
  inertie = _inertie;
  setOrigin(4,4);
}

void Bullet::update()
{
  move(inertie.x * DELTA_TIME, inertie.y * DELTA_TIME);
}
