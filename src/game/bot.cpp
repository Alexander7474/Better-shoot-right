#include"bot.h"


#include<cmath>
#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>
#include <iostream>

Bot::Bot(){
    lookAt(Vector2f(getPosition().x-5,getPosition().y));
    sw_look=glfwGetTime();
    direction=true;
    etat=patrol;
    fov = M_PI / 4;
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
#include <cmath> // Pour tan() et abs()

bool Bot::champ_visuel(GameCharacter *user) {
    float fov = M_PI / 4; // 45 degrés
    float range = 500.0f; // Portée max

    float hauteur_max = std::tan(fov / 2) * range; // Demi-hauteur du cône

    // Position du bot
    float x_A = getPosition().x;
    float y_A = getPosition().y;

    float x_B, y_B, x_C, y_C;

    // Déterminer les sommets du triangle en fonction de la direction
    if (getLookingPoint().x > getPosition().x) { // Regarde à droite
        x_B = x_A + range;
        y_B = y_A + hauteur_max;
        x_C = x_A + range;
        y_C = y_A - hauteur_max;
    } else { // Regarde à gauche
        x_B = x_A - range;
        y_B = y_A + hauteur_max;
        x_C = x_A - range;
        y_C = y_A - hauteur_max;
    }

    // Position du joueur
    float x_P = user->getPosition().x;
    float y_P = user->getPosition().y;

    // Fonction pour calculer une aire
    auto calculerAire = [](float x1, float y1, float x2, float y2, float x3, float y3) {
        return std::abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
    };

    // Calcul des aires
    float aire_ABC = calculerAire(x_A, y_A, x_B, y_B, x_C, y_C);
    float aire_ABP = calculerAire(x_A, y_A, x_B, y_B, x_P, y_P);
    float aire_BCP = calculerAire(x_B, y_B, x_C, y_C, x_P, y_P);
    float aire_CAP = calculerAire(x_C, y_C, x_A, y_A, x_P, y_P);

    // Vérification si la somme des sous-triangles est égale à l’aire du grand triangle
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

void Bot::seek_mod(){
    
}