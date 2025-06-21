#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "player.h"
#include"trooper.h"
#include"neutre.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <random>

// pointeur vers la fentre de jeu utilisable de partout
extern GLFWwindow * gameWindow;

// Variable global du moteur de jeu
extern float DELTA_TIME;
extern float GRAVITY;
extern float FPS;
extern std::default_random_engine RANDOM_ENGINE; 

class Game 
{
private:
  Scene scene;
  Camera mainPlayerCam;
  Player mainPlayer;
  Map map;
  Trooper npc;
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
