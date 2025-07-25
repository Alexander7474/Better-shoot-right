#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/crossair.h"
#include "../engine/gameCharacter.h"

class Game;

class Player final : public BbopDrawable {
      private:
        GameCharacter character;

        Crossair crossair;

        Game *game = nullptr; //<! Le player garde un pointer vers sa game

	bool mouseHold{};
      public:
        Player() = default;

        explicit Player(Game *game);

        /**
         * @brief Met a jour le player
         */
        void update(Camera *playerCam, Map *map);

        virtual void Draw(GLint *renderUniforms) const override;

        // GETTER

        GameCharacter &getCharacter();
        Crossair &getCrossair();
};
