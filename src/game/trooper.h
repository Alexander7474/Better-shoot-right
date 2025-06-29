#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"
#include "ennemi.h"

enum State{
    patrol =1,
    seek =2,
    engage =3,
    interaction =4
};
class Trooper : public Ennemi
{
private:
    
    int cpt,iterateur;
    float unlock;
    RectangleShape *hpbar;
    TexteBox *dialoque;
    float ammo,reloadtime;
    State etat;
    bool discussing;
    const char *name;
    
    
public:
    Trooper();
    void Bupdate(Map *map,GameCharacter *perso1,vector<Trooper*> otherbots);
    void seek_mod(GameCharacter *user);
    void engage_mod(GameCharacter *user);
    void patrol_mod();
    void interact(vector<Trooper*> otherbots);
    void detect_player(GameCharacter *user); 
    void Draw(Scene* scene);
    const char * getname();

};

