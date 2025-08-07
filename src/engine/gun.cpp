#include "gun.h"

#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <cmath>
#include <memory>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <vector>

#include "../game/game.h"
#include "bullet.h"
#include "macro.h"

Gun::Gun() : Item(Texture("assets/default.png")) {
        // arme par default
        setSize(64, 32);
        setName("default-gun");
        lastShotTime = glfwGetTime() - rearmTime;
        gunMouth = Vector2f(20.f, 5.f);
        gunDirection = rightDir;
        state = GunState::idle;
        bulletType = "5-56x45mm";
}

Gun::Gun(const std::string &path) : Gun() {
        animation = std::make_unique<AnimationComponent<GunState>>(this);
        audio = std::make_unique<AudioComponent<GunState>>(this);

        loadJsonFile(path);
}

void Gun::loadJsonFile(const std::string &path) {
        // Chargement des sons dans AudioComponent
        if (auto specificPtr =
                dynamic_cast<AudioComponent<GunState> *>(audio.get())) {
                for (int i = 0; i <= static_cast<int>(GunState::reload); ++i) {
                        const auto state = static_cast<GunState>(i);
                        specificPtr->loadSound(state, path);
                }
        }

        // Chargement des textures dans AnimationComponent
        if (auto specificPtr =
                dynamic_cast<AnimationComponent<GunState> *>(animation.get())) {
                for (int i = 0; i <= static_cast<int>(GunState::reload); ++i) {
                        const auto state = static_cast<GunState>(i);
                        specificPtr->loadTexture(state, path);
                }
        }

        // Après avoir chargé audio et animation,
        // on charge les caractèristique de l'arme.
        std::string jsonPath = path + "gun.json";
        std::ifstream jsonFile(jsonPath);
        if (!jsonFile.is_open()) {
                ERROR_MESSAGE("Chargement fichier json " + path);
                return;
        }

        // loading with json
        nlohmann::json jsonData;
        try {
                jsonFile >> jsonData;
        } catch (const std::exception &e) {
                ERROR_MESSAGE("Découpage json " + path);
                return;
        }

        try {
                // chargement des infos de l'arme
                setName(jsonData.at("name"));
                magazineSize = jsonData.at("magazine_size");
                ammo = magazineSize;
                rearmTime = jsonData.at("ream_time");
                bulletSpeed = jsonData.at("bullet_speed");
                bulletType = jsonData.at("bullet_type");
                semiAuto = jsonData.at("is_semi_automatic");

                float x = jsonData.at("mouth_x");
                float y = jsonData.at("mouth_y");
                gunMouth.x = x;
                gunMouth.y = y;
        } catch (const nlohmann::json::exception &e) {
                ERROR_MESSAGE("Recupération caractéristique " + jsonPath);
                return;
        }
}

void Gun::update() {
        // Play animation
        // Cast std::unique_ptr<IAnimationComponent> de Item
        // vers AnimationComponent<GunState>* pour jouer les
        // anims
        if (const auto specificPtr =
                dynamic_cast<AnimationComponent<GunState> *>(animation.get())) {
                if (specificPtr->play(state)) { // dès qu'une anim est fini on
                                                // retourne sur idle
                        state = GunState::idle;
                }
        }

        if (!armed) {
                if (glfwGetTime() - lastShotTime >= rearmTime && ammo > 0 &&
                    state == GunState::idle) {
                        armed = true;
                }
        }
}

void Gun::setAttachPoint(const Vector2f &ap) {
        attachPoint = ap;
        setPosition(ap);
}

void Gun::setAttachPoint(float x, float y) { setAttachPoint(Vector2f(x, y)); }

void Gun::shoot(Game *game, bool mouseHolded) {
        if (!armed || (semiAuto && mouseHolded))
                return;

        lastShotTime = glfwGetTime();
        ammo--;
        armed = false;
        state = GunState::shoot;

        // creation de la balle
        Vector2f mouth(getPosition().x + gunMouth.x,
                       getPosition().y +
                           gunMouth.y); // position bouche du canon

        // ajout d'un peu d'aléatoire dans la direction
        std::uniform_real_distribution<float> distribution(-0.1f, 0.1f);
        const float r = distribution(RANDOM_ENGINE);
        const float rotation = getRotation() + r; // rotation de l'arme

        Vector2f inertie(cos(rotation) * bulletSpeed,
                         sin(rotation) *
                             bulletSpeed); // vecteur d'inertie en fonction de
                                           // la rotaion du canon

        if (gunDirection == leftDir) {
                inertie = Vector2f(cos(rotation) * -bulletSpeed,
                                   sin(rotation) * -bulletSpeed);
                mouth = Vector2f(getPosition().x - gunMouth.x, mouth.y);
        }

        // calcule position de sortie de la balle
        const float outX = getPosition().x +
                           (mouth.x - getPosition().x) * cos(getRotation()) -
                           (mouth.y - getPosition().y) * sin(getRotation());
        const float outY = getPosition().y +
                           (mouth.x - getPosition().x) * sin(getRotation()) +
                           (mouth.y - getPosition().y) * cos(getRotation());

        const auto specPtr =
            dynamic_cast<Bullet *>(ItemFactory::getItem(bulletType));
        const auto b = new Bullet(*specPtr);

        // utilisr la ItemFActory ici avec bullet
        b->setPosition(outX, outY);
        b->setRotation(getRotation());

        if (gunDirection == leftDir)
                b->flipVertically();

        b->fire();
        b->computePhysic(game->getPhysicalWorld());

        b->getBody()->ApplyForceToCenter(b2Vec2(inertie.x, inertie.y), true);

        game->addItem(b);

        // Joue le sound du tir
        if (const auto specificPtr =
                dynamic_cast<AudioComponent<GunState> *>(audio.get()))
                specificPtr->play(state);
}

