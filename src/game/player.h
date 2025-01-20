#pragma once

#include "../../Bbop-Library/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

class Player : public BbopDrawable
{
private:
  GameCharacter character;

  Crossair crossair;

public:
  Player();

  /**
  * @brief Met a jour le player 
  */
  void update(Camera *playerCam, Map* map);

  virtual void Draw(GLint renderModeLoc) const override;

  //GETTER 
  
  GameCharacter& getCharacter();
  Crossair& getCrossair();
};
