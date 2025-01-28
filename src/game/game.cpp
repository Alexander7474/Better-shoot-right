#include "game.h"
#include "../../Bbop-Library/include/BBOP/Graphics/bbopFunc.h"
#include <irrKlang/ik_ISoundEngine.h>

using namespace std;

GLFWwindow* gameWindow = nullptr;
irrklang::ISoundEngine *soundEngine = nullptr;

float DELTA_TIME = 0.f;
float FPS = 0.f; 
float GRAVITY = 981.f;
default_random_engine RANDOM_ENGINE; 

Game::Game()
{
  if(map.getSpawnPoints().size() > 1){
    mainPlayer.getCharacter().setPosition(map.getSpawnPoints()[0]);
    npc.setPosition(map.getSpawnPoints()[0]);
  }
}

void Game::update()
{
  map.update();
  cerr<<"1"<<endl;
  //déterminer la position du milieu entre le joueur et son crossair
  Vector2f middlePos;
  middlePos.x = (mainPlayer.getCharacter().getPosition().x + mainPlayer.getCrossair().getPosition().x)/2.f;
  middlePos.y = (mainPlayer.getCharacter().getPosition().y + mainPlayer.getCrossair().getPosition().y)/2.f;
  cerr<<"2"<<endl;
  //déterminer la scale de la cam en fonction de la distance entre crossair et play 
  float distance = bbopGetDistance(mainPlayer.getCrossair().getPosition(), mainPlayer.getCharacter().getPosition());
  distance = distance/BBOP_WINDOW_RESOLUTION.x;
  cerr<<"3"<<endl;
  mainPlayerCam.setScale(0.6);
  mainPlayerCam.setPosition(middlePos);
  npc.Bupdate(&map , &mainPlayer.getCharacter());
  cerr<<"4"<<endl;
  mainPlayer.update(&mainPlayerCam, &map);
  cerr<<"5"<<endl;
}

void Game::Draw()
{

  map.Draw(scene, mainPlayerCam);
  scene.Draw(mainPlayer);
  scene.Draw(npc);

  
  #ifdef DEBUG 
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLeftArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getRightArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getBody().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getHead().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLegs().getCollisionBox(), scene);
  bbopDebugCollisionBox(npc.getLeftArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(npc.getRightArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(npc.getBody().getCollisionBox(), scene);
  bbopDebugCollisionBox(npc.getHead().getCollisionBox(), scene);
  bbopDebugCollisionBox(npc.getLegs().getCollisionBox(), scene);
  #endif 
}
