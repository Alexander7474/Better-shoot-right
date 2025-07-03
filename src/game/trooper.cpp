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
    hpbar=new RectangleShape(Vector2f(35.f,3.f),Vector2f(getPosition().x-3,getPosition().y-2),Vector3i(255,0,0),Vector2f(getPosition().x-30,getPosition().y-20));
    dialoque = new TexteBox("shut up steve",font);
    detect=10.0f;
    ftd=false;
    spawn=new Vector2f[3]{
        Vector2f(417.f,418.f),
        Vector2f(450.f,418.f),
        Vector2f(500.f,418.f)
    };
    hpbar->setOrigin(getPosition());
    cpt=0;
    iterateur=1;
    Bot();

}


void Trooper::Bupdate(Map *map , GameCharacter *user,vector<Trooper*> otherbots){
    if (getHead().getetat()!=dead)
    {
        interact(otherbots);
        detect_player(user);
        engage_mod(user);
        seek_mod(user);
        hpbar->setPosition(Vector2f(getPosition().x+290,getPosition().y-20));
        dialoque->setPosition(Vector2f(getPosition().x,getPosition().y-50));
    }
    update(map);

}





void Trooper::engage_mod(GameCharacter *user){
    float espace = getPosition().x-user->getPosition().x;
    if (etat==engage)
    {
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
        }

    }

}

void Trooper::patrol_mod() {
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
void Trooper::seek_mod(GameCharacter *user){
    if (etat==seek)
    {
        float espace = bbopGetDistance(user->getPosition(),getPosition());
        lookAt(seekp);
        if (espace>0)
        {
            goLeft();
        }else{
            goRight();
        }
    }

}

void Trooper::interact(vector<Trooper*> otherbots){
    if (etat==patrol)
    {
        for (Trooper *npc: otherbots)
        {
            if (bbopGetDistance(getPosition(),npc->getPosition())<40 && discussing==false && npc != this)
            {
                timer=glfwGetTime();
                discussing=true;
            }
        }
        if (glfwGetTime()-timer>3)
        {
            patrol_mod();
        }
        if (glfwGetTime()-timer>5)
        {
            discussing=false;
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


const char *Trooper::getname(){
    return name;
}

void Trooper::Draw(Scene *scene){
    if (glfwGetTime()-timer<3)
    {
        scene->Draw(*dialoque);
    }
    float hp=(35.f/500.f)*getHp();
    hpbar->setSize(Vector2f(hp,hpbar->getSize().y));
    scene->Draw(*hpbar);
}