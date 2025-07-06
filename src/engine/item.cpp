//
// Created by artyom on 7/4/25.
//

#include "item.h"

#include "gun.h"

Item::Item()
        : Sprite(Texture("assets/default.png"))
{}

Item::Item(const Texture& texture)
        : Sprite(texture)
{}

void Item::update() {

}

Item::Item(const Item &other)
                : Sprite{other},
                  name{other.name}
{
        // copy du unique_ptr vers IAnimationComponent
        if (other.animation)
                animation = std::make_unique<IAnimationComponent>(*other.animation);
}

Item::Item(Item &&other) noexcept
        : Sprite{std::move(other)},
          name{std::move(other.name)},
          animation{std::move(other.animation)} {
}

Item & Item::operator=(const Item &other) {
        if (this == &other)
                return *this;
        Sprite::operator =(other);
        name = other.name;
        // copy du unique_ptr vers IAnimationComponent
        if (other.animation)
                animation = std::make_unique<IAnimationComponent>(*other.animation);
        return *this;
}

Item & Item::operator=(Item &&other) noexcept {
        if (this == &other)
                return *this;
        Sprite::operator =(std::move(other));
        name = std::move(other.name);
        animation = std::move(other.animation);
        return *this;
}

const std::string &Item::getName() const { return name; }

void Item::setName(const std::string &name) { this->name = name; }

std::unordered_map<std::string, std::unique_ptr<Item>> ItemFactory::allItems;

void ItemFactory::loadAllItems() {
        const std::string t = "scar";
        allItems[t] = std::make_unique<Gun>();
}

Item *ItemFactory::getItem(const std::string &name) {
        Item *it = allItems[name].get();
        return it;
}
