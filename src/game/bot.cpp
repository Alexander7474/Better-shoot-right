#include"bot.h"
bool bot::detect_point(CollisionBox* menber,Vector2f point){
    for (int i = 0; i < 5; i++)
    {
        if (point.x >= menber[i].getLeft() && point.x <= menber[i].getRight() && point.y >= menber[i].getTop() && point.y <= menber[i].getBottom())
        {
            return true;
        }
        
    }
    return false;
}
bool bot::patrol_zone(){

    for (int i = 0; i < 3; i++)
    {
        if (bbopGetDistance(getPosition(),spawn[0])<60)
        {
            return true;
        }
        
    }

    return false;
}

bool bot::bc_patrol(Vector2f point) {
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