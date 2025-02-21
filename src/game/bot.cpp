#include"bot.h"


#include<cmath>
#include "../engine/gameCharacter.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
bool detect_point(CollisionBox* menber,Vector2f point){
    for (int i = 0; i < 5; i++)
    {
        if (point.x >= menber[i].getLeft() && point.x <= menber[i].getRight() && point.y >= menber[i].getTop() && point.y <= menber[i].getBottom())
        {
            return true;
        }
        
    }
    return false;
}


Bot::Bot(){
    lookAt(Vector2f(getPosition().x+5,getPosition().y));
    sw_look=glfwGetTime();
    direction=true;
    etat=patrol;
    fov = M_PI / 4;
    setSpeed(2.0f);
    points=new Vector2f[21];
    theta=new float[21];
    detect=10.0f;
    ftd=false;
    spawn=new Vector2f[3]{
        Vector2f(1.0f,1.0f),
        Vector2f(1.0f,1.0f),
        Vector2f(1.0f,1.0f)
    };
}

void Bot::Bupdate(Map *map , GameCharacter *user){

    detect_player(user);

    patrol_mod();

    engage_mod(user);
    seek_mod(user);
    cerr<<"x:"<<getLookingPoint().x-getPosition().x<<endl;
    cerr<<"y:"<<getLookingPoint().y-getPosition().y<<endl;
    cerr<<"etat:"<<etat<<endl;
    update(map);  
}

void Bot::detect_player(GameCharacter *user) {
    if (champ_visuel(user)) {
        unlock=glfwGetTime();
        if (glfwGetTime()-3>detect2)
        {
            etat=engage;
        }else 
        {
            etat=seek;
        }
    }

    if (etat==engage && glfwGetTime() - unlock > 5) {
        etat = patrol;
        ftd=false;
    }else if (etat==seek && glfwGetTime() - unlock > 5)
    {
        etat = patrol;
        ftd=false;
    }
    
}



bool Bot::champ_visuel(GameCharacter *user) {
    float range = 500.0f;

    if (bbopGetDistance(getPosition(), user->getPosition()) < range) {
        float theta0 = (getLookingPoint().x > getPosition().x) ? 0 : M_PI;


        std::vector<float> theta(21);
        for (int i = 0; i < 21; i++) {
            theta[i] = theta0 - (fov / 2) + (i * (fov / 20)); 
        }

        CollisionBox partie[5] = {
            getRightArm().getCollisionBox(),
            getLeftArm().getCollisionBox(),
            getLegs().getCollisionBox(),
            getBody().getCollisionBox(),
            getHead().getCollisionBox()
        };

        for (int i = 0; i < 21; i++) {
            Vector2f start_p = getPosition();
            Vector2f step = {
                (range * cos(theta[i])) / 5,
                (range * sin(theta[i])) / 5
            };

            for (int j = 0; j < 5; j++) {
                if (detect_point(partie, start_p)) {
                    if (!ftd)
                    {
                        ftd=true;
                        detect2=glfwGetTime();
                        seekp=user->getPosition();  
                    }
                    divi=(getPosition().x-start_p.x)/5;
                    if (divi<0) divi=divi*-1;
                    return true;
                }
                start_p.x += step.x;
                start_p.y += step.y;
            }
        }

        return false;
    }
    return false;
}



void Bot::patrol_mod(){
    if (etat==patrol)
    {
        setSpeed(6.0f);
        if (glfwGetTime()-sw_look<10)
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
        setSpeed(3.0f);   
        lookAt(user->getPosition()); 
        if (espace<200.0f)
        {
            goLeft();
            if (espace<120.0f)
            {
                getGun().shoot();
                getGun().reload();
            }
        }
        if (espace>-200.0f)
        {
            goRight();
            if (espace>-120.0f)
            {
                getGun().shoot();
                getGun().reload();
            }
        }        
        
    }
    
}

void Bot::seek_mod(GameCharacter *user){
    if (etat==seek)
    {
        float espace = bbopGetDistance(user->getPosition(),getPosition());
        setSpeed(1.5f);
        lookAt(seekp);
        if (espace>0)
        {
            goLeft();
            
        }else{
            goRight();
        }
        
        
    }
    
}