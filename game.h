#pragma once

#include "Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "player.h"

#include <GLFW/glfw3.h>

// pointeur vers la fentre de jeu utilisable de partout
extern GLFWwindow * gameWindow;
extern float DELTA_TIME;

class Game 
{
private:
  Scene scene;
  Camera mainPlayerCam;
  Player mainPlayer;
public:
  Game();

  /**
  * @brief Met a jour la game
  */
  void update();

  /**
  * @brief Dessine la game avec la scene 
  */
  void Draw();
};
