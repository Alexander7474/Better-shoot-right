#include "gameCharacter.h"
#include "../../Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-Library/include/BBOP/Graphics/collisionBoxClass.h"
#include "../game/game.h"
#include "gun.h"
#include "member.h"

#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

string gameCharacterStateString[2] = { 
  "idle",
  "run"
};

GameCharacter::GameCharacter()
  : speed(250.f),
    jumpForce(200.f),
    weight(1.f),
    position(0.f,0.f),
    inertie(0.f, 0.f),
    forceInertie(2.f),
    startFall(glfwGetTime()),
    canJump(false),
    startJump(glfwGetTime()),
    jumpTime(0.1f),
    isJumping(false)
{
  characterDirection = rightDir;

  //--------------------------------------------------------------

  leftArm.setSize(32,32);
  //leftArm.getCollisionBox().setOffsetX(3.f,3.f);
  //leftArm.getCollisionBox().setOffsetY(6.f,10.f);
  leftArm.name = "left arm";

  rightArm.setSize(32,32);
  //rightArm.getCollisionBox().setOffsetX(3.f,3.f);
  //rightArm.getCollisionBox().setOffsetY(4.f,15.f);
  rightArm.name = "right arm";

  body.setSize(32,32);
  body.setOrigin(16,16); // origine au centre du torse
  body.name = "body";

  head.setSize(32,32);
  head.setOrigin(16,16); // origine au coup
  head.name = "head";

  legs.setSize(64,32);
  legs.setOrigin(32,0); // origine sur les hanche
   legs.name = "legs";

  gun.setSize(64,32);

  //--------------------------------------------------------------

  setPos(Vector2f(300.f,0.f));
  createTextureCache("assets/personnages/soldier/");
}

void GameCharacter::createTextureCache(string path)
{
  string armLeftPath = path + "arm_left/";
  leftArm.createTextureCache(armLeftPath);

  string armRightPath = path + "arm_right/";
  rightArm.createTextureCache(armRightPath);

  string bodyPath = path + "body/";
  body.createTextureCache(bodyPath);

  string headPath = path + "head/";
  head.createTextureCache(headPath);

  string legsPath = path + "legs/";
  legs.createTextureCache(legsPath);
}

void GameCharacter::update(Map* map)
{
  //si imoblie alors state = idle 
  if(inertie.x == 0)
    legs.state = idle;

  //application de la gravité si pas de saut
  inertie.y += (glfwGetTime() - startFall) * GRAVITY * DELTA_TIME;

  if(isJumping && glfwGetTime() - startJump >= jumpTime)
    isJumping = false;

  if(isJumping){
    inertie.y = -jumpForce * DELTA_TIME;
    startFall = glfwGetTime();
  } 

  // application de l'inertie final calculé
  setPos(position.x + inertie.x, position.y + inertie.y);

  //check des collisions au sol
  bool groundCollide = false;
  for(CollisionBox & box : map->getCollision()){
    if(legs.getCollisionBox().check(box) && box.getTop() < legs.getCollisionBox().getBottom()){
      inertie.y = 0.f;
      startFall = glfwGetTime();
      setPos(position.x, box.getTop()-40.f);
      canJump = true;
      groundCollide = true;
    }
  }

  //gestion des état après les déplacement 
  if(!groundCollide && legs.state == run)
    legs.state = idle;

  leftArm.update();
  rightArm.update();
  head.update();
  body.update();
  legs.update();

  gun.update();

  #ifdef DEBUG 
  cout << "Character inertie: " << inertie.x << "|" << inertie.y << endl;
  #endif

  //diminution de l'inertie en x 
  inertie.x = inertie.x / forceInertie;
  if(inertie.x > -10.f*DELTA_TIME && inertie.x < 10.f*DELTA_TIME)
    inertie.x = 0.f;

  #ifdef DEBUG 
  cout << "Character after slow inertie: " << inertie.x << "|" << inertie.y << endl;
  cout << "Character pos: " << position.x << "|" << position.y << endl;
  cout << "Character fall start: " << startFall << endl;
  cout << "Character Jump start: " << startJump << endl;
  cout << "Character Jump time start: " << glfwGetTime() - startJump << endl;
  cout << "----------------------------------------------------------------" << endl;
  #endif
}

