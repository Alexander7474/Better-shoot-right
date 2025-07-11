#include "bullet.h"

#include "macro.h"
#include "../game/game.h"

using namespace std;

Bullet::Bullet() : Item(Texture("assets/default.png")) {
        setSize(8,2);
        inertie = Vector2f(0,0);
        state = BulletState::idle;
}

Bullet::Bullet(const std::string &path) : Bullet() {
        animation = std::make_unique<AnimationComponent<BulletState>>(this);
        audio = std::make_unique<AudioComponent<BulletState>>(this);
        loadJsonFile(path);
}


void Bullet::update() {

        if (state == BulletState::fired) {
                move(inertie.x * DELTA_TIME, inertie.y * DELTA_TIME);
        }

        // Play animation
        // Cast std::unique_ptr<IAnimationComponent> de Item
        // vers AnimationComponent<BulletState>* pour jouer les
        // anims
        if (const auto specificPtr = dynamic_cast<AnimationComponent<BulletState> *>(animation.get())) {
                specificPtr->play(state);
        }
}

void Bullet::fire(const Vector2f& inertie) {
        if (state != BulletState::fired) {
                this->inertie = inertie;
                state = BulletState::fired;
        }

}

void Bullet::loadJsonFile(const std::string &path) {
        // Chargement des sons dans AudioComponent
        if (const auto specificPtr = dynamic_cast<AudioComponent<BulletState> *>(audio.get())) {
                for (int i = 0; i <= static_cast<int>(BulletState::fired); ++i) {
                        const auto state = static_cast<BulletState>(i);
                        specificPtr->loadSound(state, path);
                }
        }

        // Chargement des textures dans AnimationComponent
        if (const auto specificPtr = dynamic_cast<AnimationComponent<BulletState> *>(animation.get())) {
                for (int i = 0; i <= static_cast<int>(BulletState::fired); ++i) {
                        const auto state = static_cast<BulletState>(i);
                        specificPtr->loadTexture(state, path);
                }
        }
}

Bullet::Bullet(const Bullet &other)
           : Item(other),
             inertie(other.inertie),
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
          inertie(other.inertie),
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
        inertie = other.inertie;
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
        inertie = other.inertie;
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
