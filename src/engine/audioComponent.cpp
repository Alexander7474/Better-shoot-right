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
void AudioComponent<AudioEnum>::loadAudio(AudioEnum state, std::string path) {
        ERROR_MESSAGE("Fonction loadAudio non ipmplémenté");
}

template <typename AudioEnum>
bool AudioComponent<AudioEnum>::play(AudioEnum state) {
        ERROR_MESSAGE("Fonction play de Audio non ipmplémenté");
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
          audios(other.audios){
}

template <typename AudioEnum>
AudioComponent<AudioEnum>::AudioComponent(AudioComponent &&other) noexcept
        : IAudioComponent(std::move(other)),
          audios(std::move(other.audios)){
}

template <typename AudioEnum>
AudioComponent<AudioEnum> & AudioComponent<AudioEnum>::operator=(const AudioComponent &other) {
        if (this == &other)
                return *this;
        IAudioComponent::operator =(other);
        audios = other.audios;
        return *this;
}

template <typename AudioEnum>
AudioComponent<AudioEnum> & AudioComponent<AudioEnum>::operator=(AudioComponent &&other) noexcept {
        if (this == &other)
                return *this;
        IAudioComponent::operator =(std::move(other));
        audios = std::move(other.audios);
        return *this;
}