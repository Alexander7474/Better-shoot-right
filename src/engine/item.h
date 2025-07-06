//
// Created by artyom on 7/4/25.
//

#ifndef ITEM_H
#define ITEM_H

#include <memory>

#include <unordered_map>

#include "animationComponent.h"

/**
 * @class Item
 * @brief Class de base pour tous les items du jeu
 *
 */
class Item : public Sprite {
private:
        std::string name;
protected:
        std::unique_ptr<IAnimationComponent> animation; //<! permet de stocker un AnimationComponent<EnumState> pour certain item
public:
        explicit Item(const Texture& texture);
        explicit Item();

        virtual void update();

        /**
         * @brief Opération de copy et de move
         * @param other
         */
        Item(const Item &other);
        Item(Item &&other) noexcept;
        Item & operator=(const Item &other);
        Item &operator=(Item &&other) noexcept;

        const std::string &getName() const;

        void setName(const std::string &name);
};

/**
 * @brief Gère la création d'item pour éviter de surcharger les appelles au disque dur
 * @details Charge tous les items du jeu dans leurs états par défault et génére des copies
 */
class ItemFactory final {
private:
        static std::unordered_map<std::string, std::unique_ptr<Item>> allItems; //<! map de tous les items du jeu liés à leur nom
public:
        /**
         * @brief charge tous les items du jeu
         */
        static void loadAllItems();

        /**
         * @brief donne un item
         */
        static Item *getItem(const std::string &name);
};

#endif //ITEM_H
