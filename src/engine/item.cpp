//
// Created by artyom on 7/4/25.
//

#include "item.h"

#include "gun.h"

#include "physic.h"

#include "macro.h"

#include <nlohmann/json.hpp>

Item::Item() : Sprite(Texture("assets/default.png")) { setSize(10, 10); }

Item::Item(const Texture &texture) : Sprite(texture) {}

void Item::update() {}

Item::Item(const Item &other) : Sprite{other}, name{other.name} {
        // copy des composants
        if (other.animation)
                animation =
                    std::make_unique<IAnimationComponent>(*other.animation);
        if (other.audio)
                audio = std::make_unique<IAudioComponent>(*other.audio);
}

Item::Item(Item &&other) noexcept
    : Sprite{std::move(other)}, name{std::move(other.name)},
      animation{std::move(other.animation)}, audio{std::move(other.audio)} {}

Item &Item::operator=(const Item &other) {
        if (this == &other)
                return *this;
        Sprite::operator=(other);
        name = other.name;
        // copy du unique_ptr vers IAnimationComponent
        if (other.animation)
                animation =
                    std::make_unique<IAnimationComponent>(*other.animation);
        if (other.audio)
                audio = std::make_unique<IAudioComponent>(*other.audio);
        return *this;
}

Item &Item::operator=(Item &&other) noexcept {
        if (this == &other)
                return *this;
        Sprite::operator=(std::move(other));
        name = std::move(other.name);
        animation = std::move(other.animation);
        audio = std::move(other.audio);
        return *this;
}

const std::string &Item::getName() const { return name; }

void Item::setName(const std::string &name) { this->name = name; }

void Item::computePhysic(b2World *world) {
        setOrigin(getSize().x / 2, getSize().y / 2);
        // une fois ajouter a umonde box2d c'est la galère pour récupérer la
        // taille donc faut faire attention
        entityBody =
            addDynamicBox(world, &getCollisionBox(), 0.f, 1.f, 1.f, 1.f, false);
}

void Item::updatePhysic() {
        setPosition(entityBody->GetPosition().x * PIXEL_PER_METER,
                    entityBody->GetPosition().y * PIXEL_PER_METER);
        setRotation(entityBody->GetAngle());
}

std::unordered_map<std::string, std::unique_ptr<Item>> ItemFactory::allItems;
bool ItemFactory::initialized = false;

void ItemFactory::loadAllItems() {
        DEBUG_MESSAGE("Appelle ItemFactory::loadAllItems");
        std::string jsonPath = "assets/items/items.json";
        std::ifstream jsonFile(jsonPath);
        if (!jsonFile.is_open()) {
                ERROR_MESSAGE("Impossible d'ouvrir les items " + jsonPath);
                return;
        }

        // loading with json
        nlohmann::json jsonData;
        try {
                jsonFile >> jsonData;
        } catch (const std::exception &e) {
                ERROR_MESSAGE("Erreur parsing json pour " + jsonPath);
                return;
        }

        // Chargement des Guns
        for (auto &[name, path] : jsonData["guns"].items()) {
                allItems[name] = std::make_unique<Gun>(path);
        }

        // Chargement des Items
        for (auto &[name, path] : jsonData["items"].items()) {
                allItems[name] = std::make_unique<Item>(
                    Texture(path.get<std::string>().c_str()));
        }

        // Item par default
        allItems["default"] = std::make_unique<Item>();

        initialized = true;
}

Item *ItemFactory::getItem(const std::string &name) {
        DEBUG_MESSAGE("Appelle ItemFactory::getItem");
        if (!initialized) {
                ERROR_MESSAGE("ItemFactory::getItem appelé mais ItemFActory "
                              "non initializé");
                return nullptr;
        }
        Item *it = allItems[name].get();
        return it;
}
