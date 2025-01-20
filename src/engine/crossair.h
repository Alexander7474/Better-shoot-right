#pragma once 

#include "../../Bbop-Library/include/BBOP/Graphics.h"

class Crossair : public Geometric, public BbopDrawable
{
private:
  RectangleShape externRect[4];
  RectangleShape middleRect;
public:
  Crossair();

  void setPosition(float x_, float y_) override;
  void Draw(GLint renderModeLoc) const override;
};
