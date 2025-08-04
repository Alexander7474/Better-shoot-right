#include "bot.h"
#include "agressivite.h" 
#include "game.h"
#include <ostream>

std::ostream& operator<<(std::ostream& os, Bot::State state) {
    switch (state) {
        case Bot::State::patrol: os << "patrol"; break;
        case Bot::State::seek: os << "seek"; break;
        case Bot::State::engage: os << "engage"; break;
        default: os << "unknown"; break;
    }
    return os;
}


Bot::Bot()
    : pnj(std::make_unique<GameCharacter>()) {
    
    
    pnj->lookAt(Vector2f(pnj->getPosition().x + 5, pnj->getPosition().y));
    
    etat = Bot::State::patrol; // ✅ Proper initialization

    fov = M_PI / 4;
    font = new Font(16, "toto.ttf");

    hpbar = new RectangleShape(Vector2f(35.f, 3.f),
                               Vector2f(pnj->getPosition().x - 3, pnj->getPosition().y - 2),
                               Vector3i(255, 0, 0),
                               Vector2f(pnj->getPosition().x - 30, pnj->getPosition().y - 20));

    dialoque = new TexteBox("shut up steve", font);
    ftd = false;

    Chokpoint= new Vector2f[3]{
        Vector2f(500.f, 1281.f),
        Vector2f(600.f, 1281.f),
        Vector2f(700.f, 1281.f)
    };

    hpbar->setOrigin(pnj->getPosition());

    cpt = 1;
    iterateur = 1;
    menace = new Agressivite(0, 50, this);
}

void Bot::update(Map* map, GameCharacter* user,Game * game) {
    if (pnj->getHead().getetat() != 3) {
        std::cerr<<getState()<<endl;
        detectPlayer(user);
        patrolMod();
        menace->update(user,game);
        hpbar->setPosition(Vector2f(pnj->getPosition().x + 290, pnj->getPosition().y - 20));
        dialoque->setPosition(Vector2f(pnj->getPosition().x, pnj->getPosition().y - 50));
    }
    float hp = (35.f / 500.f) * pnj->getHp();
    hpbar->setSize(Vector2f(hp, hpbar->getSize().y));
    pnj->update(map);
}

bool Bot::champVisuel(GameCharacter* user) {
    float range = 200.0f;

    Vector2f toUser = {
        user->getPosition().x - pnj->getPosition().x,
        user->getPosition().y - pnj->getPosition().y
    };
    float angleToUser = atan2(toUser.y, toUser.x);

    Vector2f direction = {
        pnj->getLookingPoint().x - pnj->getPosition().x,
        pnj->getLookingPoint().y - pnj->getPosition().y
    };
    float theta0 = atan2(direction.y, direction.x);

    float halfFov = fov / 2;
    if (fabs(angleToUser - theta0) > halfFov) {
        return false;
    }

    if (bbopGetDistance(pnj->getPosition(), user->getPosition()) < range) {

        std::vector<float> theta(21);
        for (int i = 0; i < 21; i++) {
            theta[i] = theta0 - halfFov + (i * (fov / 20));
        }

        CollisionBox partie[5] = {
            pnj->getRightArm().getCollisionBox(),
            pnj->getLeftArm().getCollisionBox(),
            pnj->getLegs().getCollisionBox(),
            pnj->getBody().getCollisionBox(),
            pnj->getHead().getCollisionBox()
        };

        for (int i = 0; i < 21; i++) {
            Vector2f start_p = pnj->getPosition();
            Vector2f step = {
                (range * cos(theta[i])) / 5,
                (range * sin(theta[i])) / 5
            };

            for (int j = 0; j < 5; j++) {
                if (detectPoint(partie, start_p)) {
                    if (!ftd) {
                        ftd = true;
                        Detect = glfwGetTime();
                        SeekPosition = user->getPosition();
                    }
                    Diviseur = fabs((pnj->getPosition().x - start_p.x) / 5);
                    return true;
                }
                start_p.x += step.x;
                start_p.y += step.y;
            }
        }
    }
    return false;
}

bool Bot::detectPoint(CollisionBox* menber, Vector2f point) {
    for (int i = 0; i < 5; i++) {
        if (point.x >= menber[i].getLeft() && point.x <= menber[i].getRight() &&
            point.y >= menber[i].getTop() && point.y <= menber[i].getBottom()) {
            return true;
        }
    }
    return false;
}

bool Bot::patrolZone() {
    for (int i = 0; i < 3; i++) {
        if (bbopGetDistance(pnj->getPosition(), Chokpoint[cpt]) < 500.f) {
            return true;
        }
    }
    return false;
}

bool Bot::moveToPoint(Vector2f point) {
    if (bbopGetDistance(point, pnj->getPosition()) > 10.f) {
        if (point.x - pnj->getPosition().x > 0.f) {
            pnj->goRight();
            pnj->lookAt(Vector2f(pnj->getPosition().x + 5, pnj->getPosition().y));
        } else {
            pnj->goLeft();
            pnj->lookAt(Vector2f(pnj->getPosition().x - 5, pnj->getPosition().y));
        }

        PreviousPosition = pnj->getPosition();
        return false;
    } else {
        return true;
    }
}

void Bot::setChockPoint(Vector2f s1, Vector2f s2, Vector2f s3) {
    Chokpoint= new Vector2f[3]{
        s1,
        s2,
        s3
    };
}

Vector2f Bot::getSeekPosition() {
    return SeekPosition;
}



void Bot::patrolMod() {
    if (patrolZone() && getState()==Bot::State::patrol) {
        std::cerr<<pnj->getPosition().x-Chokpoint[cpt].x<<endl;
        if (bbopGetDistance(pnj->getPosition(), Chokpoint[cpt]) < 10.0f) {
            if (cpt == 2) {
                iterateur = -1;
            }
            if (cpt == 0) {
                
                iterateur = 1;
            }
            cpt += iterateur;
        }
        moveToPoint(Chokpoint[cpt]);
    } else {
        moveToPoint(Chokpoint[0]);
        std::cerr<<"error3"<<endl;
    }
}

void Bot::detectPlayer(GameCharacter* user) {
    if (champVisuel(user)) {
        unlock = glfwGetTime();
        if (glfwGetTime() - Diviseur > Detect) {
            etat = Bot::State::engage;   // ✅ corrected
        } else if (glfwGetTime() - Diviseur / 2 > Detect) {
            etat = Bot::State::seek;     // ✅ corrected
        }
    }

    if (etat == Bot::State::engage && glfwGetTime() - unlock > 5) {
        etat = Bot::State::patrol;       // ✅ corrected
        ftd = false;
    } else if (etat == Bot::State::seek && glfwGetTime() - unlock > 5) {
        etat = Bot::State::patrol;       // ✅ corrected
        ftd = false;
    }
}

void Bot::Draw(GLint *renderUniforms) const {
    pnj->Draw(renderUniforms);
}

GameCharacter* Bot::getCharacter() {
    return pnj.get();
}

Bot::State Bot::getState() {
    return etat;
}
