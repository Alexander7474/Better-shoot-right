#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
<<<<<<< HEAD
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
=======
#include "../engine/crossair.h"
#include "../engine/gameCharacter.h"

class Bot;

class Agressivite {
      private:
        int level;   // niveau d agressivite//
        float range; // distance d'attaque//
        Bot *pnj;    // Bot //
      public:
>>>>>>> 9317585c3da299d149399e889900a6a9e2839913
        /**
         * @brief constructeur par default du composant agressivite
         *
         * @param level niveau d'agressivite
<<<<<<< HEAD
         * @param range distance dattaque 
         * @param Bot le bot quelle impact
         */
        Agressivite(int level,float range,Bot* pnj);
=======
         * @param range distance dattaque
         * @param Bot le bot quelle impact
         */
        Agressivite(int level, float range, Bot *pnj);
>>>>>>> 9317585c3da299d149399e889900a6a9e2839913
        /**
         * @brief mise a jour de l'agressivite(attaque ou non etc)
         *
         * @param joueur le fdp qu'il faudra peut être éliminée
<<<<<<< HEAD
         * @param game la game dans la quelle ca se deroule 
         * 
         */
        void update(GameCharacter *joueur,Game * game);

        int getThreat();
};
=======
         * @param game la game dans la quelle ca se deroule
         *
         */
        void update(GameCharacter *joueur, Game *game);
};
>>>>>>> 9317585c3da299d149399e889900a6a9e2839913
