#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

using namespace std;

class Bot : public GameCharacter
{
protected:
    Vector2f *spawn;
    Vector2f oldp; 
    float fov ;
    Vector2f seekp;
    float detect,detect2,divi;
    bool ftd;
    Font *font;
public:
    Bot()=default;
    bool champ_visuel(GameCharacter *user);
    bool patrol_zone();
    bool bc_patrol(Vector2f point);
    bool detect_point(CollisionBox* menber,Vector2f point);
    void add_spawn(Vector2f sp);
};
