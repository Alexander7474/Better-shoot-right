#include"agressivite.h"
#include "bot.h"
#include "game.h"

Agressivite::Agressivite(int level, float range, Bot *pnj){
    this->range=range;
    this->level=level;
    this->pnj=pnj;
}

void Agressivite::update(GameCharacter *joueur , Game * game){
    float espace = bbopGetDistance(pnj->getCharacter()->getPosition(),joueur->getPosition());
    switch (level)
    {
        case 0:
            
            if (pnj->getState() == Bot::State::seek)
            {
                pnj->getCharacter()->lookAt(pnj->getSeekPosition());
                pnj->moveToPoint(pnj->getSeekPosition());
            }
            if (pnj->getState()== Bot::State::engage)
            {
                pnj->getCharacter()->lookAt(joueur->getPosition());
                if (espace>range)
                {
                    pnj->moveToPoint(joueur->getPosition());
                }
                if (espace<range)
                {
                    pnj->getCharacter()->lookAt(joueur->getPosition());
                    pnj->getCharacter()->getGun().shoot(game,true);
                }
            }
            break;
        
            
        case 1:
            if (espace<100.f)
            {
                pnj->MoveAway(joueur->getPosition());
            }
    }
}

int Agressivite::getThreat(){
    return level;
}