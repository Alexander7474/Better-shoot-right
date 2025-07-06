#pragma once

#include <memory>
#include <string>

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "animationComponent.h"
#include "bullet.h"
#include "item.h"

enum class GunState { idle, shoot, reload };

#ifndef DIRECTION
#define DIRECTION

enum Direction { rightDir, leftDir };

#endif


class Gun : public Item {
       private:
        GunState state;

        Vector2f attachPoint;
        Direction gunDirection;

        // ressource
        std::unique_ptr<Texture> bulletTexture;  // Pointeur vers la texture des balles pour la
                                                 // charger une seule fois
        // gestion des tirs
        float damage;         // degat de l'arme
        bool armed;           // l'arme est armé ?
        int magazineSize;     // taille du chargeur
        int ammo;             // nombre de mun dans le chargeur
        double lastShotTime;  // moment du dernier tire
        double rearmTime;     // temps pour réarmer l'arme
        std::vector<Bullet>
            bulletVector;   // stock le balle de l'arme en cours "d'utilisation"
        float bulletSpeed;  // vitesse des balles
        Vector2f gunMouth;  // sortie des balles

       public:
        Gun();

        void update() override;

        void setAttachPoint(Vector2f ap);
        void setAttachPoint(float x, float y);

        /**
         * @brief fais tirer l'arme
         */
        void shoot();

        /**
         * remplie le nombre de munition dans le chargeur
         */
        void reload();

        /**
         * @brief charge une arme depuis un fichier json
         */
        void loadJsonFile(const std::string &path);

        [[nodiscard]] GunState getState() const;

        void setState(GunState state);

        [[nodiscard]] Direction getGunDirection() const;

        void setGunDirection(Direction gunDirection);

        float getDamage() const;

        void setDamage(float damage);

        bool isArmed() const;

        void setArmed(bool armed);

        int getMagazineSize() const;

        void setMagazineSize(int magazineSize);

        int getAmmo() const;

        void setAmmo(int ammo);

        const std::vector<Bullet> &getBulletVector() const;

        void setBulletVector(const std::vector<Bullet> &bulletVector);

        Gun(const Gun &other);
};
