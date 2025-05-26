#include "player.h"
#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "game.h"
#include "../engine/gameCharacter.h"

#include <GLFW/glfw3.h>

using namespace std;

Player::Player()
{

}

void Player::Draw(GLint *renderUniforms) const
{
  character.Draw(renderUniforms);
  crossair.Draw(renderUniforms);
}

void Player::update(Camera *playerCam, Map* map)
{
  //recup de la pos de la souris pour faire regarder le character 
  double mouseX, mouseY;
  glfwGetCursorPos(gameWindow, &mouseX, &mouseY);

  Vector2f mousePos = playerCam->screenPosToCamPos(Vector2f(static_cast<float>(mouseX),static_cast<float>(mouseY)));
  mousePos = playerCam->camPosToWorldPos(mousePos);
  character.lookAt(mousePos);

  float crossairS = bbopGetDistance(mousePos, character.getPosition());
  crossair.setSize(crossairS/10.f,crossairS/10.f);

  crossair.setPosition(mousePos.x, mousePos.y);

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
    character.getGun().reload();
  }
  if(glfwGetMouseButton(gameWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
    character.getGun().shoot();
  }
  character.update(map);
}

GameCharacter& Player::getCharacter() { return character;}
Crossair& Player::getCrossair() { return crossair;}
