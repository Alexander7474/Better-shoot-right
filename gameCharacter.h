#pragma once 

#include <string>
#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/fontsClass.h>
#include <BBOP/Graphics/geometricClass.h>
#include <BBOP/Graphics/lightClass.h>
#include <BBOP/Graphics/spriteClass.h>

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
