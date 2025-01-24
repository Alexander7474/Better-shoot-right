#pragma once

#include "../../Bbop-Library/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"


class Bot : public GameCharacter
{
private:

    Vector2f cible;
public:
    Bot();
    void Bupdate(Map *map,Player perso1);

    GameCharacter& getCharacter();
    //Vector2f e_cible();
};


