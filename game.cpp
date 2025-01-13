#include "game.h"
#include "Bbop-Library/include/BBOP/Graphics/bbopFunc.h"

using namespace std;

GLFWwindow* gameWindow = nullptr;
float DELTA_TIME = 0.f;
float GRAVITY = 981.f;

#ifdef DEBUG
vector<string> LOGS;
#endif

Game::Game()
{
  
}

void Game::update()
{

  mainPlayer.update(&mainPlayerCam, &map);

}

void Game::Draw()
{

  map.Draw(scene, mainPlayerCam);
  scene.useCamera(&mainPlayerCam);
  scene.Draw(mainPlayer);

  
  #ifdef DEBUG 
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLeftArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getRightArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getBody().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getHead().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLegs().getCollisionBox(), scene);
  #endif 
}
