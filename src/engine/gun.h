#pragma once 

#include "../../Bbop-Library/include/BBOP/Graphics.h"
#include "member.h"

class Gun : public Sprite
{
private:
  MemberAnim animations[15];
public:
  Gun();
  void update();
};
