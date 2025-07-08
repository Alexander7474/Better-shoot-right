#include "gun.h"

#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../game/game.h"
#include "bullet.h"
#include "macro.h"

Gun::Gun() : Item(Texture("assets/default.png")) {
        // arme par default
        setSize(64,32);
        setName("default-gun");
        armed = false;
        rearmTime = 0;
        magazineSize = 0;
        ammo = 0;
        lastShotTime = glfwGetTime() - rearmTime;
        damage = 0.f;
        gunMouth = Vector2f(20.f, 5.f);
        bulletSpeed = 0.f;
        gunDirection = rightDir;

        // texture par default des balles
        bulletTexture = std::make_unique<Texture>("assets/items/guns/bullets/default.png");
}

Gun::Gun(const std::string& path) : Gun() {
        state = GunState::idle;
        animation = std::make_unique<AnimationComponent<GunState>>(
            this);

        loadJsonFile(path);
}

void Gun::loadJsonFile(const string& path) {
        if (auto specificPtr = dynamic_cast<AnimationComponent<GunState>*>(animation.get())) {
                for (int i = 0; i < static_cast<int>(GunState::reload); ++i) {
                        const auto state = static_cast<GunState>(i);
                        specificPtr->loadTexture(state, path);
                }
        }

        // Après avoir toute les texture par default on
        // charge les textures personnalisé avec leur
        // fichier json.
        // Si pas de fichier json, il reste les texture
        // par défault.
        string jsonPath = path + "gun.json";
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
                damage = jsonData.at("damage");
                magazineSize = jsonData.at("magazine_size");
                ammo = magazineSize;
                rearmTime = jsonData.at("ream_time");
                bulletSpeed = jsonData.at("bullet_speed");

                string sound = jsonData.at("shot_sound");
                // gunShotSound =
                // soundEngine->addSoundSourceFromFile(sound.c_str());

                string bullet = jsonData.at("bullet_texture");
                bulletTexture = std::make_unique<Texture>(bullet.c_str());

                float x = jsonData.at("mouth_x");
                float y = jsonData.at("mouth_y");
                gunMouth.x = x;
                gunMouth.y = y;
        } catch (const nlohmann::json::exception &e) {
                ERROR_MESSAGE("Recupération " + path);
                return;
        }
}

void Gun::update() {

        // Play animation
        // Cast std::unique_ptr<IAnimationComponent> de Item
        // vers AnimationComponent<GunState>* pour jouer les
        // anims
        if (auto specificPtr = dynamic_cast<AnimationComponent<GunState>*>(animation.get())) {
                if (specificPtr->play(state)) {
                        state = GunState::idle;
                }
        }

        if (!armed) {
                if (glfwGetTime() - lastShotTime >= rearmTime && ammo > 0) {
                        armed = true;
                }
        }

        for (Bullet &b : bulletVector) {
                b.update();
        }
}

void Gun::setAttachPoint(const Vector2f& ap) {
        attachPoint = ap;
        setPosition(ap);
}

void Gun::setAttachPoint(float x, float y) { setAttachPoint(Vector2f(x, y)); }

void Gun::shoot() {
        if (armed) {
                lastShotTime = glfwGetTime();
                ammo--;
                armed = false;
                state = GunState::shoot;

                // creation de la balle
                Vector2f mouth(
                    getPosition().x + gunMouth.x,
                    getPosition().y + gunMouth.y);  // position bouche du canon

                // ajout d'un peu d'aléatoire dans la direction
                uniform_real_distribution<float> distribution(-0.1f, 0.1f);
                const float r = distribution(RANDOM_ENGINE);
                const float rotation = getRotation() + r;  // rotation de l'arme

                Vector2f inertie(
                    cos(rotation) * bulletSpeed,
                    sin(rotation) *
                        bulletSpeed);  // vecteur d'inertie en fonction de la
                                       // rotaion du canon

                if (gunDirection == leftDir) {
                        inertie = Vector2f(cos(rotation) * -bulletSpeed,
                                           sin(rotation) * -bulletSpeed);
                        mouth = Vector2f(getPosition().x - gunMouth.x, mouth.y);
                }

                // calcule position de sortie de la balle
                const float outX =
                    getPosition().x +
                    (mouth.x - getPosition().x) * cos(getRotation()) -
                    (mouth.y - getPosition().y) * sin(getRotation());
                const float outY =
                    getPosition().y +
                    (mouth.x - getPosition().x) * sin(getRotation()) +
                    (mouth.y - getPosition().y) * cos(getRotation());

                Bullet b(bulletTexture.get(), inertie);
                b.setPosition(outX, outY);
                b.setRotation(getRotation());

                if (gunDirection == leftDir) b.flipVertically();

                bulletVector.push_back(b);

                // Chargement du fichier audio (wav, mp3, ogg, etc.)
                Mix_Chunk* sound = Mix_LoadWAV("assets/audio/short-drill-gunshot-fx.wav");
                if (!sound) {
                        std::cerr << "Erreur Mix_LoadWAV : " << Mix_GetError() << std::endl;
                        Mix_CloseAudio();
                        SDL_Quit();
                        return;
                }

                // Jouer le son une fois (-1 pour boucle infinie)
                Mix_PlayChannel(0, sound, 0);
        }
}

