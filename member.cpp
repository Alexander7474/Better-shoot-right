#include "member.h"
#include "game.h"
#include "Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "Bbop-Library/include/BBOP/Graphics/textureClass.h"
#include "gameCharacter.h"

#include <exception>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

Member::Member()
  : attachPoint(Vector2f(0.f,0.f)),
    force(1.f)
{}

void Member::update()
{
  setPosition(attachPoint);

  //play animation 
  //
}

void Member::setAttachPoint(Vector2f _attachPoint)
{
  attachPoint = _attachPoint;
}

void Member::setAttachPoint(float x, float y)
{
  attachPoint.x = x;
  attachPoint.y = y;
}

Vector2f Member::getAttachPoint() const
{
  return attachPoint;
}

void Member::createTextureCache(std::string path)
{
  for(int i = 0; i <= run; i++){
    string statePath = path + gameCharacterStateString[i] + ".png";

    std::ifstream file(statePath);

    if(file.good()){
      animations[i].textures = bbopLoadSpriteSheet(statePath.c_str(), 1, 1);
      file.close();
    }else{
      // si la spritesheet de l'etat n'éxiste pas on charge celle de idle
      statePath = path + "idle.png";
      #ifdef DEBUG 
        LOGS.push_back("Erreur chargement " + gameCharacterStateString[i] + " sprite sheet pour " + name);
      #endif
      animations[idle].textures = bbopLoadSpriteSheet(statePath.c_str(), 1, 1);
    }
  }

  setTexture(animations[idle].textures[0]);

  string filename = "assets/img/soldier/legs/legs.json";
  std::ifstream jsonFile(filename);
  if(!jsonFile.is_open()){
    #ifdef DEBUG 
    LOGS.push_back("Erreur loading json file for " + name);
    #endif 
    return;
  }

  //loading with json
  json jsonData;
  try {
    jsonFile >> jsonData;
  } catch (const std::exception &e) {
    #ifdef DEBUG 
    LOGS.push_back("Erreur translating json for " + name);
    #endif 
    return;
  }

  try{
      string statePath = jsonData.at("run").at("sprite_sheet");
      statePath = path + statePath;
      int columns = jsonData.at("run").at("columns");
      int rows = jsonData.at("run").at("rows");

      animations[run].textures.clear();
      animations[run].textures = bbopLoadSpriteSheet(statePath.c_str(), rows, columns);

  } catch (const json::exception &e){
    #ifdef DEBUG 
    LOGS.push_back("Erreur getting JSON state animation for " + name);
    #endif 
    return;
  }
}
