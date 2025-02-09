#include"bot.h"


#include<cmath>
#include "../engine/gameCharacter.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void shuffleTable(vector<float>* table) {
    

    // Initialize random number generator with a time-based seed
    std::mt19937 rng(static_cast<unsigned int>(std::time(0)));

    // Shuffle the array in place
    std::shuffle(table, table + 6, rng);
}


Bot::Bot(){
    lookAt(Vector2f(getPosition().x-5,getPosition().y));
    sw_look=glfwGetTime();
    direction=true;
    etat=patrol;
    fov = M_PI / 4;
    precision={-1.f,1.f,5.f,9.f,0,-9};
    //setSpeed(2.0f);
}

void Bot::Bupdate(Map *map , GameCharacter *user){
    detect_player(user);
    patrol_mod();
    engage_mod(user);
    
    update(map);  
}

void Bot::detect_player(GameCharacter *user) {
    
    float espace = user->getPosition().x - getPosition().x;

    
    bool regardeDroite = getLookingPoint().x > getPosition().x;
    bool joueurADroite = espace > 0;
    bool joueurAGauche = espace < 0;
    
    if (champ_visuel(user)) {

        if (regardeDroite && joueurADroite) {
            if (espace < 500 && espace > 200 && etat!=engage) {
                etat = seek;
            } else if (espace < 200 && espace > 50) {
                etat = engage;
                detect = true;
                unlock = glfwGetTime();
                lookAt(user->getPosition()); 
            }
            if (etat==engage && espace>250 )
            {
                goLeft();
            }
        }
        else if (!regardeDroite && joueurAGauche) {
            if (espace > -500 && espace < -200 && etat!=engage) {
                etat = seek;
            } else if (espace > -200 && espace < -50) {
                etat = engage;
                detect = true;
                unlock = glfwGetTime();
                lookAt(user->getPosition()); 
            }
            if (etat==engage && espace<-250 )
            {
                goRight();
            }
            
        }
    }

    if (detect && glfwGetTime() - unlock > 5) {
        detect = false;
        etat = patrol;
    }
}

#include <cmath> 

bool Bot::champ_visuel(GameCharacter *user) {
    float range = 500.0f; 

    float hauteur_max = std::tan(fov / 2) * range;

    float x_A = getPosition().x;
    float y_A = getPosition().y;

    float x_B, y_B, x_C, y_C;

    if (getLookingPoint().x > getPosition().x) { 
        x_B = x_A + range;
        y_B = y_A + hauteur_max;
        x_C = x_A + range;
        y_C = y_A - hauteur_max;
    } else { 
        x_B = x_A - range;
        y_B = y_A + hauteur_max;
        x_C = x_A - range;
        y_C = y_A - hauteur_max;
    }

    float x_P = user->getPosition().x;
    float y_P = user->getPosition().y;

    auto calculerAire = [](float x1, float y1, float x2, float y2, float x3, float y3) {
        return std::abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
    };


    float aire_ABC = calculerAire(x_A, y_A, x_B, y_B, x_C, y_C);
    float aire_ABP = calculerAire(x_A, y_A, x_B, y_B, x_P, y_P);
    float aire_BCP = calculerAire(x_B, y_B, x_C, y_C, x_P, y_P);
    float aire_CAP = calculerAire(x_C, y_C, x_A, y_A, x_P, y_P);
    return std::abs(aire_ABC - (aire_ABP + aire_BCP + aire_CAP)) < 0.01;
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
        
        cerr<<"coco"<<endl;
        
        if (glfwGetTime()-sw_shoot>1.5f)
        {
            shuffleTable(&precision);
            point_shot.y=user->getPosition().y+precision[0];
            point_shot.x=user->getPosition().x;
            sw_shoot=glfwGetTime();
        }else{
            point_shot.x=user->getPosition().y;
            point_shot.y=user->getPosition().y+precision[0];
        }
        
        
        lookAt(point_shot); 
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

void Bot::seek_mod(GameCharacter *user){
    if (etat==seek)
    {
        float espace = user->getPosition().x - getPosition().x;
        //setspeed(1.5f);
        if (espace>0)
        {
            goRight();
            lookAt(Vector2f(getPosition().x+5,getPosition().y));
        }else{
            goLeft();
            lookAt(Vector2f(getPosition().x-5,getPosition().y));
        }
        
    }
    
}