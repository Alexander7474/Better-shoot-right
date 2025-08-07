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
        /**
         * @brief constructeur par default du composant agressivite
         *
         * @param level niveau d'agressivite
         * @param range distance dattaque 
         * @param Bot le bot quelle impact
         */
        Agressivite(int level,float range,Bot* pnj);
        /**
         * @brief mise a jour de l'agressivite(attaque ou non etc)
         *
         * @param joueur le fdp qu'il faudra peut être éliminée
         * @param game la game dans la quelle ca se deroule 
         * 
         */
        void update(GameCharacter *joueur,Game * game);
};