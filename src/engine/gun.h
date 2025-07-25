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

class Game;

// TODO -- Ajouter un état de réarmement
// TODO -- Ajouter une chute de douille au arme
class Gun final : public Item {
      private:
        GunState state;

        Vector2f attachPoint;
        Direction gunDirection;

        // gestion des tirs
        bool armed{};          // l'arme est armé ?
        int magazineSize{};    // taille du chargeur
        int ammo{};            // nombre de mun dans le chargeur
        double lastShotTime{}; // moment du dernier tire
        double rearmTime{};    // temps pour réarmer l'arme
        float bulletSpeed{}; // vitesse des balles
        Vector2f gunMouth; // sortie des balles
        std::string bulletType; // nom du type de balle (Item de ItemFactory)
	bool semiAuto{};

      public:
        Gun();
        Gun(const std::string &path);

        void update() override;

        void setAttachPoint(const Vector2f &ap);
        void setAttachPoint(float x, float y);

        /**
         * @brief fais tirer l'arme
	 *
	 * @param game pointer vers la game 
	 * @param mouseHold La souris vien d'être clické ?
         */
        void shoot(Game *game, bool mouseHolded);
	
	// TODO -- enlever cette fonction qui sert uniquement a trooper
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

        bool isArmed() const;

        void setArmed(bool armed);

        int getMagazineSize() const;

        void setMagazineSize(int magazineSize);

        int getAmmo() const;

        void setAmmo(int ammo);

        Gun(const Gun &other);

        Gun(Gun &&other) noexcept;

        Gun &operator=(const Gun &other);

        Gun &operator=(Gun &&other) noexcept;
};
