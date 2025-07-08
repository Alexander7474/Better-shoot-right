//
// Created by artyom on 7/8/25.
//

#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../../Bbop-2D/include/BBOP/Graphics.h"

/**
 * @brief Class mère de AudioComponent
 * @details permet de prévoir l'implémentation du composant audio
 * sans connaitre l'enumération qui sera utilisé comme template
 */
class IAudioComponent {
protected:
        Sprite *owner = nullptr;  //<! class fille
public:
        IAudioComponent() = default;
        virtual ~IAudioComponent() = default;

        /**
         * @brief Renvoi un pointeur vers owner
         * @return
         */
        [[nodiscard]] Sprite * getOwner() const;

        /**
         * @brief Changer le possèsseur du composant
         * @param owner
         * @details Utile dans les constructeur par copie pour transférer le composant
         */
        void setOwner(Sprite *owner);

        IAudioComponent(const IAudioComponent &other) = default;

        IAudioComponent(IAudioComponent &&other) = default;

        IAudioComponent &operator=(const IAudioComponent &other) = default;

        IAudioComponent &operator=(IAudioComponent &&other) = default;
};

/**
 * @brief Gère les sons qu'un sprite peut jouer
 * @tparam AudioEnum
 */
template <typename AudioEnum>
class AudioComponent final : public IAudioComponent {
       private:
        std::unordered_map<AudioEnum, Mix_Chunk*>
            audios;  //<! map de toute les animations possible
       public:
        /**
         * @brief
         * @param owner pointeur vers la class fille héritante du composant
         */
        explicit AudioComponent(Sprite *owner);

        /**
         * @brief Remplis la map d'animation pour chaque state de la class
         * héritante
         * @param state enumération des état de la class héritante
         * @param path chemin d'accès vers les sons et le fichier
         * audio.json
         */
        void loadAudio(AudioEnum state, std::string path);

        /**
         * @brief joue l'animation
         * @param state etat actuelle de la class fille
         * @return renvoie true si l'animation déborde
         */
        bool play(AudioEnum state);

        AudioComponent(const AudioComponent &other);

        AudioComponent(AudioComponent &&other) noexcept;

        AudioComponent<AudioEnum> &operator=(const AudioComponent &other);

        AudioComponent<AudioEnum> &operator=(AudioComponent &&other) noexcept;
};



#endif //AUDIOCOMPONENT_H
