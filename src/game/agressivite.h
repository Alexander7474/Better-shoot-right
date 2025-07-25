#include "ennemi.h"

class agressivite
{
    private:
        int level //niveau d agressivite//
        float range //distance d'attaque//
    public:
        agressivite(int level,float range);
        update(&Bot pnj,&enum etat,GameCharacter *joueur);
}