#pragma once

#include "../../Bbop-Library/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"


class Bot : public GameCharacter
{
private:

    Vector2f cible;

    float sw_look;
    float unlock;
    bool detect;
public:
    Bot();
    void Bupdate(Map *map,GameCharacter *perso1);
    void detect_player(GameCharacter *perso1);
    GameCharacter& getCharacter();
    //Vector2f e_cible();
};


