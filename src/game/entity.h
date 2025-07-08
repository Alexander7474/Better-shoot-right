#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/box2d.h>

/**
 * @brief Entité du monde
 *
 * @details tous objets qui va intéragir dans le monde de manière physique doit
 * hérité de cette class
 */
class Entity {
      protected:
        b2Body *entityBody =
            nullptr; //<! pointeur vers le body Box2d pour l'update
      public:
        virtual ~Entity() = default;

        /**
         * @brief créé l'objet physique dans le monde box2D
         * @details fais dans un second temps du constructeur pour permettre de
         * modifier les caractèristiques du sprite
         *
         * @param world Pointeur vers le monde ou ajouter l'Entité
         */
        virtual void computePhysic(b2World *world) = 0;

        /**
         * @brief Met a jour la physique depuis le monde box2D
         */
        virtual void updatePhysic() = 0;
};
