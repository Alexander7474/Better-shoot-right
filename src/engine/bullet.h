#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics.h"

#include "item.h"

#include <string>

enum class BulletState {
        idle,
        fired
};

// TODO -- Ajouter un ficher de configuration json pour les balles pour configurer les caractéristiques physiques de celle ci
// TODO -- Ajouter un ttl au balle
class Bullet final: public Item {
      private:
        Vector2f inertie;
        BulletState state;
      public:
        Bullet();
        Bullet(const std::string& path);

        void update() override;

        /**
         * @brief Détonne la munition
         * @param inertie Force de la balle
         */
        void fire(const Vector2f& inertie);

        /**
        * @brief charge une arme depuis un fichier json
        */
        void loadJsonFile(const std::string &path);

        /**
         * @brief Mem
         * @param other
         */
        Bullet(const Bullet &other);
        Bullet(Bullet &&other) noexcept;
        Bullet &operator=(const Bullet &other);
        Bullet &operator=(Bullet &&other) noexcept;

        b2Body *getBody() const;

        /**
        * @brief Héritage entity, compute le b2Body
        * @param world
        *
        * @details Rajoute dans userData un pointer vers this pour gérer
        * de potentiel collision
        */
        void computePhysic(b2World *world) override;
};
