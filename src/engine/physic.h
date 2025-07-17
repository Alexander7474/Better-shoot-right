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

// TODO -- optimiser cette fonction qui gère la creation de tous les objet dynamique box2d (peut-être laisser les class se créer elle même dans box2d)
/**
 * @brief Créé une boite de collision Dynamic dans un monde box2d à partir d'une
 * collision box BBOP
 * @warning Cette fonction modifie l'origine de la geometric passé en pointeur
 * pour correspodre à l'origine box2D
 *
 * @param world Pointeur vers le monde
 * @param box pointeur vers la boite à rajouter
 * @param restitution
 * @param density
 * @param friction
 * @param linearDamping
 * @param rotationLock Si le corp a sa rotation blocké
 * @param isBullet est-ce une balle
 * @param offsetX Permet de prendre en compte un offsetX si la Geometric est une
 * boite de collision
 * @param offsetY Permet de prendre en compte un offsetY si la Geometric est une
 * boite de collision
 *
 * @return body Corps box2d de la boite
 */
b2Body *addDynamicBox(b2World *world, Geometric *box, const float restitution,
                      const float density, const float friction,
                      const float linearDamping, const bool rotationLock,
		      const bool isBullet,
                      const Vector2f &offsetX = Vector2f(0, 0),
                      const Vector2f &offsetY = Vector2f(0, 0));

/**
 * @brief Enumeration des class possible pour le gestionaire de collision
 */
enum class BodyType {
	Undefined,
	Static,
	Utilitie,
	Item,
        Bullet,
        GameCharacter
};

/**
 * @brief Données rattachées à chaque body box2D
 * @details isTouching sert pour le listenner de gestion de collision
 */
struct BodyData {
        BodyType type = BodyType::Undefined; //<! typre de la class
        uintptr_t ptr = 0; //<! pointeur vers la class
};

class Game;
/**
 * @brief Listenner de contact pour gérer les collisions
 */
class CustomContactListener final : public b2ContactListener {
      public:
	/**
	 * @brief Init le pointer game
	 * @details Obligatiore pour utiliser des méthodes de
	 * game lors de certain contact
	 * @param g pointer vers la game
	 */
	static void setGameOwner(Game* g);

        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;

      private:
	static Game* game; //<! Pointe vers la partie qui utilise le listener

        /**
         * @brief Gère les contacts
         * @details Modifie isTouching de BodyData
         *
         * @param contact
         * @param begin
         */
        static void handleContact(b2Contact *contact, bool begin);
};
