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
    Vector2f *spawn;
    Vector2f seekp;
    Vector2f oldp; 
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
    Bot();
    void Bupdate(Map *map,GameCharacter *perso1);
    void detect_player(GameCharacter *perso1);
    void patrol_mod();
    void seek_mod(GameCharacter *user);
    void engage_mod(GameCharacter *user);
    bool champ_visuel(GameCharacter *user);
    bool patrol_zone();
    bool bc_patrol(Vector2f point);
    void getshot(std::vector<Bullet> balls , float dmg);
};


