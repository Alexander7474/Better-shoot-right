#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
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

  virtual void Draw(GLint *renderUniforms) const override;

  //GETTER 
  
  GameCharacter& getCharacter();
  Crossair& getCrossair();
};
