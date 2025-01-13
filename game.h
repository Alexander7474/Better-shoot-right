#pragma once

#include "Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "Bbop-Library/include/BBOP/Graphics/mapClass.h"
#include "player.h"

#include <GLFW/glfw3.h>
#include <vector>

// pointeur vers la fentre de jeu utilisable de partout
extern GLFWwindow * gameWindow;

// Variable global du moteur de jeu
extern float DELTA_TIME;
extern float GRAVITY;

#ifdef DEBUG
extern std::vector<std::string> LOGS;
#endif

class Game 
{
private:
  Scene scene;
  Camera mainPlayerCam;
  Player mainPlayer;
  Map map;
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
