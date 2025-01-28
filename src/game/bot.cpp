#include"bot.h"



#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>
#include <iostream>

Bot::Bot(){
    lookAt(Vector2f(getPosition().x-5,getPosition().y));
    sw_look=glfwGetTime();
}

void Bot::Bupdate(Map *map , GameCharacter *user){
    
    if (detect)
    {
        this->cible=user->getPosition();
        float espace = cible.x-getPosition().x;
        if (espace>100.0f)
        {
            goRight();
            lookAt(Vector2f(32.0f,getPosition().y-1.0f));
        }else if (espace<-100.0f)
        {
            goLeft();
            lookAt(Vector2f(32.0f,getPosition().y+1.0f));
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
            }else{
                lookAt(Vector2f(getPosition().x-5,getPosition().y));
            }
            
        }
        
    }
    
    
    
    
    update(map);  
}

void Bot::detect_player(GameCharacter * user){
    float espace = user->getPosition().x-getPosition().x;
    if (getLookingPoint().x>getPosition().x)
    {
        if (espace>-100)
        {
            detect=true;
            unlock=glfwGetTime();
        }
        
    }else if (getLookingPoint().x<getPosition().x)
    {
        if (espace<100)
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