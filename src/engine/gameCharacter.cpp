#include "gameCharacter.h"
#include "../game/game.h"
#include "gun.h"
#include "member.h"

#include <GLFW/glfw3.h>
#include <cmath>

#include "physic.h"

using namespace std;

string gameCharacterStateString[2] = { 
  "idle",
  "run"
};

GameCharacter::GameCharacter()
  : newtonX(40.f),
    newtonY(10.f),
    restitution(0.f),
    friction(2.f),
    density(1.f),
    linearDamping(1.f),
    hp(10.f)
{
  characterDirection = rightDir;
  scale = 0.75f;

  //--------------------------------------------------------------

  leftArm.setSize(32*scale,32*scale);
  //leftArm.getCollisionBox().setOffsetX(3.f,3.f);
  //leftArm.getCollisionBox().setOffsetY(6.f,10.f);
  leftArm.name = "left arm";

  rightArm.setSize(32*scale,32*scale);
  //rightArm.getCollisionBox().setOffsetX(3.f,3.f);
  //rightArm.getCollisionBox().setOffsetY(4.f,15.f);
  rightArm.name = "right arm";

  body.setSize(32*scale,32*scale);
  body.setOrigin(16*scale,16*scale); // origine au centre du torse
  body.name = "body";

  head.setSize(32*scale,32*scale);
  head.setOrigin(16*scale,16*scale); // origine au coup
  head.name = "head";

  legs.setSize(64*scale,32*scale);
  legs.setOrigin(32*scale,0*scale); // origine sur les hanche
   legs.name = "legs";

  gun.setSize(64*scale,32*scale);

  //--------------------------------------------------------------

  setPos(Vector2f(300.f,0.f));
  createTextureCache("assets/personnages/soldier/");
}

void GameCharacter::createTextureCache(const string& path)
{
  const string armLeftPath = path + "arm_left/";
  leftArm.createTextureCache(armLeftPath);

  const string armRightPath = path + "arm_right/";
  rightArm.createTextureCache(armRightPath);

  const string bodyPath = path + "body/";
  body.createTextureCache(bodyPath);

  const string headPath = path + "head/";
  head.createTextureCache(headPath);

  const string legsPath = path + "legs/";
  legs.createTextureCache(legsPath);
}

void GameCharacter::update(Map* map)
{
  //gestion de l'état du personnage
  if (entityBody->GetLinearVelocity().x <= 0.5f && entityBody->GetLinearVelocity().x >= -0.5f) {
    legs.state = idle;
    legs.animations[idle].lastFrameStartTime = glfwGetTime();
  }

  //mise à jour des membres
  leftArm.update();
  rightArm.update();
  head.update();
  body.update();
  legs.update();

  //mise à jour des objets
  gun.update();
}

void GameCharacter::setPos(const Vector2f& pos)
{
  setPosition(pos);

  body.setAttachPoint(pos);

  if(characterDirection == rightDir){
    
    leftArm.setOrigin(8*scale,16*scale); // origine au niveau de l'épaule 
    rightArm.setOrigin(8*scale,16*scale); 
    gun.setOrigin(8*scale,16*scale);
  
    rightArm.setAttachPoint(pos.x-5*scale, pos.y-2*scale);
    leftArm.setAttachPoint(pos.x-2*scale, pos.y-2*scale);

  }else{

    leftArm.setOrigin(24*scale,16*scale); // origine au niveau de l'épaule 
    rightArm.setOrigin(24*scale,16*scale);
    gun.setOrigin(56*scale,16*scale);
  
    rightArm.setAttachPoint(pos.x+5*scale, pos.y-2*scale);
    leftArm.setAttachPoint(pos.x+2*scale, pos.y-2*scale);

  }
  
  gun.setAttachPoint(rightArm.attachPoint);

  head.setAttachPoint(pos.x, pos.y-9*scale);

  legs.setAttachPoint(pos.x, pos.y+8*scale);
}

void GameCharacter::setPos(const float x, const float y)
{
  setPos(Vector2f(x ,y));
}

void GameCharacter::Draw(GLint *renderUniforms) const
{
  leftArm.Draw(renderUniforms);
  legs.Draw(renderUniforms);
  body.Draw(renderUniforms);
  head.Draw(renderUniforms);
  gun.Draw(renderUniforms);
  rightArm.Draw(renderUniforms);

  for(Bullet b : gun.getBullets()){
    b.Draw(renderUniforms);
  }
}

void GameCharacter::lookAt(const Vector2f& lp)
{
  lookingPoint = lp;
  
  //on utilise le centre x entre la pos du body et du right arm pour déterminer la direction du character 
  float xCenter;
  if(characterDirection == rightDir){
    xCenter = rightArm.getPosition().x + ( (body.getPosition().x - rightArm.getPosition().x) / 2.f );
  }else{
    xCenter = body.getPosition().x + ( (rightArm.getPosition().x - body.getPosition().x) / 2.f );
  }

  // faut-il changer la direction ?
  if(characterDirection == rightDir && xCenter > lookingPoint.x){
    characterDirection = leftDir;
    flipY();
  }

  if(characterDirection == leftDir && xCenter < lookingPoint.x){
    characterDirection = rightDir;
    flipY();
  }


  #ifdef DEBUG
  //cout << "Character new looking point: " << lookingPoint.x << "|" << lookingPoint.y << endl;
  #endif

  setMemberRotation(leftArm);
  setMemberRotation(rightArm);
  setMemberRotation(head);

  gun.setRotation(rightArm.getRotation());
}

