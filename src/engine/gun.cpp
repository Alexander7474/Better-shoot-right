#include "gun.h"
#include "../game/game.h"
#include <GLFW/glfw3.h>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

Gun::Gun()
  :  Sprite(Texture("assets/guns/scar/scar.png"))
{
  // arme par default
  name = "scar";
  isArmed = true;
  rearmTime = 0.1;
  magazineSize = 20;
  ammo = 20;
  lastShotTime = glfwGetTime()-rearmTime;
  damage = 0.5f;

  //texture par  default des balles 
  bulletTexture = new Texture("assets/guns/bullets/default.png");

  loadJsonFile("assets/guns/scar/");
}

Gun::~Gun()
{
  delete bulletTexture;
}

void Gun::loadJsonFile(string path)
{
  //on charge pour chaque anim la texture par default
  for(int i = 0; i <= reload_state; i++){
    string stateSheetPath = path + "default.png";
        
    animations[i].textures = bbopLoadSpriteSheet(stateSheetPath.c_str(), 1, 1);
    animations[i].duration = 0.0;
    animations[i].nFrame = 1;
    animations[i].frameTime = 0.0;
    animations[i].lastFrameStartTime = 0.0;
    animations[i].startTime = 0.0;
  }
  setTexture(animations[gun_idle_state].textures[0]);

  //après avoir toute les texture par default on charge les textures personnalisé avec leur fichier json
  //si pas de fichier json, il reste les texture par défault
  string jsonPath = path + "gun.json";
  std::ifstream jsonFile(jsonPath);
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
    LOGS.push_back("Erreur parsing json for " + name);
    #endif 
    return;
  }

  try{
    //chargement des infos de l'arme
    name = jsonData.at("name");
    damage = jsonData.at("damage");
    magazineSize = jsonData.at("magazine_size");
    ammo = magazineSize;

    string sound = jsonData.at("shot_sound");
    gunShotSound = soundEngine->addSoundSourceFromFile(sound.c_str());

    string bullet = jsonData.at("bullet_texture");
    delete bulletTexture;
    bulletTexture = new Texture(bullet.c_str());

    rearmTime = jsonData.at("ream_time");

    //chargement de animation de l'arme
    for(int i = 0; i <= reload_state; i++){

      if(jsonData.contains(to_string(i))){

        string stateSheetPath = jsonData.at(to_string(i)).at("sprite_sheet");
        stateSheetPath = path + stateSheetPath;

        int columns = jsonData.at(to_string(i)).at("columns");
        int rows = jsonData.at(to_string(i)).at("rows");
        int nFrame = jsonData.at(to_string(i)).at("n_frame");

        double duration = jsonData.at(to_string(i)).at("duration");
      
        animations[i].textures.clear();
        animations[i].textures = bbopLoadSpriteSheet(stateSheetPath.c_str(), rows, columns);
        animations[i].duration = duration;
        animations[i].nFrame = nFrame;
        animations[i].frameTime = duration/nFrame;
      }else{

        #ifdef DEBUG 
        LOGS.push_back("Erreur loading " + to_string(i) + " for " + name + ", loading default.png");
        #endif
      }

    }

    setTexture(animations[idle].textures[0]);
  } catch (const json::exception &e){
    #ifdef DEBUG 
    LOGS.push_back("Erreur getting JSON state animation for " + name);
    #endif 
    return;
  }

}

void Gun::update()
{
  if(!isArmed){
    if(glfwGetTime() - lastShotTime >= rearmTime && ammo > 0){
      isArmed = true;
    }
  }
}

void Gun::setAttachPoint(Vector2f ap)
{
  attachPoint = ap;
  setPosition(ap);
}

void Gun::setAttachPoint(float x, float y)
{
  setAttachPoint(Vector2f(x,y));
}

void Gun::shoot()
{
  if(isArmed){
    soundEngine->play2D(gunShotSound, false);
    lastShotTime = glfwGetTime();
    ammo--;
    isArmed = false;
  }
}

void Gun::reload()
{
  ammo = magazineSize;
}
