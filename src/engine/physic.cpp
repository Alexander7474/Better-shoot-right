#include "physic.h"
#include <box2d/b2_body.h>
#include <string>

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

b2Body* addDynamicBox(b2World* world, const Geometric* box, const float restitution, const float density, const float friction, const float linearDamping, const bool rotationLock)
{
  // 1. Définir le corps statique
  b2BodyDef bodyDef;
  bodyDef.position.Set((box->getPosition().x + (box->getSize().x / 2)) / PIXEL_PER_METER, (box->getPosition().y + (box->getSize().y / 2)) / PIXEL_PER_METER); 
  bodyDef.type = b2_dynamicBody;
  bodyDef.fixedRotation = rotationLock;
  bodyDef.linearDamping = linearDamping; // Très utile !

  b2Body* body = world->CreateBody(&bodyDef);

  // 2. Définir la forme (boîte centrée)
  b2PolygonShape boxShape;
  boxShape.SetAsBox((box->getSize().x / 2) / PIXEL_PER_METER, (box->getSize().y / 2) / PIXEL_PER_METER); 
 
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &boxShape;
  fixtureDef.restitution = restitution;
  fixtureDef.density = density;
  fixtureDef.friction = friction;
  
  std::string log = "Box dynamic ajouter l:" + std::to_string(box->getSize().x) + " h:" + std::to_string(box->getSize().y) + " x:" + std::to_string(box->getPosition().x) + " y:" + std::to_string(box->getPosition().y);
  log += "\nBOX2D COORD x:" + std::to_string(body->GetPosition().x) + " y:" + std::to_string(body->GetPosition().y);
  LOGS.push_back(log);

  // 3. Ajouter la fixture au corps
  body->CreateFixture(&fixtureDef);

  return body;
}

void CustomContactListener::BeginContact(b2Contact* contact)
{
  handleContact(contact, true);
}

void CustomContactListener::EndContact(b2Contact* contact)
{
  handleContact(contact, false);
}

void CustomContactListener::handleContact(b2Contact* contact, const bool begin)
{
  b2Fixture* fixtureA = contact->GetFixtureA();
  b2Fixture* fixtureB = contact->GetFixtureB();
  b2Body* bodyA = fixtureA->GetBody();
  b2Body* bodyB = fixtureB->GetBody();

  b2WorldManifold manifold;
  contact->GetWorldManifold(&manifold);
  const b2Vec2 normal = manifold.normal;

  if (bodyB->GetUserData().pointer) {
    if (normal.y < -0.5f) {
      reinterpret_cast<BodyData *>(bodyB->GetUserData().pointer)->isTouchingDown = begin;
    }else {
      reinterpret_cast<BodyData *>(bodyB->GetUserData().pointer)->isTouchingDown = false;
    }
  }
  if (bodyA->GetUserData().pointer) {
    if (normal.y > 0.5f) {
      reinterpret_cast<BodyData *>(bodyA->GetUserData().pointer)->isTouchingDown = begin;
    }else {
      reinterpret_cast<BodyData *>(bodyA->GetUserData().pointer)->isTouchingDown = false;
    }
  }
}