void GameCharacter::setMemberRotation(Sprite &member)
{
  setMemberRotation(member, 1.f);
}

void GameCharacter::setMemberRotation(Sprite &member, float m)
{
  // on détermine l'angle de rotation des bras pour suivre la souris 
  float distanceC = bbopGetDistance(member.getPosition(), lookingPoint);
  float distanceA = member.getPosition().x - lookingPoint.x;
  float distanceB = member.getPosition().y - lookingPoint.y;

  float rotation = acos((distanceA * distanceA + distanceC * distanceC - distanceB * distanceB) / (2 * distanceA * distanceC));
  rotation *= m;

  if(distanceA < 0)
    rotation += M_PI;
  if(distanceB < 0)
    rotation = -rotation;
  
  member.setRotation(rotation);

  #ifdef DEBUG 
  cout << "Character leftArm C distance to the looking Point: " << distanceC << endl;
  cout << "Character leftArm A distance to the looking Point: " << distanceA << endl;
  cout << "Character leftArm B distance to the looking Point: " << distanceB << endl;
  cout << "Character leftArm rotation to the looking Point: " << rotation << endl;
  cout << "----------------------------------------------------------------" << endl;
  #endif
}

Vector2f GameCharacter::getLookingPoint() const
{
  return lookingPoint;  
}

void GameCharacter::flipY()
{
  rightArm.flipVertically();
  leftArm.flipVertically();
  body.flipVertically();
  head.flipVertically();
  legs.flipVertically();

  gun.flipVertically();

  if(characterDirection == rightDir){
    
    leftArm.setOrigin(8*scale,8*scale); // origine au niveau de l'épaule 
    rightArm.setOrigin(8*scale,8*scale); 
  
    rightArm.setAttachPoint(body.getPosition().x-5*scale, rightArm.getPosition().y);
    leftArm.setAttachPoint(body.getPosition().x-2*scale, leftArm.getPosition().y);

    gun.gunDirection = rightDir;
  }else {
    leftArm.setOrigin(24*scale,8*scale); // origine au niveau de l'épaule
    rightArm.setOrigin(24*scale,8*scale);

    gun.gunDirection = leftDir;
  
    rightArm.setAttachPoint(body.getPosition().x+5*scale, rightArm.getPosition().y);
    leftArm.setAttachPoint(body.getPosition().x+2*scale, leftArm.getPosition().y);
  }
}

void GameCharacter::goLeft(const float newtonDiff)
{
  //gestion physique
  const b2Vec2 velocity(-(newtonX+newtonDiff), 0.0f);
  entityBody->ApplyForceToCenter(velocity, true);

  //gestion animation
  if(legs.state != run){
    legs.state = run;
    legs.animations[run].lastFrameStartTime = glfwGetTime();
    if(characterDirection == rightDir)
      legs.isReverse = true;
    else
      legs.isReverse = false;
  }
}

void GameCharacter::goRight(const float newtonDiff)
{
  //gestion physique
  const b2Vec2 velocity(newtonX+newtonDiff, 0.0f);
  entityBody->ApplyForceToCenter(velocity, true);

  //gestion animation
  if(legs.state != run) {
    legs.animations[run].lastFrameStartTime = glfwGetTime();
    legs.state = run;
    if(characterDirection == rightDir)
      legs.isReverse = false;
    else
      legs.isReverse = true;
  }
  
}

void GameCharacter::jump()
{
  auto* data = reinterpret_cast<BodyData*>(entityBody->GetUserData().pointer);
  if (data->isTouchingDown) {
    const b2Vec2 impulsion(0.0f, -entityBody->GetMass() * newtonY); // vers le haut
    entityBody->ApplyLinearImpulseToCenter(impulsion, true);
    data->jumpCpt++;
  }
}

//GETTER
Member& GameCharacter::getLeftArm() { return leftArm; }
Member& GameCharacter::getRightArm() { return rightArm; }
Member& GameCharacter::getBody() { return body; }
Member& GameCharacter::getHead() { return head; }
Member& GameCharacter::getLegs() { return legs; }
Gun& GameCharacter::getGun() { return gun; }
float GameCharacter::getHp() const {return hp;}

// SETTER
void GameCharacter::setHp(const float _hp){this->hp=_hp;}


//ENTITY 

void GameCharacter::computePhysic(b2World* world)
{
  setSize(20.f,50.f);
  setOrigin(getSize().x / 2, getSize().y / 2);
  entityBody = addDynamicBox(world, this, restitution, density, friction, linearDamping, true);

  auto* data = new BodyData;
  entityBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

void GameCharacter::updatePhysic()
{
  setPos(entityBody->GetPosition().x * PIXEL_PER_METER,
              entityBody->GetPosition().y * PIXEL_PER_METER - 5.f);
}
