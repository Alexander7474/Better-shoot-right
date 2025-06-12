#pragma once

#include "../../Bbop-Library/include/BBOP/Graphics.h"
#include "player.h"
#include"bot.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <random>
#include <box2d/box2d.h>

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

  //gestion physique 
  RectangleShape testRect;
  RectangleShape testRect2;
  b2Body* body;
  b2Body* bodyStatic;
  b2Body* groundBody;
  b2World world;
  

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
