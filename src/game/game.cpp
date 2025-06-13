#include "game.h"
#include "../engine/box2d-bbop-link.h"
#include "entity.h"

#include <box2d/b2_body.h>
#include <box2d/box2d.h>

using namespace std;

GLFWwindow* gameWindow = nullptr;

float DELTA_TIME = 0.f;
float FPS = 0.f; 
float GRAVITY = 9.8f;
default_random_engine RANDOM_ENGINE; 

Game::Game()
  :  physicalWorld(b2Vec2(0.0f,GRAVITY)) // création du monde physique avec un vecteur de gravité
{
  if(map.getSpawnPoints().size() > 1){
    mainPlayer.getCharacter().setPosition(map.getSpawnPoints()[0]);
    npc.setPosition(map.getSpawnPoints()[1]);
  }

  //-------------------------------------------------------------------------
  //rajoute le boite de collision au monde physique 
  for(CollisionBox& box : map.getCollision()){
    addStaticBox(&physicalWorld, &box);
  }
}

void Game::update()
{
  //simple gestion de animations
  map.update();
    
  mainPlayer.getCharacter().setPos(map.getSpawnPoints()[0]);

  //déterminer la position du milieu entre le joueur et son crossair
  Vector2f middlePos;
  middlePos.x = (mainPlayer.getCharacter().getPosition().x + mainPlayer.getCrossair().getPosition().x)/2.f;
  middlePos.y = (mainPlayer.getCharacter().getPosition().y + mainPlayer.getCrossair().getPosition().y)/2.f;

  //déterminer la scale de la cam en fonction de la distance entre crossair et play 
  float distance = bbopGetDistance(mainPlayer.getCrossair().getPosition(), mainPlayer.getCharacter().getPosition());
  distance = distance/BBOP_WINDOW_RESOLUTION.x;
  mainPlayerCam.setScale(1.5);
  mainPlayerCam.setPosition(middlePos);
  mainPlayer.update(&mainPlayerCam, &map);

  //Gestion de la physique-------------------------------------------------------------------------
  float timeStep = 1.0f / 100.f;
  int velocityIterations = 6;
  int positionIterations = 2;

  //mis a jour du monde box2d 
  physicalWorld.Step(timeStep, velocityIterations, positionIterations);

  //mise a jour des entitées après la mise a jour du monde box2d 
  for(auto &ent : entities){
    ent.updatePhysique();
  }

  //-----------------------------------------------------------------------------------------------
 }

void Game::Draw()
{
  map.Draw(scene, mainPlayerCam);
  scene.Draw(mainPlayer);
  scene.render();

  
  #ifdef DEBUG 
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLeftArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getRightArm().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getBody().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getHead().getCollisionBox(), scene);
  bbopDebugCollisionBox(mainPlayer.getCharacter().getLegs().getCollisionBox(), scene);
  #endif 
}
