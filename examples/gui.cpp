#include <cstdio>

#include "client/gfx.h"

int main()
{
  if(!gfx::initialize())
    return 1;

  if (!gfx::createWindow(640, 480, "Gfx Example"))
    return 2;

  float move_amount = 0.f;
  float rotation = 0.f,
        rotation_rate = 90.f;

  double timer = gfx::getTime(),
         delta_time = 0.0;
  while (!gfx::windowShouldClose())
  {
    // TODO(naum): Create a gfx::getDeltaTime()
    auto t = gfx::getTime();
    delta_time = timer - t;
    timer = t;

    // Update rotation
    rotation += rotation_rate * delta_time;

    // NOTE(naum): Must be called before any ImGui function
    gfx::pollEvents();

    gfx::moveCamera(-10 * move_amount, 0);

    glPushMatrix();
    glTranslatef(320.f, 240.f, 0.f);
    glRotatef(rotation, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f); glVertex3f(  0.f, 107.f, 0.f);
    glColor3f(0.f, 1.f, 0.f); glVertex3f(-92.f, -53.f, 0.f);
    glColor3f(0.f, 0.f, 1.f); glVertex3f( 92.f, -53.f, 0.f);
    glEnd();
    glPopMatrix();

    // GUI
    {
      if (ImGui::Begin("Test window", // Window title
                       nullptr,    // (bool*) opened
                       {200, 100}, // Size
                       0.8f,
                       ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_AlwaysAutoResize)) // Window flags
      {
        ImGui::Text("This is a test window!");
        if (ImGui::CollapsingHeader("Options", nullptr, true, true))
        {
          ImGui::SliderFloat("Move amount", &move_amount, -1.f, 1.f);
          ImGui::SliderFloat("Rotation rate (deg/sec)", &rotation_rate, -360.f, 360.f);
        }
      }
      ImGui::End();
    }

    // Swap
    gfx::swapBuffers();
  }

  gfx::terminate();

  return 0;
}
