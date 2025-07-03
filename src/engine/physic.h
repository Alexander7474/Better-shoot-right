#pragma once

// Contient les fonction de lien entre BBOP et box2D

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include <box2d/box2d.h>

// Box 2D foncitonne en mètre donc il faut une constante pour savoir combien de
// pixel vaut un mètre
#define PIXEL_PER_METER 32

/**
 * @brief Créé une boite de collision Static dans un monde box2d à partir d'une
 * collision box BBOP
 *
 * @param world Pointeur vers le monde
 * @param box pointeur vers la forme à rajouter
 *
 * @return body Corps box2d de la boite
 */
b2Body *addStaticBox(b2World *world, const Geometric *box);

/**
 * @brief Créé une boite de collision Dynamic dans un monde box2d à partir d'une
 * collision box BBOP
 * @warning Cette fonction modifie l'origine de la geometric passé en pointeur pour correspodre à l'origine box2D
 *
 * @param world Pointeur vers le monde
 * @param box pointeur vers la boite à rajouter
 * @param restitution
 * @param density
 * @param friction
 * @param linearDamping
 * @param rotationLock Si le corp a sa rotation blocké
 * @param offsetX Permet de prendre en compte un offsetX si la Geometric est une boite de collision
 * @param offsetY Permet de prendre en compte un offsetY si la Geometric est une boite de collision
 *
 * @return body Corps box2d de la boite
 */
b2Body *addDynamicBox(b2World *world, Geometric *box,
                      const float restitution, const float density,
                      const float friction, const float linearDamping,
                      const bool rotationLock, const Vector2f& offsetX = Vector2f(0,0),
                      const Vector2f& offsetY = Vector2f(0,0));

/**
 * @brief Données rattachées à chaque body box2D
 * @details isTouching sert pour le listenner de gestion de collision
 */
struct BodyData {
        // detection de collision
        bool isTouchingDown = false;
        bool isTouchingUp = false;
        bool isTouchingLeft = false;
        bool isTouchingRight = false;
        // limitation
        short unsigned jumpCpt;
};

/**
 * @brief Listenner de contact pour gérer les collisions
 */
class CustomContactListener final : public b2ContactListener {
      public:
        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;

      private:
        /**
         * @brief Gère les contacts
         * @details Modifie isTouching de BodyData
         *
         * @param contact
         * @param begin
         */
        static void handleContact(b2Contact *contact, bool begin);
};