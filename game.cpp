#include "game.h"

GLFWwindow* gameWindow = nullptr;
float DELTA_TIME = 0.f;

Game::Game()
{
  
}

void Game::update()
{

  mainPlayer.update(&mainPlayerCam);

}

void Game::Draw()
{

  scene.Use();

  scene.useCamera(&mainPlayerCam);
  scene.Draw(mainPlayer);
}
