#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

using namespace std;

class Bot : public GameCharacter
{
protected:
    Vector2f *spawn; //liste des point de passage en mode patrouille//
    Vector2f oldp; //ancienne position pour le deplacement//
    float fov ;//fov/
    Vector2f seekp;//dernier point du joueur avant de sortir du champvisuel//
    float detect2,divi;//temp de detection//
    bool ftd;//first time detect//
    Font *font;
    float timer;//timer pour la discussion//
public:
    Bot()=default;
    bool champ_visuel(GameCharacter *user);//verifie si le joueur se situe dans le champ visuel//
    bool patrol_zone();//verifie si le bot est proche d un des point de patrouille//
    bool bc_patrol(Vector2f point);//retourne a un point donne//
    bool detect_point(CollisionBox* menber,Vector2f point);//verifie si un des menbres du joueur est detecte //
    void setspawn(Vector2f s1,Vector2f s2,Vector2f s3);//permet de donner une liste de point de patrouille//
    
};

