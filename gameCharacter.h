#pragma once 

#include <string>
#include "Bbop-Library/include/BBOP/Graphics.h"

class GameCharacter : public BbopDrawable, public Geometric
{
private:
  // attribute for the body of the character 
  Sprite legs;
  Sprite rightArm;
  Sprite leftArm;
  Sprite body;
  Sprite head;

public:
  GameCharacter(std::string characterSpritePath);

  /**
  * @brief Draw the character
  */
  virtual void Draw(GLint renderModeLoc) const override;
};
