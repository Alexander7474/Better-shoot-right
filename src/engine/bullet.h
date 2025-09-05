#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics.h"

#include "item.h"

#include <string>

enum class BulletState { idle, fired, broken };

// TODO -- Ajouter un ficher de configuration json pour les balles pour
// configurer les caractéristiques physiques de celle ci
// TODO -- Ajouter un ttl au balle
class Bullet final : public Item {
      private:
        BulletState state;
        float damage;

      public:
        Bullet();
        Bullet(const std::string &path);

        void update() override;

        /**
         * @brief Détonne la munition
         * @param inertie Force de la balle
         */
        void fire();

        /**
         * @brief Casse le bullet
         * @details A utiliser après une collision
         */
        void broke();

        /**
         * @brief charge une balle depuis un fichier json
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
        float getDamage();
        void setDamage(float damage);

        const BulletState &getState() const;

        /**
         * @brief Héritage entity, compute le b2Body
         * @param world
         *
         * @details Rajoute dans userData un pointer vers this pour gérer
         * de potentiel collision
         */
        void computePhysic(b2World *world) override;
};
