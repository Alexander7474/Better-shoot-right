//
// Created by artyom on 7/4/25.
//

#include "animationComponent.h"

#include <nlohmann/json.hpp>

#include "member.h"

#include "gun.h"

template <typename AnimationEnum>
AnimationComponent<AnimationEnum>::AnimationComponent(Sprite *owner)
    : owner(owner) {}

template <typename AnimationEnum>
void AnimationComponent<AnimationEnum>::loadTextureCache(AnimationEnum state,
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
                LOGS.push_back("Impossible d'ouvrir les animations " +
                               jsonPath);
                return;
        }

        // loading with json
        nlohmann::json jsonData;
        try {
                jsonFile >> jsonData;
        } catch (const std::exception &e) {
                LOGS.push_back("Erreur parsing json pour " + jsonPath);
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
                        LOGS.push_back("Erreur loading " +
                                       std::to_string(static_cast<int>(state)) +
                                       " pour " + jsonPath);
                }
        } catch (const nlohmann::json::exception &e) {
                LOGS.push_back("Erreur getting JSON state animation for " +
                               jsonPath + " | " + e.what());
                return;
        }
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

template <typename AnimationEnum>
bool AnimationComponent<AnimationEnum>::isReverse() const {
        return reverse;
}

template <typename AnimationEnum>
void AnimationComponent<AnimationEnum>::setReverse(bool reverse) {
        this->reverse = reverse;
}

template class AnimationComponent<MemberState>;  // force la génération pour MemberState
template class AnimationComponent<GunState>;  // force la génération pour GunState