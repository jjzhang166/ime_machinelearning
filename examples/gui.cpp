#include <cstdio>

#include "client/gfx.h"

int main()
{
  if(!gfx::initialize())
    return 1;

  if (!gfx::createWindow(640, 480, "Gfx Example"))
    return 2;

  float move_amount = 0.f;
  bool show_test_window = false;
  while (!gfx::windowShouldClose())
  {
    // NOTE(naum): Must be called before any ImGui function
    gfx::pollEvents();

    //gfx::moveCamera(10 * move_amount, 0);

    glLoadIdentity();
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f); glVertex3f(200.f, 100.f, 0.f);
    glColor3f(0.f, 1.f, 0.f); glVertex3f(100.f, 273.f, 0.f);
    glColor3f(0.f, 0.f, 1.f); glVertex3f(300.f, 273.f, 0.f);
    glEnd();

    // GUI
    /*
    {
      if (ImGui::Button("Test Window"))
        show_test_window ^= 1;
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    */

    {
      ImGui::SetNextWindowPos({100, 200}, ImGuiSetCond_FirstUseEver);
      if (ImGui::Begin("Test window", // Window title
                       nullptr,    // (bool*) opened
                       {200, 100}, // Size
                       0.8f,
                       ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoSavedSettings)) // Window flags
      {
        ImGui::Text("This is a test window!");
        if (ImGui::CollapsingHeader("Options"))
        {
          ImGui::SliderFloat("Move amount", &move_amount, -1.f, 1.f);
        }
      }
      ImGui::End();
    }

    if (show_test_window)
    {
      ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiSetCond_FirstUseEver);
      ImGui::ShowTestWindow(&show_test_window);
    }

    // Swap
    gfx::swapBuffers();
  }

  gfx::terminate();

  return 0;
}
