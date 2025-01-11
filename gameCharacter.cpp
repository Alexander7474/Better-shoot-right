#include "gameCharacter.h"
#include "Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "game.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

GameCharacter::GameCharacter()
  : speed(100.f),
    jumpForce(1.f),
    weight(1.f)
{
  createTextureCache("img/soldier/");
  characterDirection = rightDir;
}

void GameCharacter::createTextureCache(string path)
{
  string armLeftPath = path + "arm_left/idle.png";
  leftArmTexture[idle].textures.push_back(Texture(armLeftPath.c_str()));

  string armRightPath = path + "arm_right/idle.png";
  rightArmTexture[idle].textures.push_back(Texture(armRightPath.c_str()));

  string bodyPath = path + "body/idle.png";
  bodyTexture[idle].textures.push_back(Texture(bodyPath.c_str()));

  string headPath = path + "head/idle.png";
  headTexture[idle].textures.push_back(Texture(headPath.c_str()));

  string legsPath = path + "legs/idle.png";
  legsTexture[idle].textures.push_back(Texture(legsPath.c_str()));

  //--------------------------------------------------------------

  leftArm.setTexture(leftArmTexture[idle].textures[0]);
  leftArm.setSize(32,32);
  leftArm.setOrigin(8,8); // origine au niveau de l'épaule 

  rightArm.setTexture(rightArmTexture[idle].textures[0]);
  rightArm.setSize(32,32);
  rightArm.setOrigin(8,8); // origine au niveau de l'épaule 

  body.setTexture(bodyTexture[idle].textures[0]);
  body.setSize(32,32);
  body.setOrigin(16,16); // origine au centre du torse

  head.setTexture(headTexture[idle].textures[0]);
  head.setSize(32,32);
  head.setOrigin(16,16); // origine au coup

  legs.setTexture(legsTexture[idle].textures[0]);
  legs.setSize(64,32);
  legs.setOrigin(32,0); // origine sur les hanche
 
  //--------------------------------------------------------------

  setPos(Vector2f(200.f,200.f));
}

void GameCharacter::update()
{

  #ifdef DEBUG 
  cout << "Character pos: " << position.x << "|" << position.y << endl;
  #endif
}

void GameCharacter::setPos(Vector2f pos)
{
  position = pos;

  body.setPosition(pos);

  if(characterDirection == rightDir){
    
    leftArm.setOrigin(8,8); // origine au niveau de l'épaule 
    rightArm.setOrigin(8,8); 
  
    rightArm.setPosition(pos.x-5, pos.y-2);
    leftArm.setPosition(pos.x-2, pos.y-2);
  }else{

    leftArm.setOrigin(24,8); // origine au niveau de l'épaule 
    rightArm.setOrigin(24,8);
  
    rightArm.setPosition(pos.x+5, pos.y-2);
    leftArm.setPosition(pos.x+2, pos.y-2);
  }

  head.setPosition(pos.x, pos.y-9);

  legs.setPosition(pos.x, pos.y+8);
}

void GameCharacter::Draw(GLint renderModeLoc) const
{
  leftArm.Draw(renderModeLoc);
  legs.Draw(renderModeLoc);
  body.Draw(renderModeLoc);
  head.Draw(renderModeLoc);
  rightArm.Draw(renderModeLoc);
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
}

void GameCharacter::setMemberRotation(Sprite &member)
{
  // on détermine l'angle de rotation des bras pour suivre la souris 
  float distanceC = bbopGetDistance(member.getPosition(), lookingPoint);
  float distanceA = member.getPosition().x - lookingPoint.x;
  float distanceB = member.getPosition().y - lookingPoint.y;

  float rotation = acos((distanceA * distanceA + distanceC * distanceC - distanceB * distanceB) / (2 * distanceA * distanceC));

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

  if(characterDirection == rightDir){
    
    leftArm.setOrigin(8,8); // origine au niveau de l'épaule 
    rightArm.setOrigin(8,8); 
  
    rightArm.setPosition(body.getPosition().x-5, rightArm.getPosition().y);
    leftArm.setPosition(body.getPosition().x-2, leftArm.getPosition().y);
  }else{

    leftArm.setOrigin(24,8); // origine au niveau de l'épaule 
    rightArm.setOrigin(24,8);
  
    rightArm.setPosition(body.getPosition().x+5, rightArm.getPosition().y);
    leftArm.setPosition(body.getPosition().x+2, leftArm.getPosition().y);
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
  setPos(Vector2f(position.x-speed*DELTA_TIME, position.y));
}

void GameCharacter::goRight()
{
  setPos(Vector2f(position.x+speed*DELTA_TIME, position.y));
}

void GameCharacter::jump()
{

}
