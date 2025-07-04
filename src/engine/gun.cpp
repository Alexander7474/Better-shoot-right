#include "gun.h"
#include "../game/game.h"
#include "bullet.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <random>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

Gun::Gun()
  :  Sprite(Texture("assets/guns/scar/scar.png"))
{
  // arme par default
  name = "scar";
  armed = true;
  rearmTime = 0.1;
  magazineSize = 20;
  ammo = 20;
  lastShotTime = glfwGetTime()-rearmTime;
  damage = 0.5f;
  gunMouth = Vector2f(20.f,5.f);
  bulletSpeed = 1.f;
  gunDirection = rightDir;

  //texture par  default des balles 
  bulletTexture = new Texture("assets/guns/bullets/default.png");

  state = gun_idle_state;

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

  // Après avoir toute les texture par default on
  // charge les textures personnalisé avec leur
  // fichier json.
  // Si pas de fichier json, il reste les texture
  // par défault.
  string jsonPath = path + "gun.json";
  std::ifstream jsonFile(jsonPath);
  if(!jsonFile.is_open()){
    LOGS.push_back("Erreur loading json file for " + name);
    return; 
  }

  //loading with json
  json jsonData;
  try {
    jsonFile >> jsonData;
  } catch (const std::exception &e) {
    LOGS.push_back("Erreur parsing json for " + name);
    return;
  }

  try{
    //chargement des infos de l'arme
    name = jsonData.at("name");
    damage = jsonData.at("damage");
    magazineSize = jsonData.at("magazine_size");
    ammo = magazineSize;
    rearmTime = jsonData.at("ream_time");
    bulletSpeed = jsonData.at("bullet_speed");

    string sound = jsonData.at("shot_sound");
    //gunShotSound = soundEngine->addSoundSourceFromFile(sound.c_str());

    string bullet = jsonData.at("bullet_texture");
    delete bulletTexture;
    bulletTexture = new Texture(bullet.c_str());

    float x = jsonData.at("mouth_x");
    float y = jsonData.at("mouth_y");
    gunMouth.x = x;
    gunMouth.y = y;

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
        LOGS.push_back("Erreur loading " + to_string(i) + " for " + name + ", loading default.png");
      }

    }

    setTexture(animations[gun_idle_state].textures[0]);
  } catch (const json::exception &e){
    LOGS.push_back("Erreur getting JSON info for " + name);
    return;
  }

}




void Gun::update()
{
   //play animation 
  //
  if(glfwGetTime() - animations[state].lastFrameStartTime >= animations[state].frameTime){
    animCnt++;
    animations[state].lastFrameStartTime = glfwGetTime();
  }

  //débordement anim sens normal
  if(animCnt >= animations[state].nFrame){
    animCnt = 0;
    state = gun_idle_state;
  }

  setTexture(animations[state].textures[animCnt]);

  if(!armed){
    if(glfwGetTime() - lastShotTime >= rearmTime && ammo > 0){
      armed = true;
    }
  }

  for(Bullet &b : bulletVector){
    b.update();
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
  if(armed){
    lastShotTime = glfwGetTime();
    ammo--;
    armed = false;
    state = shoot_state;

    //creation de la balle 
    Vector2f mouth(getPosition().x + gunMouth.x, getPosition().y + gunMouth.y); // position bouche du canon

    // ajout d'un peu d'aléatoire dans la direction
    uniform_real_distribution<float> distribution(-0.1f,0.1f);
    const float r = distribution(RANDOM_ENGINE);
    const float rotation = getRotation() + r; //rotation de l'arme


    Vector2f inertie(cos(rotation) * bulletSpeed,sin(rotation) * bulletSpeed); //vecteur d'inertie en fonction de la rotaion du canon
    
    if(gunDirection == leftDir){
      inertie = Vector2f(cos(rotation) * -bulletSpeed,sin(rotation) * -bulletSpeed);
      mouth = Vector2f(getPosition().x - gunMouth.x, mouth.y);
    }

    //calcule position de sortie de la balle 
    const float outX = getPosition().x + (mouth.x - getPosition().x) * cos(getRotation()) - (mouth.y - getPosition().y) * sin(getRotation());
    const float outY = getPosition().y + (mouth.x - getPosition().x) * sin(getRotation()) + (mouth.y - getPosition().y) * cos(getRotation());
    
    Bullet b(bulletTexture, inertie);
    b.setPosition(outX, outY);
    b.setRotation(getRotation());

    if(gunDirection == leftDir)
      b.flipVertically();

    bulletVector.push_back(b);
  }
}

void Gun::reload()
{
  ammo = magazineSize;
}

const std::string & Gun::getName() const {
  return name;
}

void Gun::setName(const std::string &name) {
  this->name = name;
}

GunState Gun::getState() const {
  return state;
}

void Gun::setState(const GunState state) {
  this->state = state;
}

Direction Gun::getGunDirection() const {
  return gunDirection;
}

void Gun::setGunDirection(const Direction gunDirection) {
  this->gunDirection = gunDirection;
}

float Gun::getDamage() const {
  return damage;
}

void Gun::setDamage(const float damage) {
  this->damage = damage;
}

bool Gun::isArmed() const {
  return armed;
}

void Gun::setArmed(const bool armed) {
  this->armed = armed;
}

int Gun::getMagazineSize() const {
  return magazineSize;
}

void Gun::setMagazineSize(const int magazineSize) {
  this->magazineSize = magazineSize;
}

int Gun::getAmmo() const {
  return ammo;
}

void Gun::setAmmo(const int ammo) {
  this->ammo = ammo;
}

const std::vector<Bullet> & Gun::getBulletVector() const {
  return bulletVector;
}

void Gun::setBulletVector(const std::vector<Bullet> &bulletVector) {
  this->bulletVector = bulletVector;
}

