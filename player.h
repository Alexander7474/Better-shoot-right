#pragma once

#include "Bbop-Library/include/BBOP/Graphics/bbopGlobal.h"
#include "Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "gameCharacter.h"

class Player : public BbopDrawable
{
private:
  GameCharacter character;
public:
  Player();

  /**
  * @brief Met a jour le player 
  */
  void update(Camera *playerCam);

  virtual void Draw(GLint renderModeLoc) const override;
};
