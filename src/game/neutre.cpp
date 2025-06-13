#include "neutre.h"

#include <iostream>

Neutre::Neutre()
{
    cerr<<"pp"<<endl;
    cerr<<"pp1"<<endl;
    cerr<<"pp3"<<endl;   
    armed = false;
    show=false;
    font = new Font(32, "toto.ttf");
    boxs.push_back(new TexteBox("bbop enginer", font));
}

void Neutre::Bupdate(Map *map, GameCharacter *perso1,GLFWwindow *gameWindow)
{
    if (glfwGetKey(gameWindow, GLFW_KEY_T)==GLFW_PRESS && bbopGetDistance(perso1->getPosition(),getPosition())<30)
    {
        show=true;
        timer=glfwGetTime();
        boxs[0]->setPosition(Vector2f(getPosition().x,getPosition().y-5));
    }else{
        if (glfwGetTime()-timer>5 && show==true)
        {
            show=false;
        }
        
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

Neutre::~Neutre()
{
  for(auto &t : boxs){
    delete t;
  }
  delete font;
}
