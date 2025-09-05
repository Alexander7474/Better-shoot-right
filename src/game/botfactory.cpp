#include "botfactory.h"
#include <nlohmann/json.hpp>

#include <GLFW/glfw3.h>

std::unordered_map<std::string, std::unique_ptr<Bot>> BotFactory::BotConstructor;

void BotFactory::LoadBots(){
    std::string jsonPath = "assets/personnages/bot/bot.json";
    std::ifstream jsonFile(jsonPath);
    nlohmann::json jsonData;
    jsonFile>>jsonData;
    for (const auto& [name, config] : jsonData.items()) {
        float range=config["range"];
        float damage=config["damage"];
        int threat=config["threat"];
        float healthpoint=config["healthpoint"];
        BotConstructor[name]=std::make_unique<Bot>();
        BotConstructor[name].get()->setBot(threat,damage,healthpoint,range);
    }
}

// TODO -- Passer par un constructeur par copie dans botFactory pour récupèrer les bots quand gameCharacter aura un constructeur par copie
Bot* BotFactory::getBot(std::string name){
    Bot *bt=new Bot();
    bt->setBot(BotConstructor[name].get()->getThreat(),BotConstructor[name].get()->getDamage(),BotConstructor[name].get()->getCharacter()->getHp(),BotConstructor[name].get()->getRange());
    return bt;
}
