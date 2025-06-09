#include "game.h"
#include <box2d/box2d.h>

using namespace std;

GLFWwindow* gameWindow = nullptr;


float DELTA_TIME = 0.f;
float FPS = 0.f; 
float GRAVITY = 981.f;
default_random_engine RANDOM_ENGINE; 

Game::Game()
  :  world(b2Vec2(0.0f,9.8f))
{
  if(map.getSpawnPoints().size() > 1){
    mainPlayer.getCharacter().setPosition(map.getSpawnPoints()[0]);
    npc.setPosition(map.getSpawnPoints()[1]);
  }

  // Définition du corps
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(mainPlayer.getCharacter().getPosition().x, mainPlayer.getCharacter().getPosition().y-100.f); // position en mètres
  body = world.CreateBody(&bodyDef);

  // Définir la forme du carré (1m x 1m)
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(0.5f, 0.5f); // largeur/2, hauteur/2

  // Attacher la forme au corps avec des propriétés physiques
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  body->CreateFixture(&fixtureDef);

  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(mainPlayer.getCharacter().getPosition().x, mainPlayer.getCharacter().getPosition().y+100.f);
  groundBody = world.CreateBody(&groundBodyDef);

  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 0.5f); // un sol très large

  groundBody->CreateFixture(&groundBox, 0.0f); // densité = 0 car statique
}

void Game::update()
{
  map.update();
    
  mainPlayer.getCharacter().setPos(map.getSpawnPoints()[0]);

  //déterminer la position du milieu entre le joueur et son crossair
  Vector2f middlePos;
  middlePos.x = (mainPlayer.getCharacter().getPosition().x + mainPlayer.getCrossair().getPosition().x)/2.f;
  middlePos.y = (mainPlayer.getCharacter().getPosition().y + mainPlayer.getCrossair().getPosition().y)/2.f;

  //déterminer la scale de la cam en fonction de la distance entre crossair et play 
  float distance = bbopGetDistance(mainPlayer.getCrossair().getPosition(), mainPlayer.getCharacter().getPosition());
  distance = distance/BBOP_WINDOW_RESOLUTION.x;
  mainPlayerCam.setScale(0.6);
  mainPlayerCam.setPosition(middlePos);
  mainPlayer.update(&mainPlayerCam, &map);

  float timeStep = 1.0f / 60.0f;
  int velocityIterations = 6;
  int positionIterations = 2;

  world.Step(timeStep, velocityIterations, positionIterations);

  testRect.setPosition(body->GetPosition().x,body->GetPosition().y);
  testRect.setSize(50.f,50.f);
}

void Game::Draw()
{
  map.Draw(scene, mainPlayerCam);
  scene.Draw(testRect);
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
