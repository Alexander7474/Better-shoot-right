//
// Created by artyom on 7/4/25.
//

#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <unordered_map>
#include <vector>

#include "../../Bbop-2D/include/BBOP/Graphics.h"

// TODO-- Faire une class commune pour AudioComponent et AnimationComponent

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

/**
 * @brief Class mère de AnimationComponent
 * @details permet de prévoir l'implémentation du composant animations
 * sans connaitre l'enumération qui sera utilisé comme template
 */
class IAnimationComponent {
protected:
        Sprite *owner = nullptr;  //<! class fille
        bool reverse{};  //<! joue l'animation à l'envers
        int animCnt{};   //<! compteur de frame de l'animation
public:
        IAnimationComponent() = default;
        virtual ~IAnimationComponent() = default;

        [[nodiscard]] bool isReverse() const;

        void setReverse(bool reverse);

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

        IAnimationComponent(const IAnimationComponent &other) = default;

        IAnimationComponent(IAnimationComponent &&other) = default;

        IAnimationComponent &operator=(const IAnimationComponent &other) = default;

        IAnimationComponent &operator=(IAnimationComponent &&other) = default;
};

/**
 * @brief Gère les animations qu'un sprite peut jouer
 * @tparam AnimationEnum
 */
template <typename AnimationEnum>
class AnimationComponent final : public IAnimationComponent {
       private:
        std::unordered_map<AnimationEnum, Animation>
            animations;  //<! map de toute les animations possible
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
        void loadTexture(AnimationEnum state, std::string path);

        /**
         * @brief joue l'animation
         * @param state etat actuelle de la class fille
         * @return renvoie true si l'animation déborde
         */
        bool play(AnimationEnum state);

        AnimationComponent(const AnimationComponent &other);

        AnimationComponent(AnimationComponent &&other) noexcept;

        AnimationComponent<AnimationEnum> &operator=(const AnimationComponent &other);

        AnimationComponent<AnimationEnum> &operator=(AnimationComponent &&other) noexcept;
};

#endif  // ANIMATIONCOMPONENT_H
