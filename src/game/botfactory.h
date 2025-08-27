#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"
#include "bot.h"


class BotFactory {
private:
    std::unordered_map<std::string, std::unique_ptr<Bot>> BotConstructor;
public:
    void LoadBots();
    Bot* getBot(std::string name);
};


