//
// Created by artyom on 7/4/25.
//

#include "animationComponent.h"

#include <nlohmann/json.hpp>

#include "member.h"

#include "gun.h"

#include "macro.h"

template <typename AnimationEnum>
AnimationComponent<AnimationEnum>::AnimationComponent(Sprite *owner)
    : IAnimationComponent() {
        this->owner = owner;
}

template <typename AnimationEnum>
void AnimationComponent<AnimationEnum>::loadTexture(AnimationEnum state,
                                                    std::string path) {
        // on charge pour chaque anim la texture par default
        std::string defaultSheetPath = path + "default.png";

        animations[state].textures =
            bbopLoadSpriteSheet(defaultSheetPath.c_str(), 1, 1);
        animations[state].duration = 0.0;
        animations[state].nFrame = 1;
        animations[state].frameTime = 0.0;
        animations[state].lastFrameStartTime = 0.0;
        animations[state].startTime = 0.0;

        // Après avoir charger la texture par default
        // on charge la sprite sheet si le fichier json
        // l'à
        // Si pas de fichier json, il reste les texture par défault
        std::string jsonPath = path + "animations.json";
        std::ifstream jsonFile(jsonPath);
        if (!jsonFile.is_open()) {
                ERROR_MESSAGE("Impossible d'ouvrir les animations " + jsonPath +
                              " | state " +
                              std::to_string(static_cast<int>(state)));
                return;
        }

        // loading with json
        nlohmann::json jsonData;
        try {
                jsonFile >> jsonData;
        } catch (const std::exception &e) {
                ERROR_MESSAGE("Découpage json " + jsonPath + " | state " +
                              std::to_string(static_cast<int>(state)));
                return;
        }

        try {
                if (jsonData.contains(
                        std::to_string(static_cast<int>(state)))) {
                        std::string sheetPath =
                            jsonData.at(std::to_string(static_cast<int>(state)))
                                .at("sprite_sheet");
                        sheetPath = path + sheetPath;

                        int columns =
                            jsonData.at(std::to_string(static_cast<int>(state)))
                                .at("columns");
                        int rows =
                            jsonData.at(std::to_string(static_cast<int>(state)))
                                .at("rows");
                        int nFrame =
                            jsonData.at(std::to_string(static_cast<int>(state)))
                                .at("n_frame");

                        double duration =
                            jsonData.at(std::to_string(static_cast<int>(state)))
                                .at("duration");

                        animations[state].textures.clear();
                        animations[state].textures = bbopLoadSpriteSheet(
                            sheetPath.c_str(), rows, columns);
                        animations[state].duration = duration;
                        animations[state].nFrame = nFrame;
                        animations[state].frameTime = duration / nFrame;

                } else {
                        ERROR_MESSAGE("Chargement " + jsonPath + " | state " +
                                      std::to_string(static_cast<int>(state)));
                }
        } catch (const nlohmann::json::exception &e) {
                ERROR_MESSAGE("Recupération animation " + jsonPath + " | " +
                              e.what());
                return;
        }

        DEBUG_MESSAGE("Chargement terminé: " + jsonPath + " | state " +
                      std::to_string(static_cast<int>(state)));
}

template <typename AnimationEnum>
bool AnimationComponent<AnimationEnum>::play(AnimationEnum state) {
        bool overflow = false;
        // play animation
        //
        if (glfwGetTime() - animations[state].lastFrameStartTime >=
            animations[state].frameTime) {
                if (!reverse)
                        animCnt++;
                else
                        animCnt--;
                animations[state].lastFrameStartTime = glfwGetTime();
        }

        // débordement anim sens normal
        if (animCnt >= animations[state].nFrame && !reverse) {
                animCnt = 0;
                overflow = true;
        }

        // débordement anim a l'envers
        if (animCnt < 0 && reverse) {
                animCnt = animations[state].nFrame - 1;
                overflow = true;
        }

        // evite que les anims a 1 frame sois bugué après une anim reverse
        if (animations[state].nFrame == 1) {
                animCnt = 0;
        }

        owner->setTexture(animations[state].textures[animCnt]);
        return overflow;
}

bool IAnimationComponent::isReverse() const { return reverse; }

void IAnimationComponent::setReverse(bool reverse) { this->reverse = reverse; }

Sprite *IAnimationComponent::getOwner() const { return owner; }

void IAnimationComponent::setOwner(Sprite *owner) { this->owner = owner; }

template <typename AnimationEnum>
AnimationComponent<AnimationEnum>::AnimationComponent(
    const AnimationComponent &other)
    : IAnimationComponent(other), animations(other.animations) {}

template <typename AnimationEnum>
AnimationComponent<AnimationEnum>::AnimationComponent(
    AnimationComponent &&other) noexcept
    : IAnimationComponent(std::move(other)),
      animations(std::move(other.animations)) {}

template <typename AnimationEnum>
AnimationComponent<AnimationEnum> &
AnimationComponent<AnimationEnum>::operator=(const AnimationComponent &other) {
        if (this == &other)
                return *this;
        IAnimationComponent::operator=(other);
        animations = other.animations;
        return *this;
}

template <typename AnimationEnum>
AnimationComponent<AnimationEnum> &AnimationComponent<AnimationEnum>::operator=(
    AnimationComponent &&other) noexcept {
        if (this == &other)
                return *this;
        IAnimationComponent::operator=(std::move(other));
        animations = std::move(other.animations);
        return *this;
}

template class AnimationComponent<MemberState>; // force la génération pour
                                                // MemberState
template class AnimationComponent<GunState>;    // force la génération pour
                                                // GunState
template class AnimationComponent<BulletState>; // force la génération pour
                                                // BulletState
