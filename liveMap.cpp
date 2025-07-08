#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sys/stat.h>
#include <ctime>

#include "Bbop-2D/include/BBOP/Graphics.h"
#include "Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "Bbop-2D/include/BBOP/Graphics/mapClass.h"

using namespace std;

Camera *camera;

extern const std::string mapPath = "assets/map/map2/";
extern const std::string mapPathLdtk = "assets/map/map2/map.ldtk";

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera->setScale(camera->getScale() + (-yoffset)*0.1);
} 

std::time_t* getFileModificationTime(const std::string& filePath) {
    struct stat fileStat;

    // Obtenir les informations du fichier
    if (stat(filePath.c_str(), &fileStat) == 0) {
      std::time_t* t = new std::time_t(fileStat.st_mtime);  
      return t;
    }

  return nullptr;
}

int main()
{
  //initialisation de la lib
  GLFWwindow *gameWindow;
  bbopInit(640,360,"window name",gameWindow);
  cout << "Version prototype" << endl;

  glfwSwapInterval(1);

  getFileModificationTime(mapPath);

  Scene scene;
  camera = new Camera;
  Map *map;
  map = new Map(mapPath.c_str());

  Vector2f lastMousePos(0.f,0.f);
  Vector2f lastCamPos = camera->getPosition();

  std::time_t* modifT = getFileModificationTime(mapPathLdtk);

  glfwSetScrollCallback(gameWindow, scroll_callback);

  //main while loop
  while (!glfwWindowShouldClose(gameWindow))
  {
    // clear de la fenêtre en noire
    bbopCleanWindow(gameWindow,Vector3i(0,0,0),1.0f);

    std::time_t* x = getFileModificationTime(mapPathLdtk);
    if(*modifT != *x){
      modifT = x;
      cerr << "Rechargement de la map" << endl;
      delete map;
      map = new Map(mapPath.c_str());
    }

    // Mettre a jour la camera 
    if(glfwGetMouseButton(gameWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
      double xpos, ypos;
      glfwGetCursorPos(gameWindow, &xpos, &ypos);
      Vector2f mousePos(static_cast<float>(xpos), static_cast<float>(ypos));
      mousePos = camera->screenPosToCamPos(mousePos);
      mousePos = camera->camPosToWorldPos(mousePos);

      Vector2f diffMouse(mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y);

      Vector2f diffCam(camera->getPosition().x - lastCamPos.x, camera->getPosition().y - lastCamPos.y);

      if(diffCam.x != diffMouse.x || diffCam.y != diffMouse.y){
        camera->setPosition(Vector2f(lastCamPos.x + diffMouse.x, lastCamPos.y + diffMouse.y));
      }

    }else{
      double xpos, ypos;
      glfwGetCursorPos(gameWindow, &xpos, &ypos);
      Vector2f mousePos(static_cast<float>(xpos), static_cast<float>(ypos));
      mousePos = camera->screenPosToCamPos(mousePos);
      mousePos = camera->camPosToWorldPos(mousePos);

      lastMousePos = mousePos;
      lastCamPos = camera->getPosition();
    }

    map->update();

    map->Draw(scene, *camera);

    scene.render();

    // vérification des erreurs
    bbopErrorCheck();
    // swap du back buffer avec le front buffer
    glfwSwapBuffers(gameWindow);
    // recupération des events glfw
    glfwPollEvents();
  }
  // destruction de la fen^tre et fin de glfw
  glfwDestroyWindow(gameWindow);
  glfwTerminate();


  return 0;
}


