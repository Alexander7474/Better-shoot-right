#include"bot.h"



#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>
#include <iostream>

Bot::Bot(){
    lookAt(Vector2f(getPosition().x-5,getPosition().y));
    sw_look=glfwGetTime();
    direction=true;
    etat=patrol;
}

void Bot::Bupdate(Map *map , GameCharacter *user){
    detect_player(user);
    patrol_mod();
    engage_mod(user);
    
    update(map);  
}

void Bot::detect_player(GameCharacter * user){
    float espace = user->getPosition().x-getPosition().x;
    if (getLookingPoint().x>getPosition().x)
    {
        if (espace<500 && espace>200)
        {
            etat=seek;
        }else if (espace<200 && espace>50)
        {
            etat=engage;
            detect=true;
            unlock=glfwGetTime();
            lookAt(user->getPosition());
        }
        
    }else if (getLookingPoint().x<getPosition().x)
    {
        if (espace>-500 && espace<-200)
        {
            etat=seek;
        }else if (espace>-200 && espace<-50)
        {
            etat=engage;
            detect=true;
            unlock=glfwGetTime();             
        }
    }
    if (glfwGetTime()-unlock >3)
    {
        detect=false;
        etat=patrol;
    }
}

void Bot::patrol_mod(){
    if (etat==patrol)
    {
        if (glfwGetTime()-sw_look<0.5)
        {
            if (direction)
            {
                goLeft();
                lookAt(Vector2f(getPosition().x-5,getPosition().y));
            }else{
                goRight();
                lookAt(Vector2f(getPosition().x+5,getPosition().y));
            }
            
        }else{
            direction=!direction;
            sw_look=glfwGetTime();
        }
        
    }
    
}

void Bot::engage_mod(GameCharacter *user){
    float espace = user->getPosition().x-getPosition().x;
    if (etat==engage)
    {
        lookAt(user->getPosition()); 
        if (espace>100.0f && espace<200.0f)
        {
            goRight();
            if (espace<120.0f)
            {
                getGun().shoot();
                getGun().reload();
            }
        }
        if (espace<-100.0f && espace>-200.0f)
        {
            goLeft();
            if (espace>-120.0f)
            {
                getGun().shoot();
                getGun().reload();
            }
        }else{
            getGun().shoot();
            getGun().reload();
        }                
        
    }
    
}