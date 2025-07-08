//
// Created by artyom on 7/8/25.
//

#include "audioComponent.h"

#include <nlohmann/json.hpp>

#include "member.h"

#include "gun.h"

#include "macro.h"

template <typename AudioEnum>
AudioComponent<AudioEnum>::AudioComponent(Sprite *owner): IAudioComponent() {
        this->owner = owner;
}

template <typename AudioEnum>
void AudioComponent<AudioEnum>::loadSound(AudioEnum state, const std::string& path) {
        // Ici on ne charge pas d'audio par défault
        // Pas d'audio trouvé = pas de son !
        std::string jsonPath = path + "sounds.json";
        std::ifstream jsonFile(jsonPath);
        if (!jsonFile.is_open()) {
                ERROR_MESSAGE("Impossible d'ouvrir les sons " +
                               jsonPath + " | state " + std::to_string(static_cast<int>(state)));
                return;
        }

        // loading with json
        nlohmann::json jsonData;
        try {
                jsonFile >> jsonData;
        } catch (const std::exception &e) {
                ERROR_MESSAGE("Découpage json " + jsonPath + " | state " + std::to_string(static_cast<int>(state)));
                return;
        }

        try {
                if (!jsonData.contains(std::to_string(static_cast<int>(state)))) {
                        ERROR_MESSAGE("Chargement " + jsonPath + " | state " + std::to_string(static_cast<int>(state)) + " non trouvé");
                        return;
                }

                std::string soundFile = jsonData.at(std::to_string(static_cast<int>(state)));

                //chargement du son
                std::string soundPath = path + soundFile;
                Mix_Chunk* sound = Mix_LoadWAV(soundPath.c_str());

                if (!sound) {
                        ERROR_MESSAGE("Chargement " + jsonPath + " | fichier son state "
                                + std::to_string(static_cast<int>(state)) + "/" + soundPath
                                + " non trouvé");
                        return;
                }

                // Ajouter le mix chunk dans la unordered_map
                sounds[state] = sound;
        } catch (const nlohmann::json::exception &e) {
                ERROR_MESSAGE("Recupération audio " +
                               jsonPath + " | " + e.what());
                return;
        }

        DEBUG_MESSAGE("Chargement terminé: " + jsonPath + " | state " + std::to_string(static_cast<int>(state)));
}

template <typename AudioEnum>
void AudioComponent<AudioEnum>::play(AudioEnum state) {
        if (sounds[state] != nullptr) {
                Mix_PlayChannel(0, sounds[state], 0);
        }else {
                ERROR_MESSAGE("Pas de sons pour l'état " + std::to_string(static_cast<int>(state)));
        }
}

Sprite * IAudioComponent::getOwner() const {
        return owner;
}

void IAudioComponent::setOwner(Sprite *owner) {
        this->owner = owner;
}

template <typename AudioEnum>
AudioComponent<AudioEnum>::AudioComponent(const AudioComponent &other)
        : IAudioComponent(other),
          sounds(other.sounds){
}

template <typename AudioEnum>
AudioComponent<AudioEnum>::AudioComponent(AudioComponent &&other) noexcept
        : IAudioComponent(std::move(other)),
          sounds(std::move(other.sounds)){
}

template <typename AudioEnum>
AudioComponent<AudioEnum> & AudioComponent<AudioEnum>::operator=(const AudioComponent &other) {
        if (this == &other)
                return *this;
        IAudioComponent::operator =(other);
        sounds = other.sounds;
        return *this;
}

template <typename AudioEnum>
AudioComponent<AudioEnum> & AudioComponent<AudioEnum>::operator=(AudioComponent &&other) noexcept {
        if (this == &other)
                return *this;
        IAudioComponent::operator =(std::move(other));
        sounds = std::move(other.sounds);
        return *this;
}

template class AudioComponent<GunState>; // force la génération pour GunState