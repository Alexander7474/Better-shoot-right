#include "bullet.h"
#include "macro.h"
#include "physic.h"
#include "../game/game.h"

using namespace std;

Bullet::Bullet() : Item(Texture("assets/default.png")) {
        setSize(2,1);
        state = BulletState::idle;
}

Bullet::Bullet(const std::string &path) : Bullet() {
        animation = std::make_unique<AnimationComponent<BulletState>>(this);
        audio = std::make_unique<AudioComponent<BulletState>>(this);
        loadJsonFile(path);
}


void Bullet::update() {
        // Play animation
        // Cast std::unique_ptr<IAnimationComponent> de Item
        // vers AnimationComponent<BulletState>* pour jouer les
        // anims
        if (const auto specificPtr = dynamic_cast<AnimationComponent<BulletState> *>(animation.get())) {
                specificPtr->play(state);
        }
}

void Bullet::fire() {
	state = BulletState::fired;
}

void Bullet::broke() {
	state = BulletState::broken;

}

void Bullet::loadJsonFile(const std::string &path) {
        // Chargement des sons dans AudioComponent
        if (const auto specificPtr = dynamic_cast<AudioComponent<BulletState> *>(audio.get())) {
                for (int i = 0; i <= static_cast<int>(BulletState::broken); ++i) {
                        const auto state = static_cast<BulletState>(i);
                        specificPtr->loadSound(state, path);
                }
        }

        // Chargement des textures dans AnimationComponent
        if (const auto specificPtr = dynamic_cast<AnimationComponent<BulletState> *>(animation.get())) {
                for (int i = 0; i <= static_cast<int>(BulletState::broken); ++i) {
                        const auto state = static_cast<BulletState>(i);
                        specificPtr->loadTexture(state, path);
                }
        }
}

Bullet::Bullet(const Bullet &other)
           : Item(other),
             state(other.state) {

        // Changement de possèsseur du composant copier.
        // On cast en premier l'autre composant pour voir
        // si le type correspond bien.
        // Puis on copie et change l'ownership.
        if (const auto specificPtr =
                dynamic_cast<AnimationComponent<BulletState> *>(
                    other.animation.get())) {
                animation = std::make_unique<AnimationComponent<BulletState>>(
                    *specificPtr); // copie
                dynamic_cast<AnimationComponent<BulletState> *>(animation.get())
                    ->setOwner(this);
                    }

        if (const auto specificPtr =
                dynamic_cast<AudioComponent<BulletState> *>(other.audio.get())) {
                audio = std::make_unique<AudioComponent<BulletState>>(
                    *specificPtr); // copie
                dynamic_cast<AudioComponent<BulletState> *>(audio.get())
                    ->setOwner(this);
                }
}

Bullet::Bullet(Bullet &&other) noexcept
        : Item(std::move(other)),
          state(other.state) {

        // Changement de possèsseur du composant déplacer.
        // On cast en premier l'autre composant pour voir
        // si le type correspond bien.
        // Puis on le déplace et change l'ownership.
        if (dynamic_cast<AnimationComponent<BulletState> *>(other.animation.get())) {
                // on déplace le pointeur unique
                animation = std::move(other.animation);

                // le composant pointe maintenant vers this comme owner
                if (animation) {
                        dynamic_cast<AnimationComponent<BulletState> *>(animation.get())
                            ->setOwner(this);
                }
        }

        if (dynamic_cast<AudioComponent<BulletState> *>(other.audio.get())) {
                // on déplace le pointeur unique
                audio = std::move(other.audio);

                // le composant pointe maintenant vers this comme owner
                if (audio) {
                        dynamic_cast<AudioComponent<BulletState> *>(audio.get())
                            ->setOwner(this);
                }
        }
}

Bullet & Bullet::operator=(const Bullet &other) {
        if (this == &other)
                return *this;
        Item::operator =(other);
        state = other.state;

        // Changement de possèsseur du composant copier.
        // On cast en premier l'autre composant pour voir
        // si il correspond bien.
        // Puis on copie et change l'ownership.
        if (const auto specificPtr =
                dynamic_cast<AnimationComponent<BulletState> *>(
                    other.animation.get())) {
                animation = std::make_unique<AnimationComponent<BulletState>>(
                    *specificPtr); // copie
                dynamic_cast<AnimationComponent<BulletState> *>(animation.get())
                    ->setOwner(this);
                    }

        if (const auto specificPtr =
                dynamic_cast<AudioComponent<BulletState> *>(other.audio.get())) {
                audio = std::make_unique<AudioComponent<BulletState>>(
                    *specificPtr); // copie
                dynamic_cast<AudioComponent<BulletState> *>(audio.get())
                    ->setOwner(this);
                }
        
        return *this;
}

Bullet & Bullet::operator=(Bullet &&other) noexcept {
        if (this == &other)
                return *this;
        Item::operator =(std::move(other));
        state = other.state;

        // Changement de possèsseur du composant déplacer.
        // On cast en premier l'autre composant pour voir
        // si le type correspond bien.
        // Puis on le déplace et change l'ownership.
        if (dynamic_cast<AnimationComponent<BulletState> *>(other.animation.get())) {
                // on déplace le pointeur unique
                animation = std::move(other.animation);

                // le composant pointe maintenant vers this comme owner
                if (animation) {
                        dynamic_cast<AnimationComponent<BulletState> *>(animation.get())
                            ->setOwner(this);
                }
        }

        if (dynamic_cast<AudioComponent<BulletState> *>(other.audio.get())) {
                // on déplace le pointeur unique
                audio = std::move(other.audio);

                // le composant pointe maintenant vers this comme owner
                if (audio) {
                        dynamic_cast<AudioComponent<BulletState> *>(audio.get())
                            ->setOwner(this);
                }
        }

        return *this;
}

b2Body *Bullet::getBody() const { return entityBody; }

const BulletState &Bullet::getState() const { return state; }

void Bullet::computePhysic(b2World *world) {
	setOrigin(getSize().x / 2, getSize().y / 2);
        entityBody =
            addDynamicBox(world, &getCollisionBox(), 0.f, 1.f, 1.f, 1.f, false, true);

        // Ajout du pointeur userData
        auto *data = new BodyData;
        data->type = BodyType::Bullet;
        data->ptr = reinterpret_cast<uintptr_t>(this);
        entityBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}
