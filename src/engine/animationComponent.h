//
// Created by artyom on 7/4/25.
//

#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <unordered_map>
#include <vector>

#include "../../Bbop-2D/include/BBOP/Graphics.h"

// structure de stockage d'un animation, respecte la structure des fichier json
struct Animation {
        std::vector<Texture>
            textures;      //<! ensemble de texture qui form l'animation
        double duration;   // temps de l'animation
        int nFrame;        // nombre de frame
        double startTime;  // depart de l'anim
        double lastFrameStartTime;  // depart de la dernière frame
        double frameTime;
};

class IAnimationComponent {
public:
        virtual ~IAnimationComponent() = default;
};

template <typename AnimationEnum>
class AnimationComponent final : public IAnimationComponent {
       private:
        Sprite *owner;  //<! class fille
        std::unordered_map<AnimationEnum, Animation>
            animations;  //<! map de toute les animations possible
        bool reverse{};  //<! joue l'animation à l'envers
        int animCnt{};   //<! compteur de frame de l'animation
       public:
        /**
         * @brief
         * @param owner pointeur vers la class fille héritante du composant
         */
        explicit AnimationComponent(Sprite *owner);

        /**
         * @brief Remplis la map d'animation pour chaque state de la class
         * héritante
         * @param state enumération des état de la class héritante
         * @param path chamin d'accès vers les anims et le fichier
         * animations.json
         */
        void loadTextureCache(AnimationEnum state, std::string path);

        /**
         * @brief joue l'animation
         * @param state etat actuelle de la class fille
         * @return renvoie true si l'animation déborde
         */
        bool play(AnimationEnum state);

        [[nodiscard]] bool isReverse() const;

        void setReverse(bool reverse);

        /**
         * @brief Renvoi un pointeur vers owner
         * @return
         */
        Sprite * getOwner() const;

        /**
         * @brief Changer le possèsseur du composant
         * @param owner
         * @details Utile dans les constructeur par copie pour transférer le composant
         */
        void setOwner(Sprite *owner);

        AnimationComponent(const AnimationComponent &other);

        AnimationComponent(AnimationComponent &&other) noexcept;

        AnimationComponent<AnimationEnum> &operator=(const AnimationComponent &other);

        AnimationComponent<AnimationEnum> &operator=(AnimationComponent &&other) noexcept;
};

#endif  // ANIMATIONCOMPONENT_H
