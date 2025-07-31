#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"
#include "bot.h"
#include <vector>
using namespace std;
enum Sate{
  speak,
  allie,
  fight
};

class Neutre : public Bot
{
private:
    
    Vector2f cible;
    int cpt,iterateur;
    bool direction;
    float *theta;
    float unlock;
    float ammo,reloadtime;
    bool armed;
    Sate etat;
    bool show;
    float distance;
     // pointeur vers la font du bot
    std::vector<TexteBox*> boxs; // liste de pointeurs vers les textebox du bot

public:

  Neutre();
  ~Neutre();

  void Bupdate(Map *map,GameCharacter *perso1 ,GLFWwindow *gameWindow);
  void DetectPlayer(GameCharacter *perso1);
  void riposte(GameCharacter *perso);
  void engage_mod(GameCharacter *user);
  void getshot(std::vector<Bullet> balls , float dmg);
  void Drawbox(Scene *scene);
};
