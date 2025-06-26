#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "player.h"
#include "bot.h"
#include "../engine/dynamicSprite.h"
#include "entity.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <random>
#include <box2d/box2d.h>

// pointeur vers la fentre de jeu utilisable de partout
extern GLFWwindow * gameWindow;

// Variable global du moteur de jeu
extern double DELTA_TIME;
extern double FPS;
extern double FPS_COUNTER;
extern double LAST_FPS_UPDATE;
extern float GRAVITY;
extern std::default_random_engine RANDOM_ENGINE; 

class Game 
{
private:
  Scene scene;
  Camera mainPlayerCam;
  Player mainPlayer;
  Map map;
  Bot npc;
  std::vector<DynamicSprite*> dynamics; // liste des enititées

  //gestion physique 
  b2World physicalWorld; 
  std::vector<Entity*> entities; // liste des enititées
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
