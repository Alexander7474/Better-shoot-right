#include"bot.h"
#include "../../Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "game.h"
#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>
#include <iostream>

Bot::Bot(){

}

void Bot::Bupdate(Map *map , Player user){
    this->cible=user.getCharacter().getPosition();
    float espace = cible.x-getPosition().x;
    
    if (espace>16.0f)
    {
        goLeft();
        lookAt(Vector2f(32.0f,getPosition().y-1.0f));
    }else if (espace<-16.0f)
    {
        goRight();
        lookAt(Vector2f(32.0f,getPosition().y+1.0f));
    }else{
        lookAt(cible);
        getGun().shoot();
    }
    update(map);  
}