void Gun::reload() {
        if (state == GunState::reload)
                return;

        state = GunState::reload;
        ammo = magazineSize;

        // Joue le sound du reload
        if (const auto specificPtr =
                dynamic_cast<AudioComponent<GunState> *>(audio.get())) {
                specificPtr->play(state);
        }
}

GunState Gun::getState() const { return state; }

void Gun::setState(const GunState state) { this->state = state; }

Direction Gun::getGunDirection() const { return gunDirection; }

void Gun::setGunDirection(const Direction gunDirection) {
        this->gunDirection = gunDirection;
}

bool Gun::isArmed() const { return armed; }

void Gun::setArmed(const bool armed) { this->armed = armed; }

int Gun::getMagazineSize() const { return magazineSize; }

void Gun::setMagazineSize(const int magazineSize) {
        this->magazineSize = magazineSize;
}

int Gun::getAmmo() const { return ammo; }

void Gun::setAmmo(const int ammo) { this->ammo = ammo; }

Gun::Gun(const Gun &other)
    : Item(other), state(other.state), attachPoint(other.attachPoint),
      gunDirection(other.gunDirection), armed(other.armed),
      magazineSize(other.magazineSize), ammo(other.ammo),
      lastShotTime(other.lastShotTime), rearmTime(other.rearmTime),
      bulletSpeed(other.bulletSpeed), gunMouth(other.gunMouth),
      bulletType(other.bulletType), semiAuto(other.semiAuto) {

        // Changement de possèsseur du composant copier.
        // On cast en premier l'autre composant pour voir
        // si il correspond bien.
        // Puis on copie et change l'ownership.
        if (const auto specificPtr =
                dynamic_cast<AnimationComponent<GunState> *>(
                    other.animation.get())) {
                animation = std::make_unique<AnimationComponent<GunState>>(
                    *specificPtr); // copie
                dynamic_cast<AnimationComponent<GunState> *>(animation.get())
                    ->setOwner(this);
        }

        if (const auto specificPtr =
                dynamic_cast<AudioComponent<GunState> *>(other.audio.get())) {
                audio = std::make_unique<AudioComponent<GunState>>(
                    *specificPtr); // copie
                dynamic_cast<AudioComponent<GunState> *>(audio.get())
                    ->setOwner(this);
        }
}

Gun::Gun(Gun &&other) noexcept
    : Item(std::move(other)), state(other.state),
      attachPoint(other.attachPoint), gunDirection(other.gunDirection),
      armed(other.armed), magazineSize(other.magazineSize), ammo(other.ammo),
      lastShotTime(other.lastShotTime), rearmTime(other.rearmTime),
      bulletSpeed(other.bulletSpeed), gunMouth(other.gunMouth),
      bulletType(other.bulletType), semiAuto(other.semiAuto) {}

Gun &Gun::operator=(const Gun &other) {
        if (this == &other)
                return *this;
        Item::operator=(other);
        state = other.state;
        attachPoint = other.attachPoint;
        gunDirection = other.gunDirection;
        armed = other.armed;
        magazineSize = other.magazineSize;
        ammo = other.ammo;
        lastShotTime = other.lastShotTime;
        rearmTime = other.rearmTime;
        bulletSpeed = other.bulletSpeed;
        gunMouth = other.gunMouth;
        bulletType = other.bulletType;
        semiAuto = other.semiAuto;

        // Changement de possèsseur du composant copier.
        // On cast en premier l'autre composant pour voir
        // si il correspond bien.
        // Puis on copie et change l'ownership.
        if (const auto specificPtr =
                dynamic_cast<AnimationComponent<GunState> *>(
                    other.animation.get())) {
                animation = std::make_unique<AnimationComponent<GunState>>(
                    *specificPtr); // copie
                dynamic_cast<AnimationComponent<GunState> *>(animation.get())
                    ->setOwner(this);
        }

        if (const auto specificPtr =
                dynamic_cast<AudioComponent<GunState> *>(other.audio.get())) {
                audio = std::make_unique<AudioComponent<GunState>>(
                    *specificPtr); // copie
                dynamic_cast<AudioComponent<GunState> *>(audio.get())
                    ->setOwner(this);
        }
        return *this;
}

Gun &Gun::operator=(Gun &&other) noexcept {
        if (this == &other)
                return *this;
        Item::operator=(std::move(other));
        state = other.state;
        attachPoint = other.attachPoint;
        gunDirection = other.gunDirection;
        armed = other.armed;
        magazineSize = other.magazineSize;
        ammo = other.ammo;
        lastShotTime = other.lastShotTime;
        rearmTime = other.rearmTime;
        bulletSpeed = other.bulletSpeed;
        gunMouth = other.gunMouth;
        bulletType = other.bulletType;
        semiAuto = other.semiAuto;
        return *this;
}

void Gun::shoot() { return; }
