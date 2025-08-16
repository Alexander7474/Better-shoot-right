#include "botfactory.h"
#include <nlohmann/json.hpp>

#include <GLFW/glfw3.h>

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
        Bots[name]=Bot();
        Bots[name].setBot(threat,damage,healthpoint,range);
    }
}

Bot* BotFactory::getBot(std::string name){
    return &Bots[name];
}
