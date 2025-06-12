#include"bot.h"




bool bot::champ_visuel(GameCharacter *user) {
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
                    return true;
                }
                start_p.x += step.x;
                start_p.y += step.y;
            }
        }
    }
    return false;
}


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

void bot::getshot(vector<Bullet> balls ,float dmg){
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
        getHead().setetat(2);
    }
    
}