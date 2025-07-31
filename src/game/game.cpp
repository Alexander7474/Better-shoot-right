#include "game.h"
#include "../engine/physic.h"
#include "entity.h"
#include "../engine/dynamicSprite.h"

#include <box2d/b2_body.h>
#include <box2d/box2d.h>
#include <string>

using namespace std;

GLFWwindow* gameWindow = nullptr;

double DELTA_TIME = 0;
double FPS = 0;
double FPS_COUNTER = 0;
double LAST_FPS_UPDATE = glfwGetTime();
float GRAVITY = 9.8f;
default_random_engine RANDOM_ENGINE; 

Game::Game()
  :  map("assets/map/map1/"), physicalWorld(b2Vec2(0.0f,GRAVITY)) // création du monde physique avec un vecteur de gravité
{
  auto* listener = new CustomContactListener();
  physicalWorld.SetContactListener(listener);
  npc=new Bot();
  
  if(map.getSpawnPoints().size() > 1){
    mainPlayer.getCharacter().setPosition(map.getSpawnPoints()[0]);
    npc->getCharacthere()->setPosition(Vector2f(500.f, 1281.f));
  }
  //init physic-------------------------------------------------------------------------
  //rajoute le boite de collision au monde physique
  for(CollisionBox& box : map.getCollision()){
    addStaticBox(&physicalWorld, &box);
  }

  entities.push_back(&mainPlayer.getCharacter());
  entities.push_back(npc->getCharacthere());

  // compute entities
  unsigned long long cptEnt = 0;
  for(const auto& e : entities){
    e->computePhysic(&physicalWorld);
    cptEnt++;
  }
  const string log = to_string(cptEnt) + " entitées initialisées dans le monde box2d pas Game";
  LOGS.push_back(log);
  //------------------------------------------------------------------------------------
}

void Game::update()
{
  //simple gestion de animations
  map.update();


  //déterminer la position du milieu entre le joueur et son crossair
  Vector2f middlePos;
  middlePos.x = (mainPlayer.getCharacter().getPosition().x + mainPlayer.getCrossair().getPosition().x)/2.f;
  middlePos.y = (mainPlayer.getCharacter().getPosition().y + mainPlayer.getCrossair().getPosition().y)/2.f;

  float distance = bbopGetDistance(middlePos, mainPlayer.getCharacter().getPosition());

  // limite la distance à la quelle la caméra peut aller
  if (distance > 50.f) {
    const double dx = middlePos.x - mainPlayer.getCharacter().getPosition().x;
    const double dy = middlePos.y - mainPlayer.getCharacter().getPosition().y;
    const double scale = 50 / distance;
    middlePos.x = (mainPlayer.getCharacter().getPosition().x + scale * dx);
    middlePos.y = (mainPlayer.getCharacter().getPosition().y + scale * dy);
  }

  mainPlayerCam.setScale(0.8);
  mainPlayerCam.setPosition(middlePos);
  mainPlayer.update(&mainPlayerCam, &map);
  npc->update(&map,&mainPlayer.getCharacter());

  const int state = glfwGetKey(gameWindow, GLFW_KEY_G);
  if (state == GLFW_PRESS) {
    mainPlayer.getCharacter().toggleRagdollMod(&physicalWorld);
  }


  //Gestion de la physique-------------------------------------------------------------------------
  constexpr float timeStep = 1.0f / 60.f;
  int velocityIterations = 6;
  int positionIterations = 2;

  //mis a jour du monde box2d
  physicalWorld.Step(timeStep, velocityIterations, positionIterations);

  //mise a jour des entitées après la mise a jour du monde box2d
  for(auto &ent : entities){
    ent->updatePhysic();
  }
  //-----------------------------------------------------------------------------------------------
 }

void Game::Draw()
{
  map.Draw(scene, mainPlayerCam);
  scene.Draw(mainPlayer);
  scene.Draw(*npc->getCharacthere());

  for(auto& d: dynamics){
    scene.Draw(*d);
    bbopDebugCollisionBox(d->getCollisionBox(), scene);
  }

  bbopDebugCollisionBox(mainPlayer.getCharacter().getHead().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLegs().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getBody().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getRightArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLeftArm().getCollisionBox(), scene);

  for (CollisionBox& box : map.getCollision()) {
    bbopDebugCollisionBox(box, scene);
  }

  scene.render();
}
