#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"
#include "bot.h"

enum State{
    patrol =1,
    seek =2,
    engage =3
};
class ennemi : public bot
{
private:
    Vector2f seekp;
    Vector2f cible;
    int cpt,iterateur;
    bool direction;
    float *theta;
    float fov ;
    float unlock;
    float ammo,reloadtime;
    float detect,detect2,divi;
    State etat;
    bool ftd;
    
public:
    ennemi();
    void Bupdate(Map *map,GameCharacter *perso1);
    void detect_player(GameCharacter *perso1);
    void patrol_mod();
    void seek_mod(GameCharacter *user);
    void engage_mod(GameCharacter *user);
    void getshot(std::vector<Bullet> balls , float dmg);
};