void GameCharacter::setPos(Vector2f pos)
{
  position = pos;

  body.setAttachPoint(pos);

  if(characterDirection == rightDir){
    
    leftArm.setOrigin(8,16); // origine au niveau de l'épaule 
    rightArm.setOrigin(8,16); 
    gun.setOrigin(8,16);
  
    rightArm.setAttachPoint(pos.x-5, pos.y-2);
    leftArm.setAttachPoint(pos.x-2, pos.y-2);

  }else{

    leftArm.setOrigin(24,16); // origine au niveau de l'épaule 
    rightArm.setOrigin(24,16);
    gun.setOrigin(56,16);
  
    rightArm.setAttachPoint(pos.x+5, pos.y-2);
    leftArm.setAttachPoint(pos.x+2, pos.y-2);

  }
  
  gun.setAttachPoint(rightArm.attachPoint);

  head.setAttachPoint(pos.x, pos.y-9);

  legs.setAttachPoint(pos.x, pos.y+8);
}

void GameCharacter::setPos(float x, float y)
{
  setPos(Vector2f(x ,y));
}

void GameCharacter::Draw(GLint renderModeLoc) const
{
  leftArm.Draw(renderModeLoc);
  legs.Draw(renderModeLoc);
  body.Draw(renderModeLoc);
  head.Draw(renderModeLoc);
  gun.Draw(renderModeLoc);
  rightArm.Draw(renderModeLoc);

  for(Bullet b : gun.getBullets()){
    b.Draw(renderModeLoc);
  }
}

void GameCharacter::lookAt(Vector2f lp)
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
  cout << "Character new looking point: " << lookingPoint.x << "|" << lookingPoint.y << endl;
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
    
    leftArm.setOrigin(8,8); // origine au niveau de l'épaule 
    rightArm.setOrigin(8,8); 
  
    rightArm.setAttachPoint(body.getPosition().x-5, rightArm.getPosition().y);
    leftArm.setAttachPoint(body.getPosition().x-2, leftArm.getPosition().y);

    gun.gunDirection = rightDir;
  }else{

    leftArm.setOrigin(24,8); // origine au niveau de l'épaule 
    rightArm.setOrigin(24,8);

    gun.gunDirection = leftDir;
  
    rightArm.setAttachPoint(body.getPosition().x+5, rightArm.getPosition().y);
    leftArm.setAttachPoint(body.getPosition().x+2, leftArm.getPosition().y);
  }

  #ifdef DEBUG
  if(characterDirection == rightDir){
    cout << "Character flipped to right" << endl;
  }else{
    cout << "Character flipped to left" << endl;
  }
  #endif
}

void GameCharacter::goLeft()
{
  inertie.x = -speed * DELTA_TIME;
  if(legs.state != run){
    legs.state = run;
    legs.animations[run].lastFrameStartTime = glfwGetTime();
    if(characterDirection == rightDir)
      legs.isReverse = true;
    else
      legs.isReverse = false;
  }
}

void GameCharacter::goRight()
{
  inertie.x = speed * DELTA_TIME;
  if(legs.state != run){
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
  if(canJump){
    canJump = false;
    isJumping = true;
    startJump = glfwGetTime();
  }
}

//GETTER 

Member& GameCharacter::getLeftArm() { return leftArm; }
Member& GameCharacter::getRightArm() { return rightArm; }
Member& GameCharacter::getBody() { return body; }
Member& GameCharacter::getHead() { return head; }
Member& GameCharacter::getLegs() { return legs; }
Gun& GameCharacter::getGun() { return gun; }
