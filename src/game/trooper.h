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
    
    int cpt,iterateur;//sense en mode patrouille//
    float unlock;//temp avant de delock le joueur//
    RectangleShape *hpbar;//barre de vie//
    TexteBox *dialoque;//texte de dialogue//
    float ammo,reloadtime;//munition , temp de recharge//
    State etat;//etat du bot//
    bool discussing;//en train de parler//
    const char *name;//nom de bot //
    
    
public:
    Trooper();
    void Bupdate(Map *map,GameCharacter *perso1,vector<Trooper*> otherbots);//mise a jour du bot//
    void seek_mod(GameCharacter *user);// mode recherche lorsque le joueur sort du champ de vue//
    void engage_mod(GameCharacter *user);//mode de combat//
    void patrol_mod();//patrouille//
    void interact(vector<Trooper*> otherbots);//interaction avec les autres bot//
    void detect_player(GameCharacter *user);//look bot// 
    void Draw(Scene* scene);//dessine la bar de vie et le dialogue//
    const char * getname();//recup nom//

};



