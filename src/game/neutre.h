#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"
#include "bot.h"
using namespace std;


class neutre : public bot
{
private:
    
    Vector2f cible;
    int cpt,iterateur;
    bool direction;
    float *theta;
    float unlock;
    float ammo,reloadtime;
    bool armed;
    bool allie;
    vector<TexteBox> box;
    bool show;
    float timer;

public:
    neutre();
    void Bupdate(Map *map,GameCharacter *perso1 ,GLFWwindow *gameWindow);
    void detect_player(GameCharacter *perso1);
    void riposte(GameCharacter *perso);
    void engage_mod(GameCharacter *user);
    void getshot(std::vector<Bullet> balls , float dmg);
    void Drawbox(Scene *scene);
};