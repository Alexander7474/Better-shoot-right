#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"
#include "bot.h"


class BotFactory {
private:
    static std::unordered_map<std::string, std::unique_ptr<Bot>> BotConstructor;
public:
    static void LoadBots();
    static Bot* getBot(std::string name);
};


