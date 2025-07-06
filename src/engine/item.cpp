//
// Created by artyom on 7/4/25.
//

#include "item.h"

#include "gun.h"

#include "physic.h"

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

void Item::computePhysic(b2World *world) {
        setOrigin(getSize().x / 2, getSize().y / 2);
        // une fois ajouter a umonde box2d c'est la galère pour récupérer la taille
        // donc faut faire attention
        entityBody = addDynamicBox(world, &getCollisionBox(), 0.f, 1.f, 1.f, 1.f, false);
}

void Item::updatePhysic() {
        setPosition(entityBody->GetPosition().x * PIXEL_PER_METER,
                    entityBody->GetPosition().y * PIXEL_PER_METER);
        setRotation(entityBody->GetAngle());
}

std::unordered_map<std::string, std::unique_ptr<Item>> ItemFactory::allItems;
bool ItemFactory::initialized = false;

void ItemFactory::loadAllItems() {
        allItems["scar"] = std::make_unique<Gun>("assets/guns/scar/");
        allItems["ak47"] = std::make_unique<Gun>("assets/guns/ak47/");
        allItems["sniper"] = std::make_unique<Gun>("assets/guns/sniper/");
        allItems["default"] = std::make_unique<Item>();
        allItems["default"]->setSize(64,32);
        initialized = true;
}

Item *ItemFactory::getItem(const std::string &name) {
        if (!initialized) {
                LOGS.push_back("ItemFactory uninitialized !");
                return nullptr;
        }
        Item *it = allItems[name].get();
        return it;
}
