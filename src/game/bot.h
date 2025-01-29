#pragma once

#include "../../Bbop-Library/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

enum State{
    patrol =1,
    seek =2,
    engage =3
};
class Bot : public GameCharacter
{
private:

    Vector2f cible;

    float sw_look;
    float unlock;
    bool detect;
    State etat;
public:
    Bot();
    void Bupdate(Map *map,GameCharacter *perso1);
    void detect_player(GameCharacter *perso1);
    void patrol_mod();
    void seek_mod();
    GameCharacter& getCharacter();
};


