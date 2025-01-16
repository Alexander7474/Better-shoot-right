#include "player.h"
#include "../../Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "game.h"
#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

Player::Player()
{

}

void Player::Draw(GLint renderModeLoc) const
{
  character.Draw(renderModeLoc);
}

void Player::update(Camera *playerCam, Map* map)
{

  //recup de la pos de la souris pour faire regarder le character 
  double mouseX, mouseY;
  glfwGetCursorPos(gameWindow, &mouseX, &mouseY);

  Vector2f mousePos = playerCam->screenPosToCamPos(Vector2f(static_cast<float>(mouseX),static_cast<float>(mouseY)));
  character.lookAt(mousePos);

  #ifdef DEBUG
  cout << "Position de la souris: " << mousePos.x << "|" << mousePos.y << endl;
  cout << "Position de la souris no convert: " << mouseX << "|" << mouseY << endl;
  cout << "----------------------------------------------------------------" << endl;
  #endif

  //gestion des inputs 
  if(glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_PRESS){
    character.goLeft();
  }
  if(glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS){
    character.goRight();
  }
  if(glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS){
    character.jump();
  }
  if(glfwGetKey(gameWindow, GLFW_KEY_R) == GLFW_PRESS){
    character.getGun().shoot();
  }
  character.update(map);
}

GameCharacter& Player::getCharacter() { return character;}
