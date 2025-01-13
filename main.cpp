#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Bbop-Library/include/BBOP/Graphics/sceneClass.h"
#include "game.h"

using namespace std;

int main()
{
  //initialisation de la lib
  bbopInit(640,360,"window name",gameWindow);
  cout << "Version prototype" << endl;

  Game game;

  glfwSwapInterval(1);
 
  #ifdef DEBUG
  LOGS.push_back("VERSION DE DEBUG");
  #endif

  //main while loop
  while (!glfwWindowShouldClose(gameWindow))
  {
    double timeSave = glfwGetTime();

    #ifdef DEBUG 
    system("clear");
    for(string &s: LOGS){
      cout << s << endl;
    }
    cout << "DELTA_TIME: " << DELTA_TIME << endl;
    cout << "----------------------------------------------------------------" << endl;
    #endif

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
  }
  // destruction de la fen^tre et fin de glfw
  glfwDestroyWindow(gameWindow);
  glfwTerminate();


  return 0;
}
