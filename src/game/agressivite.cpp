#include"agressivite.h"

aagressivite::agressivite(int level, float range){
    this.range=range;
    this.level=level;
}

void agressivite::update(&Bot pnj,&enum etat,GameCharacter *joueur){
    float espace = getPosition().x-joueur->getPosition().x;
    switch (level)
    {
        case 0:
            
            if (etat==seek)
            {
                pnj.lookAt(pnj.getSeekPoint());
                if (espace>0)
                {
                    pnj.goLeft();
                }else{
                    pnj.goRight();
                }
            }
            if (etat==engage)
            {
                pnj.lookAt(joueur->getPosition());
                if (espace>50.0f)
                {
                    goLeft();
                }
                if (espace<-50.0f)
                {
                    goRight();
                }
                if (espace<range && espace>-range)
                {
                    getGun().shoot();
                    getGun().reload();
                }
            }
            break;
        
            
        case 1:
            if (espace>0)
            {
                pnj.goLeft();
            }else{
                pnj.goRight();
            }
            break;
    }
}