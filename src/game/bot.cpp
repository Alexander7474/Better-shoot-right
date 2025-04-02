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

Vector2f closestp(Vector2f *point , Vector2f b){
    Vector2f clos = point[0];
    for (int i = 0; i < 3; i++)
    {
        if (bbopGetDistance(point[i],b)<bbopGetDistance(clos,b))
        {
            clos=point[i];
        }
        
    }
    return clos;
}

Bot::Bot(){
    lookAt(Vector2f(getPosition().x+5,getPosition().y));
    etat=patrol;
    fov = M_PI / 4;
    setSpeed(2.0f);
    points=new Vector2f[21];
    theta=new float[21];
    detect=10.0f;
    ftd=false;
    direction = true;
    spawn=new Vector2f[3]{
        Vector2f(417.f,418.f),
        Vector2f(450.f,418.f),
        Vector2f(500.f,418.f)
    };
    cpt=0;
    iterateur=1;
    
}

void Bot::Bupdate(Map *map , GameCharacter *user){
    cerr<<etat<<endl;
    getshot(user->getGun().getBullets(),user->getGun().getDamage());
    if (etat!=dead)
    {
        detect_player(user);
        patrol_mod();
        engage_mod(user);
        seek_mod(user);
    }
    
    update(map);  
}

void Bot::detect_player(GameCharacter *user) {
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




bool Bot::champ_visuel(GameCharacter *user) {
    float range = 200.0f;
    
    Vector2f toUser = {
        user->getPosition().x - getPosition().x,
        user->getPosition().y - getPosition().y
    };
    float angleToUser = atan2(toUser.y, toUser.x);


    Vector2f direction = {
        getLookingPoint().x - getPosition().x,
        getLookingPoint().y - getPosition().y
    };
    float theta0 = atan2(direction.y, direction.x);

    float halfFov = fov / 2;
    if (fabs(angleToUser - theta0) > halfFov) {
        return false;
    }

    if (bbopGetDistance(getPosition(), user->getPosition()) < range) {

        std::vector<float> theta(21);
        for (int i = 0; i < 21; i++) {
            theta[i] = theta0 - halfFov + (i * (fov / 20));
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
                    if (!ftd) {
                        ftd = true;
                        detect2 = glfwGetTime();
                        seekp = user->getPosition();
                    }
                    divi = fabs((getPosition().x - start_p.x) / 5);
                    cerr<<divi<<endl;
                    return true;
                }
                start_p.x += step.x;
                start_p.y += step.y;
            }
        }
    }
    return false;
}





void Bot::patrol_mod() {
    if (etat == patrol) {
        
        
        if (patrol_zone()) {
            if (bbopGetDistance(getPosition(),spawn[cpt])<5.0f)
            {
                if (cpt==2)
                {
                    iterateur=-1;
                    cerr<<spawn[cpt].x<<endl;
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


void Bot::engage_mod(GameCharacter *user){
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
        if (bbopGetDistance(getPosition(),spawn[cpt])<60)
        {
            return true;
        }
        
    }

    return false;
}

bool Bot::bc_patrol(Vector2f point) {
    setSpeed(15.0f);

    if (bbopGetDistance(point, getPosition())> 5) { 
        if (oldp.x == getPosition().x) {
            jump();  
        }

        if (point.x - getPosition().x > 0) {
            goRight();
            lookAt(Vector2f(getPosition().x + 5, getPosition().y));
        } else {
            goLeft();
            lookAt(Vector2f(getPosition().x - 5, getPosition().y));
        }

        oldp = getPosition();  
        return false;
    } else {
        setSpeed(0);  
        return true;
    }
}

    
    
    
void Bot::getshot(vector<Bullet> balls ,float dmg){
    CollisionBox partie[5] = {
        getRightArm().getCollisionBox(),
        getLeftArm().getCollisionBox(),
        getLegs().getCollisionBox(),
        getBody().getCollisionBox(),
        getHead().getCollisionBox()
    };
    for (int i = 0; i < 5; i++)
    {
        for (long unsigned int j = 0; j < balls.size(); j++)
        {
            if (partie[i].check(balls[j].getCollisionBox()))
            {
                sethp(gethp()-2);
                break;
            }
        }
    }
    if (gethp()<=0)
    {
        etat=dead;
    }
    
}