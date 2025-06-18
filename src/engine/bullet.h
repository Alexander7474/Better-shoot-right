#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics.h"

enum BulletType
{
  _5x56mm,
  apfsds // lol
};

class Bullet : public Sprite
{
private:
  Vector2f inertie;
  bool used;
  friend class Gun;

public:
  Bullet(Texture *texture, Vector2f _inertie);

  void update();
  bool hit();
  void setused();
};
