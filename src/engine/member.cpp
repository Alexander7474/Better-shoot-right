#include "member.h"
#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-2D/include/BBOP/Graphics/textureClass.h"

#include <GLFW/glfw3.h>
#include <exception>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

Member::Member()
  : attachPoint(Vector2f(0.f,0.f)),
    force(1.f)
{
  animCnt = 0;
  state = idle;
  isReverse = false;
}

void Member::update()
{

  //play animation 
  //
  if (state!=dead)
  {
    if(glfwGetTime() - animations[state].lastFrameStartTime >= animations[state].frameTime && state){
      if(!isReverse)
        animCnt++;
      else
        animCnt--;
      animations[state].lastFrameStartTime = glfwGetTime();
    }

    //débordement anim sens normal
    if(animCnt >= animations[state].nFrame && !isReverse){
      animCnt = 0;
    }

    //débordement anim a l'envers
    if(animCnt < 0 && isReverse){
      animCnt = animations[state].nFrame -1;
    }

    //evite que les anims a 1 frame sois bugué après une anim reverse
    if(animations[state].nFrame == 1){
      animCnt = 0;
    }

    setTexture(animations[state].textures[animCnt]);
  }
  
  

  #ifdef DEBUG 
  cout << "Character Member: " << name << endl;
  cout << "state: " << state << endl;
  cout << "animCnt: " << animCnt << " /max: " << animations[state].nFrame << endl;
  cout << "lastFrameStartTime: " << animations[state].lastFrameStartTime << endl;
  cout << "----------------------------------------------------------------" << endl;
  #endif
}

void Member::setAttachPoint(Vector2f _attachPoint)
{
  attachPoint = _attachPoint;
  setPosition(attachPoint);
}

void Member::setAttachPoint(float x, float y)
{
  setAttachPoint(Vector2f(x,y));
}

Vector2f Member::getAttachPoint() const
{
  return attachPoint;
}

void Member::createTextureCache(std::string path)
{
  //on charge pour chaque anim la texture par default
  for(int i = 0; i <= run; i++){
    string stateSheetPath = path + "default.png";
        
    animations[i].textures = bbopLoadSpriteSheet(stateSheetPath.c_str(), 1, 1);
    animations[i].duration = 0.0;
    animations[i].nFrame = 1;
    animations[i].frameTime = 0.0;
    animations[i].lastFrameStartTime = 0.0;
    animations[i].startTime = 0.0;
  }
  setTexture(animations[idle].textures[0]);

  //après avoir toute les texture par default on charge les textures personnalisé avec leur fichier json
  //si pas de fichier json, il reste les texture par défault
  string jsonPath = path + "animations.json";
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

    for(int i = 0; i <= run; i++){

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

void Member::setetat(int k){
  if (k==0)
  {
    state = idle;
  }else if (k==1)
  {
    state = run;
  }else if (k==2)
  {
    state = dead;
  }else{
    cerr<<"state do not exist"<<endl;
  }
  
}

int Member::getetat(){
  return state;
}