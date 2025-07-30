#include"agressivite.h"
#include "bot.h"

Agressivite::Agressivite(int level, float range, Bot *pnj){
    this->range=range;
    this->level=level;
    this->pnj=pnj;
}

void Agressivite::update(GameCharacter *joueur){
    float espace = pnj->getCharacthere()->getPosition().x-joueur->getPosition().x;
    switch (level)
    {
        case 0:
            
            if (pnj->getState() == Bot::State::seek)
            {
                pnj->getCharacthere()->lookAt(pnj->getSeekPosition());
                if (espace>0)
                {
                    pnj->getCharacthere()->goLeft();
                }else{
                    pnj->getCharacthere()->goRight();
                }
            }
            if (pnj->getState()== Bot::State::engage)
            {
                pnj->getCharacthere()->lookAt(joueur->getPosition());
                if (espace>50.0f)
                {
                    pnj->getCharacthere()->goLeft();
                }
                if (espace<-50.0f)
                {
                    pnj->getCharacthere()->goRight();
                }
                if (espace<range && espace>-range)
                {
                    pnj->getCharacthere()->getGun().shoot();
                    pnj->getCharacthere()->getGun().reload();
                }
            }
            break;
        
            
        case 1:
            if (espace>0)
            {
                pnj->getCharacthere()->goLeft();
            }else{
                pnj->getCharacthere()->goRight();
            }
            break;
    }
}