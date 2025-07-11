#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "../engine/dynamicSprite.h"
#include "entity.h"
#include "player.h"
#include "trooper.h"

#include <GLFW/glfw3.h>
#include <box2d/box2d.h>
#include <random>
#include <vector>

// pointeur vers la fentre de jeu utilisable de partout
extern GLFWwindow *gameWindow;

// Variable global du moteur de jeu
extern double DELTA_TIME;
extern double FPS;
extern double FPS_COUNTER;
extern double LAST_FPS_UPDATE;
extern float GRAVITY;
extern std::default_random_engine RANDOM_ENGINE;

class Game {
      private:
        Scene scene;
        Camera mainPlayerCam;
        Player mainPlayer{};
        Map map;
        std::vector<Trooper *> npc;
        std::vector<std::unique_ptr<DynamicSprite>>
            dynamics; // liste des sprites dynamics
        std::vector<std::unique_ptr<Item>> items;

        // gestion physique
        b2World physicalWorld;
        std::vector<Entity *> entities; // liste des enititées
      public:
        Game();

        /**
         * @brief Met a jour la game
         */
        void update();

        /**
         * @brief Dessine la game avec la scene
         */
        void Draw();

        /**
         * @brief Rajoute un item dans la game
         * @param item pointeur vers l'item
         *
         * @details Un unique_ptr<Item> du vector items va prendre possession
         * de l'objet pointer par *item.
         * @details La méthode se charge de mettre en place la class Entity
         * avec computePhysic()
         *
         * @warning L'item dois être parfaitement construit et près à être
         * utilisé en tant que telle, C'EST A DIRE UN ITEMS (pas un gun, pas
         * un bullets, UN ITEM).
         * @warning Ne pas donner une entité déjà initialisé avec computePhysic()
         */
        void addItem(Item *item);

        b2World *getPhysicalWorld();
};
