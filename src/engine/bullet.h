#pragma once

#include "../../Bbop-Library/include/BBOP/Graphics.h"

enum BulletType
{
  _5x56mm,
  apfsds // lol
};

class Bullet : public Sprite
{
private:
  Vector2f inertie;

  friend class Gun;

public:
  Bullet(Texture *texture);

  void update();
};
