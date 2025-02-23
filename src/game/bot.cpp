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
        Vector2f(417.f,418.),
        Vector2f(1.0f,1.0f),
        Vector2f(1.0f,1.0f)
    };
}

void Bot::Bupdate(Map *map , GameCharacter *user){

    detect_player(user);

    patrol_mod();
    bc_patrol(spawn[0]);
    engage_mod(user);
    seek_mod(user);
    cerr<<"x:"<<getPosition().y<<endl;
    cerr<<"y:"<<spawn[0].y<<endl;
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
        etat = bczone;
        ftd=false;
    }else if (etat==seek && glfwGetTime() - unlock > 5)
    {
        etat = bczone;
        ftd=false;
    }
    
}




bool Bot::champ_visuel(GameCharacter *user) {
    float range = 200.0f;
    
    // Vérification de la distance avant d'effectuer les calculs
    if (bbopGetDistance(getPosition(), user->getPosition()) < range) {

        // Calculer un angle d'orientation basé sur la direction réelle du regard
        Vector2f direction = {
            getLookingPoint().x - getPosition().x,
            getLookingPoint().y - getPosition().y
        };        
        float theta0 = atan2(direction.y, direction.x); // Angle en radians

        std::cout << "Theta0: " << theta0 
                  << " | Looking Point: (" << getLookingPoint().x << ", " << getLookingPoint().y << ")"
                  << " | Position: (" << getPosition().x << ", " << getPosition().y << ")"
                  << std::endl;

        // Création des angles de balayage pour le champ visuel
        std::vector<float> theta(21);
        for (int i = 0; i < 21; i++) {
            theta[i] = theta0 - (fov / 2) + (i * (fov / 20)); 
        }

        // Récupération des boîtes de collision du bot
        CollisionBox partie[5] = {
            getRightArm().getCollisionBox(),
            getLeftArm().getCollisionBox(),
            getLegs().getCollisionBox(),
            getBody().getCollisionBox(),
            getHead().getCollisionBox()
        };

        // Balayage du champ visuel
        for (int i = 0; i < 21; i++) {
            Vector2f start_p = getPosition();
            Vector2f step = {
                (range * cos(theta[i])) / 5,
                (range * sin(theta[i])) / 5
            };

            // Vérifier si une partie du bot est détectée dans le champ visuel
            for (int j = 0; j < 5; j++) {
                if (detect_point(partie, start_p)) {
                    if (!ftd) {
                        ftd = true;
                        detect2 = glfwGetTime();
                        seekp = user->getPosition();
                    }
                    divi = fabs((getPosition().x - start_p.x) / 5); // Correction absolue
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
        cerr<<patrol_zone()<<endl;
        if (patrol_zone())
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
        }else{
            float a = bbopGetDistance(getPosition(),spawn[0]);
            Vector2f zonev=spawn[0];
            Vector2f re;
            for (int i = 0; i < 3; i++)
            {
                if (bbopGetDistance(getPosition(),spawn[i])<a)
                {
                    a=bbopGetDistance(getPosition(),spawn[i]);
                    zonev=spawn[i];
                    re=spawn[0];
                    spawn[0]=zonev;
                    spawn[i]=re;
                }
            }
            etat=bczone;
        }
        
        
        
    }
    
}

void Bot::engage_mod(GameCharacter *user){
    float espace = user->getPosition().x-getPosition().x;
    if (etat==engage)
    {
        
        setSpeed(10.0f);   
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


bool Bot::patrol_zone(){

    for (int i = 0; i < 3; i++)
    {
        float distance=bbopGetDistance(getPosition(),spawn[i]);
        if (distance <150 )
        {
            etat=patrol;
            return true;
        }
    }
    etat=bczone;
    return false;
}

void Bot::bc_patrol(Vector2f point){
    if (etat==bczone)
    {
        if (getPosition().x!=point.x && getPosition().y!=point.y)
        {
            if (oldp.x==getPosition().x)
            {
                jump();
            }
            
            if (bbopGetDistance(point,getPosition())>0)
            {
                goRight();
            }else{
                goLeft();
            }
            oldp={
                getPosition().x,
                getPosition().y
            };
        }
    }
    
    
    
    
}