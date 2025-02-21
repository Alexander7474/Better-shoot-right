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

    Vector2f *points;
    Vector2f spawn;
    Vector2f seekp;
    Vector2f oldp; 
    float *theta;
    float fov ;
    float sw_look;
    bool direction;
    float unlock;
    float detect,detect2,divi;
    State etat;
    bool ftd;
public:
    Bot();
    void Bupdate(Map *map,GameCharacter *perso1);
    void detect_player(GameCharacter *perso1);
    void patrol_mod();
    void seek_mod(GameCharacter *user);
    void engage_mod(GameCharacter *user);
    GameCharacter& getCharacter();
    bool champ_visuel(GameCharacter *user);
};


