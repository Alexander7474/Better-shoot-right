#pragma once

#include <memory>
#include <string>

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "animationComponent.h"

enum class MemberState { idle, run, ragdoll, dead };

class Member : public Sprite {
      private:
        // physique
        Vector2f attachPoint;
        float force;

        // info
        std::string name;

        // anim
        MemberState state;
        std::unique_ptr<AnimationComponent<MemberState>> animation;

        friend class GameCharacter;

      public:
        Member();

        void update();

        /**
         * @brief
         * @param _attachPoint point ou le membre se fixe
         */
        void setAttachPoint(const Vector2f &_attachPoint);
        void setAttachPoint(float x, float y);

        Vector2f getAttachPoint() const;

        /**
         * @brief charge les animations dans le composant animation
         * @param path chemin d'accès au animations
         */
        void createTextureCache(const std::string &path);

        // TODO -- remove this shit
        void setetat(int k);
        int getetat();

        MemberState getState();
};