void Gun::reload() { ammo = magazineSize; }

GunState Gun::getState() const { return state; }

void Gun::setState(const GunState state) { this->state = state; }

Direction Gun::getGunDirection() const { return gunDirection; }

void Gun::setGunDirection(const Direction gunDirection) {
        this->gunDirection = gunDirection;
}

float Gun::getDamage() const { return damage; }

void Gun::setDamage(const float damage) { this->damage = damage; }

bool Gun::isArmed() const { return armed; }

void Gun::setArmed(const bool armed) { this->armed = armed; }

int Gun::getMagazineSize() const { return magazineSize; }

void Gun::setMagazineSize(const int magazineSize) {
        this->magazineSize = magazineSize;
}

int Gun::getAmmo() const { return ammo; }

void Gun::setAmmo(const int ammo) { this->ammo = ammo; }

const std::vector<Bullet> &Gun::getBulletVector() const { return bulletVector; }

void Gun::setBulletVector(const std::vector<Bullet> &bulletVector) {
        this->bulletVector = bulletVector;
}

Gun::Gun(const Gun &other)
        : Item(other),
          state(other.state),
          attachPoint(other.attachPoint),
          gunDirection(other.gunDirection),
          damage(other.damage),
          armed(other.armed),
          magazineSize(other.magazineSize),
          ammo(other.ammo),
          lastShotTime(other.lastShotTime),
          rearmTime(other.rearmTime),
          bulletVector(other.bulletVector),
          bulletSpeed(other.bulletSpeed),
          gunMouth(other.gunMouth)
{

        // copy du unique_ptr Texture
        if (other.bulletTexture)
                bulletTexture = std::make_unique<Texture>(*other.bulletTexture);

        // Changement de possèsseur du composant copier.
        // On cast en premier l'autre composant pour voir
        // si il correspond bien.
        // Puis on copie et change l'ownership.
        if (const auto specificPtr = dynamic_cast<AnimationComponent<GunState>*>(other.animation.get())) {
                animation = std::make_unique<AnimationComponent<GunState>>(*specificPtr); // copie
                dynamic_cast<AnimationComponent<GunState>*>(animation.get())->setOwner(this);
        }
}

Gun::Gun(Gun &&other) noexcept
        : Item(std::move(other)),
          state(other.state),
          attachPoint(other.attachPoint),
          gunDirection(other.gunDirection),
          bulletTexture(std::move(other.bulletTexture)),
          damage(other.damage),
          armed(other.armed),
          magazineSize(other.magazineSize),
          ammo(other.ammo),
          lastShotTime(other.lastShotTime),
          rearmTime(other.rearmTime),
          bulletVector(std::move(other.bulletVector)),
          bulletSpeed(other.bulletSpeed),
          gunMouth(other.gunMouth)
{
}

Gun & Gun::operator=(const Gun &other) {
        if (this == &other)
                return *this;
        Item::operator =(other);
        state = other.state;
        attachPoint = other.attachPoint;
        gunDirection = other.gunDirection;
        damage = other.damage;
        armed = other.armed;
        magazineSize = other.magazineSize;
        ammo = other.ammo;
        lastShotTime = other.lastShotTime;
        rearmTime = other.rearmTime;
        bulletVector = other.bulletVector;
        bulletSpeed = other.bulletSpeed;
        gunMouth = other.gunMouth;
        // copy du unique_ptr Texture
        if (other.bulletTexture)
                bulletTexture = std::make_unique<Texture>(*other.bulletTexture);
        // changement de possèsseur du composant copier
        if (auto specificPtr = dynamic_cast<AnimationComponent<GunState>*>(animation.get())) {
                specificPtr->setOwner(this);
        }
        return *this;
}

Gun & Gun::operator=(Gun &&other) noexcept {
        if (this == &other)
                return *this;
        Item::operator =(std::move(other));
        state = other.state;
        attachPoint = other.attachPoint;
        gunDirection = other.gunDirection;
        bulletTexture = std::move(other.bulletTexture);
        damage = other.damage;
        armed = other.armed;
        magazineSize = other.magazineSize;
        ammo = other.ammo;
        lastShotTime = other.lastShotTime;
        rearmTime = other.rearmTime;
        bulletVector = std::move(other.bulletVector);
        bulletSpeed = other.bulletSpeed;
        gunMouth = other.gunMouth;
        return *this;
}