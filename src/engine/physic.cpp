#include "physic.h"
#include <box2d/b2_body.h>
#include <string>

/**
* @brief Créé une boite de collision dans un monde box2d à partir d'une collision box BBOP 
*
* @param world Pointeur vers le monde 
* @param box pointeur vers la boite à rajouter 
*
* @return body Corps box2d de la boite 
*/
b2Body* addStaticBox(b2World* world, const Geometric* box)
{
  // 1. Définir le corps statique
  b2BodyDef bodyDef;
  bodyDef.position.Set((box->getPosition().x + (box->getSize().x / 2)) / PIXEL_PER_METER, (box->getPosition().y + (box->getSize().y / 2)) / PIXEL_PER_METER); 
  bodyDef.type = b2_staticBody;

  b2Body* body = world->CreateBody(&bodyDef);

  // 2. Définir la forme (boîte centrée)
  b2PolygonShape boxShape;
  boxShape.SetAsBox((box->getSize().x / 2) / PIXEL_PER_METER, (box->getSize().y / 2) / PIXEL_PER_METER);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &boxShape;
  fixtureDef.density = 0.f;
  fixtureDef.friction = 1.f;

  std::string log = "Box static ajouter l:" + std::to_string(box->getSize().x) + " h:" + std::to_string(box->getSize().y) + " x:" + std::to_string(box->getPosition().x) + " y:" + std::to_string(box->getPosition().y);
  log += "\nBOX2D COORD x:" + std::to_string(body->GetPosition().x) + " y:" + std::to_string(body->GetPosition().y);
  LOGS.push_back(log);

  // 3. Ajouter la fixture au corps
  body->CreateFixture(&fixtureDef); // densité = 0 pour statique

  return body;
}

/**
* @brief Créé une boite de collision Dynamic dans un monde box2d à partir d'une collision box BBOP 
*
* @param world Pointeur vers le monde 
* @param box pointeur vers la boite à rajouter
* @param density
* @param friction
* @param rotationLock Si le corp à sa rotation blocké
*
* @return body Corps box2d de la boite 
*/
b2Body* addDynamicBox(b2World* world, const Geometric* box, const float density, const float friction, const bool rotationLock)
{
  // 1. Définir le corps statique
  b2BodyDef bodyDef;
  bodyDef.position.Set((box->getPosition().x + (box->getSize().x / 2)) / PIXEL_PER_METER, (box->getPosition().y + (box->getSize().y / 2)) / PIXEL_PER_METER); 
  bodyDef.type = b2_dynamicBody;
  bodyDef.fixedRotation = rotationLock;
  bodyDef.linearDamping = 3.0f; // Très utile !

  b2Body* body = world->CreateBody(&bodyDef);

  // 2. Définir la forme (boîte centrée)
  b2PolygonShape boxShape;
  boxShape.SetAsBox((box->getSize().x / 2) / PIXEL_PER_METER, (box->getSize().y / 2) / PIXEL_PER_METER); 
 
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &boxShape;
  fixtureDef.restitution = 0.0f;
  fixtureDef.density = density;
  fixtureDef.friction = friction;
  
  std::string log = "Box dynamic ajouter l:" + std::to_string(box->getSize().x) + " h:" + std::to_string(box->getSize().y) + " x:" + std::to_string(box->getPosition().x) + " y:" + std::to_string(box->getPosition().y);
  log += "\nBOX2D COORD x:" + std::to_string(body->GetPosition().x) + " y:" + std::to_string(body->GetPosition().y);
  LOGS.push_back(log);

  // 3. Ajouter la fixture au corps
  body->CreateFixture(&fixtureDef);

  return body;
}

