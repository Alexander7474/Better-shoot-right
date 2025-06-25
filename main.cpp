#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Bbop-2D/include/BBOP/Graphics/bbopFunc.h"
#include "src/game/game.h"

using namespace std;

int main()
{
  //initialisation de la lib
  bbopInit(2560,1440,"window name",gameWindow);
  bbopChangeWindowResolution(640,360);
  cout << "Version prototype" << endl;

  Game game;

  glfwSwapInterval(1);

  for(string &s: LOGS){
    cout << s << endl;
  }

  //main while loop
  while (!glfwWindowShouldClose(gameWindow))
  {
    double timeSave = glfwGetTime();

    // clear de la fenêtre en noire
    bbopCleanWindow(gameWindow,Vector3i(0,0,0),1.0f);
    
    game.update();

    game.Draw();
    
    // vérification des erreurs
    bbopErrorCheck();
    // swap du back buffer avec le front buffer
    glfwSwapBuffers(gameWindow);
    // recupération des events glfw
    glfwPollEvents();

    DELTA_TIME = glfwGetTime() - timeSave;
    FPS = 1/DELTA_TIME;
  }
  // destruction de la fen^tre et fin de glfw
  glfwDestroyWindow(gameWindow);
  glfwTerminate();

  return 0;
}
