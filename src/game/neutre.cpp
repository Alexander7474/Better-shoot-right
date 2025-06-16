#include "neutre.h"

#include <iostream>

Neutre::Neutre()
{
    armed = false;
    show=false;
    etat=speak;
    font = new Font(16, "toto.ttf");
    boxs.push_back(new TexteBox("Hello adventurer want to party if so press x if you want to fight pressw w", font));
}

void Neutre::Bupdate(Map *map, GameCharacter *perso1,GLFWwindow *gameWindow)
{
    distance=bbopGetDistance(perso1->getPosition(),getPosition());
    if (glfwGetKey(gameWindow, GLFW_KEY_T)==GLFW_PRESS && distance<30 && etat==speak)
    {
        cerr<<"pp"<<endl;
        show=true;
        lookAt(perso1->getPosition());
        boxs[0]->setPosition(Vector2f(getPosition().x-60,getPosition().y-30));
    }else{
        if (show==true && glfwGetKey(gameWindow, GLFW_KEY_X)==GLFW_PRESS)
        {
          show=false;
          etat=allie;
        }
        if (show==true && glfwGetKey(gameWindow, GLFW_KEY_F)==GLFW_PRESS)
        {
          show=false;
          etat=fight;
        }
        
    }
    if (etat == allie && distance>50)
    {
      bc_patrol(perso1->getPosition());
    }
    if (etat== fight)
    {
      riposte(perso1);
    }
    
    update(map);
}

void Neutre::Drawbox(Scene *scene)
{
    if (show)
    {
      scene->Draw(*boxs[0]);
    }
    
}

void Neutre::riposte(GameCharacter *perso){
  lookAt(perso->getPosition());
  if (distance>70)
  {
    bc_patrol(perso->getPosition());
  }
  getGun().shoot();
}

Neutre::~Neutre()
{
  for(auto &t : boxs){
    delete t;
  }
  delete font;
}
