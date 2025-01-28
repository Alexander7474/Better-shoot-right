#include"bot.h"



#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>
#include <iostream>

Bot::Bot(){
    lookAt(Vector2f(getPosition().x-5,getPosition().y));
    sw_look=glfwGetTime();
}

void Bot::Bupdate(Map *map , GameCharacter *user){
    detect_player(user);
    if (detect)
    {
        this->cible=user->getPosition();
        float espace = cible.x-getPosition().x;
        if (espace>100.0f)
        {
            goRight();
            lookAt(cible);
            if (espace<120.0f)
            {
                getGun().shoot();
                getGun().reload();
            }
        }else if (espace<-100.0f)
        {
            goLeft();
            lookAt(cible);
            if (espace>-120.0f)
            {
                getGun().shoot();
                getGun().reload();
            }                    
        }else{
            lookAt(cible);
            getGun().shoot();
            getGun().reload();
        }
    }else{
        if (sw_look-glfwGetTime()<-1.5)
        {
            if (getLookingPoint().x<getPosition().x)
            {
                lookAt(Vector2f(getPosition().x+5,getPosition().y));
                sw_look=glfwGetTime();
            }else{
                lookAt(Vector2f(getPosition().x-5,getPosition().y));
                sw_look=glfwGetTime();
            }
            
        }
        
    }
    update(map);  
}

void Bot::detect_player(GameCharacter * user){
    float espace = user->getPosition().x-getPosition().x;
    if (getLookingPoint().x>getPosition().x)
    {
        if (espace>-100 && espace<-50)
        {
            detect=true;
            unlock=glfwGetTime();
        }
        
    }else if (getLookingPoint().x<getPosition().x)
    {
        if (espace<100 && espace>50)
        {
            detect=true;
            unlock=glfwGetTime();
        }
    }
    if (unlock-glfwGetTime() < -1)
    {
        detect=false;
    }
    
    
}