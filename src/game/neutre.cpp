#include"neutre.h"
#include <filesystem>
#include <iostream>
#include <fstream>

neutre::neutre() {
    cerr<<"pp"<<endl;
    Font *f=new Font(32,"Robot.ttf");
    cerr<<"pp1"<<endl;
    TexteBox textbox("bbop enginer", f);
    cerr<<"pp3"<<endl;   
    box.push_back(textbox);       
    armed = false;
    show=false;
}




void neutre::Bupdate(Map *map, GameCharacter *perso1,GLFWwindow *gameWindow){

    cerr<<file.good()<<endl;
   
    if (glfwGetKey(gameWindow, GLFW_KEY_T)==GLFW_PRESS && bbopGetDistance(perso1->getPosition(),getPosition())<30)
    {
        box[0].setPosition(Vector2f(getPosition().x,getPosition().y-5));
        show=true;
        timer=glfwGetTime();
    }else{
        if (glfwGetTime()-timer>5 && show==true)
        {
            show=false;
        }
        
    }
    update(map);
}
void neutre::Drawbox(Scene *scene){
    if (show)
    {
        
        scene->Draw(box[0]);
    }
    
}