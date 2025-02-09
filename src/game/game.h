#pragma once

#include "../../Bbop-Library/include/BBOP/Graphics.h"
#include "player.h"
#include"bot.h"
#include <GLFW/glfw3.h>
#include <irrKlang/ik_ISoundEngine.h>
#include <vector>
#include <irrKlang/irrKlang.h>
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
  Bot npc;
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
