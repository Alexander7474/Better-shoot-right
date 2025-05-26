#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

enum State{
    patrol =1,
    seek =2,
    engage =3
};
class bot : public GameCharacter
{
protected:
    Vector2f *spawn;
    Vector2f oldp; 
public:
    bot();
    bool champ_visuel(GameCharacter *user);
    bool patrol_zone();
    bool bc_patrol(Vector2f point);
    void getshot(std::vector<Bullet> balls , float dmg);
    bool detect_point(CollisionBox* menber,Vector2f point);
    void add_spawn(Vector2f sp);
};
