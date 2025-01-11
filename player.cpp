#include "player.h"
#include "Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "Bbop-Library/include/BBOP/Graphics/cameraClass.h"
#include "game.h"

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

void Player::update(Camera *playerCam)
{

  //recup de la pos de la souris pour faire regarder le character 
  double mouseX, mouseY;
  glfwGetCursorPos(gameWindow, &mouseX, &mouseY);

  Vector2f mousePos = playerCam->screenPosToCamPos(Vector2f(static_cast<float>(mouseX),static_cast<float>(mouseY)));
  character.lookAt(mousePos);

  #ifdef DEBUG
  cout << "Position de la souris: " << mousePos.x << "|" << mousePos.y << endl;
  cout << "Position de la souris no convert: " << mouseX << "|" << mouseY << endl;
  #endif

  //gestion des inputs 
  if(glfwGetKey(gameWindow, GLFW_KEY_Q) == GLFW_PRESS){
    character.goLeft();
  }
  if(glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS){
    character.goRight();
  }


  character.update();
}
