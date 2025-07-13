#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <memory>
#include <string>

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../game/entity.h"
#include "gun.h"
#include "member.h"

extern const char *gameCharacterStateString[4];

// éviter les redéfinitions
#ifndef DIRECTION
#define DIRECTION

enum Direction { rightDir, leftDir };

#endif

// TODO -- Gérer la vie et la collision avec le balle
class GameCharacter : public BbopDrawable, public Geometric, public Entity {
      private:
        // membres
        Member legs;
        Member rightArm;
        Member leftArm;
        Member body;
        Member head;
        // membres pour les ragdolls
        b2Body *headR;
        b2Body *bodyR;
        b2Body *rightArmR;
        b2Body *leftArmR;
        b2Body *legsR;

        // objet du character
        std::unique_ptr<Gun> gun;

        // gestion du regard et de l'orientation du character
        Vector2f lookingPoint;        //<! Ou le character regarde
        Direction characterDirection; //<! Direction du regard

        // taille du caractère
        float scale;

        // attributs de la physique
        float maxVelocityX;
        float maxVelocityY;
        float newtonX;
        float newtonY;
        float restitution;
        float friction;
        float density;
        float linearDamping;
        bool onRagdoll,touchingDown = false;

        // attributs du jeu
        float hp;

        /**
         *  @brief Détermine la rotaion d'un membre en fonction du lookingPoint
         */
        void setMemberRotation(Sprite &member);
        void setMemberRotation(Sprite &member, float m);

      public:
        GameCharacter();

        /**
         * @brief remplie les listes de textures
         */
        void createTextureCache(const std::string &path);

        /**
         * @brief Met jour le character
         */
        void update(Map *map);

        /**
         * @brief Draw the character
         */
        void Draw(GLint *renderUniforms) const override;

        /**
         * @brief change la postition du character
         * @details Donc toutes les position des sprites constituant le
         * character avec le centre du torse comment référence/origine
         */
        void setPos(const Vector2f &pos);
        void setPos(float x, float y);

        /**
         * @brief renvoie l'endroite ou regarde le character
         */
        Vector2f getLookingPoint() const;

        /**
         * @brief Fait regarder le character au coordonnées lp
         */
        void lookAt(const Vector2f &lp);

        /**
         * @brief Déplace le personnage vers la gauche
         *
         * @param newtonDiff Permet d'ajouter ou de soustraire de la force au
         * déplacement
         */
        void goLeft(float newtonDiff = 0.f);

        /**
         * @brief Déplace le personnage vers la droite
         *
         * @param newtonDiff Permet d'ajouter ou de soustraire de la force au
         * déplacement
         */
        void goRight(float newtonDiff = 0.f);

        void jump();

        /**
         * @brief retourne le character sur l'axe y
         */
        void flipY();


        Member &getLeftArm();
        Member &getRightArm();
        Member &getBody();
        Member &getHead();
        Member &getLegs();
        [[nodiscard]] float getHp() const;
        [[nodiscard]] Gun &getGun() const;
        [[nodiscard]] bool isTouchingDown() const;

        void setHp(float _hp);
        void setTouchingDown(bool touchingDown);

        // ENTITY HERITAGE gestion de la physique
        void computePhysic(b2World *world) override;
        void updatePhysic() override;

        /**
         * @brief Met le personnage sur le mode ragdoll
         * @warning Dans ce mode le joueur perd le controle du personnage
         *
         * @param world Monde box2D
         */
        void toggleRagdollMod(b2World *world);
};

#endif