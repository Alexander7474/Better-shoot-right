#pragma once
#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"
#include "bot.h"

class Ennemi : public Bot
{
public:
    // Virtual destructor
    virtual ~Ennemi() = default;
    
    // Virtual functions with default implementations
    virtual void Bupdate(Map *map, GameCharacter *perso1, vector<GameCharacter*> otherbots) {}
    virtual void detect_player(GameCharacter *perso1) {}
    virtual void patrol_mod() {}
    virtual void seek_mod(GameCharacter *user) {}
    virtual void engage_mod(GameCharacter *user) {}
};