#include"bot.h"



#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>
#include <iostream>

Bot::Bot(){
    lookAt(Vector2f(getPosition().x-5,getPosition().y));
    sw_look=glfwGetTime();
    direction=true;
}

void Bot::Bupdate(Map *map , GameCharacter *user){
    detect_player(user);
    if(!detect){
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
        if (espace>-500 && espace<-200)
        {
            etat=seek;
        }else if (espace>-200 && espace<-50)
        {
            etat=engage;
            detect=true;
            unlock=glfwGetTime();
            if (espace<-100.0f && espace>-200.0f)
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
            etat=patrol;
        }
        
    }else if (getLookingPoint().x<getPosition().x)
    {
        if (espace<500 && espace>200)
        {
            etat=seek;
        }else if (espace<200 && espace>50)
        {
            etat=engage;
            detect=true;
            unlock=glfwGetTime();
            if (espace>100.0f && espace<200.0f)
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
            etat=patrol;
        }
    }
    if (glfwGetTime()-unlock >3)
    {
        detect=false;
    }
}

void Bot::patrol_mod(){
    if (etat=patrol)
    {
        if (glfwGetTime()-sw_look<1.5)
        {
            if (direction)
            {
                goLeft();
            }else{
                goRight();
            }
            
        }else{
            direction=!direction;
            sw_look=glfwGetTime();
        }
        
    }
    
}