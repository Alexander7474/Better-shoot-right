#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifdef IMGUI_DEBUG
#include "src/backends/imgui_impl_glfw.h"
#include "src/backends/imgui_impl_opengl3.h"
#include "src/imgui/imgui.h"
#endif

#include "Bbop-2D/include/BBOP/Graphics/bbopFunc.h"
#include "src/game/game.h"

using namespace std;

int main() {
        // initialisation de la lib
        bbopInit(1920, 1080, "window name", gameWindow);
        bbopChangeWindowResolution(640, 360);
        cout << "Version prototype" << endl;

#ifdef IMGUI_DEBUG
        // ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        // Setup ImGui style
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(
            1.5f); // agrandit tous les espacements, bordures, etc.
        io.FontGlobalScale = 1.5f; // agrandit tout le texte

        // Backend ImGui + GLFW + OpenGL
        ImGui_ImplGlfw_InitForOpenGL(gameWindow, true);
        ImGui_ImplOpenGL3_Init();
#endif

        Game game;

        glfwSwapInterval(1);

        for (string &s : LOGS) {
                cout << s << endl;
        }

        // main while loop
        while (!glfwWindowShouldClose(gameWindow)) {
                const double timeSave = glfwGetTime();

#ifdef IMGUI_DEBUG
                // Démarrer frame ImGui
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
#endif

                // clear de la fenêtre en noire
                bbopCleanWindow(gameWindow, Vector3i(0, 0, 0), 1.0f);

                game.update();
                game.Draw();

#ifdef IMGUI_DEBUG
                // Interface performance
                ImGui::Begin("Performance Info");
                ImGui::Text("FPS: %.2f, DELTA_TIME: %.2f)", FPS, DELTA_TIME);
                ImGui::End();
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

                // vérification des erreurs
                bbopErrorCheck();
                // swap du back buffer avec le front buffer
                glfwSwapBuffers(gameWindow);
                // recupération des events glfw
                glfwPollEvents();

                DELTA_TIME = glfwGetTime() - timeSave;

                FPS_COUNTER++;
                if (glfwGetTime() - LAST_FPS_UPDATE >= 1.0) {
                        FPS = FPS_COUNTER / (glfwGetTime() - LAST_FPS_UPDATE);
                        LAST_FPS_UPDATE = glfwGetTime();
                        FPS_COUNTER = 0;
                }
        }

// Nettoyage
#ifdef IMGUI_DEBUG
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
#endif
        glfwDestroyWindow(gameWindow);
        glfwTerminate();

        return 0;
}
