#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

class Bot;

class Agressivite
{
    private:
        int level; //niveau d agressivite//
        float range; //distance d'attaque//
        Bot* pnj;//Bot //
    public:
        Agressivite(int level,float range,Bot* pnj);
        void update(GameCharacter *joueur);
};