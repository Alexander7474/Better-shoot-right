#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <irrKlang/ik_ESoundOutputDrivers.h>
#include <irrKlang/irrKlang.h>

#include "Bbop-Library/include/BBOP/Graphics.h"

using namespace std;

int main()
{
  //initialisation de la lib
  GLFWwindow *gameWindow;
  bbopInit(2080,512,"window name",gameWindow);
  cout << "Version prototype" << endl;

  glfwSwapInterval(1);

  Scene scene;
  Camera camera;
  Map map;

  //main while loop
  while (!glfwWindowShouldClose(gameWindow))
  {
    // clear de la fenêtre en noire
    bbopCleanWindow(gameWindow,Vector3i(0,0,0),1.0f);
    
    map.update();

    map.Draw(scene, camera);

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
