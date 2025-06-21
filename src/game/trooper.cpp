#include"trooper.h"


#include<cmath>
#include "../engine/gameCharacter.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;




Trooper::Trooper(){
    lookAt(Vector2f(getPosition().x+5,getPosition().y));
    name="steve";
    etat=patrol;
    fov = M_PI / 4;
    font = new Font(16, "toto.ttf");
    char buffer[20];
    sprintf(buffer, "HP: %d", gethp());
    hpbar=new TexteBox(buffer, font);
    dialoque = new TexteBox("shut up steve",font);
    setSpeed(2.0f);
    detect=10.0f;
    ftd=false;
    spawn=new Vector2f[3]{
        Vector2f(417.f,418.f),
        Vector2f(450.f,418.f),
        Vector2f(500.f,418.f)
    };
    cpt=0;
    iterateur=1;
    Bot();
    
}


void Trooper::Bupdate(Map *map , GameCharacter *user){
    getshot(user->getGun().getBullets(),user->getGun().getDamage());
    if (getHead().getetat()!=dead)
    {
    
        detect_player(user);
        patrol_mod();
        engage_mod(user);
        seek_mod(user);
        char buffer[20];
        sprintf(buffer, "HP: %d", gethp());
        hpbar->setTexte(buffer);
        hpbar->setPosition(Vector2f(getPosition().x-5,getPosition().y-20));
    
    }
    update(map);  
    
}





void Trooper::engage_mod(GameCharacter *user){
    float espace = getPosition().x-user->getPosition().x;
    if (etat==engage)
    {
        
        setSpeed(10.0f);   
        lookAt(user->getPosition()); 
        if (espace>50.0f)
        {
            goLeft();
        }
        if (espace<-50.0f)
        {
            goRight();
        }        
        if (espace<100.0f && espace>-100.0f)
        {
            getGun().shoot();
            getGun().reload();
        }
        if (espace<50 && espace>-50)
        {
            setSpeed(0);
        }
        
    }
    
}

void Trooper::patrol_mod() {
    if (etat == patrol) {
        
        
        if (patrol_zone()) {
            if (bbopGetDistance(getPosition(),spawn[cpt])<5.0f)
            {
                if (cpt==2)
                {
                    iterateur=-1;
                }
                if (cpt==0)
                {
                    iterateur=1;
                }
                cpt+=iterateur;
            }
            bc_patrol(spawn[cpt]);
        } else {
            bc_patrol(spawn[0]); 
        }
    }
}
void Trooper::seek_mod(GameCharacter *user){
    if (etat==seek)
    {
        float espace = bbopGetDistance(user->getPosition(),getPosition());
        setSpeed(4.5f);
        lookAt(seekp);
        if (espace>0)
        {
            goLeft();            
        }else{
            goRight();
        }
    }
    
}

void Trooper::interact(vector<GameCharacter*> otherbots){
    for (GameCharacter *npc: otherbots)
    {
        if (bbopGetDistance(getPosition(),npc->getPosition())<40 )
        {
            timer=glfwGetTime();
        }
        
    }
    
}

void Trooper::detect_player(GameCharacter *user) {
    if (champ_visuel(user)) {
        unlock=glfwGetTime();
        if (glfwGetTime()-divi>detect2)
        {
            etat=engage;
        }else if (glfwGetTime()-divi/2>detect2)
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

TexteBox* Trooper::gethpbar(){
    return hpbar;
}

const char *Trooper::getname(){
    return name;
}