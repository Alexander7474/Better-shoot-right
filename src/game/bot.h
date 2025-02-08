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
    float fov ;
    float sw_look;
    bool direction;
    float unlock;
    bool detect;
    State etat;
    vector<float> precision;
public:
    Bot();
    void Bupdate(Map *map,GameCharacter *perso1);
    void detect_player(GameCharacter *perso1);
    void patrol_mod();
    void seek_mod();
    void engage_mod(GameCharacter *user);
    GameCharacter& getCharacter();
    bool champ_visuel(GameCharacter *user);
};


