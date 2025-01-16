#include "bullet.h"

using namespace std;

Bullet::Bullet(Texture *texture, Vector2f _inertie)
  : Sprite(*texture)
{
  inertie = _inertie;
}

void Bullet::update()
{
  move(inertie);
}
