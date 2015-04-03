#ifndef CLIENT_GFX_H
#define CLIENT_GFX_H

#include <GLFW/glfw3.h>

#include "client/gui/imgui.h"

namespace gfx
{

bool initialize();
void terminate();

//void setErrorCallback(void (*error_callback)(int error, const char* description));
void setKeyCallback(void (*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mods));

bool createWindow(int w, int h, const char* title);
bool windowShouldClose();
void pollEvents();
void swapBuffers();

void setCameraPosition(float x, float y);
void setCameraWidth(float w);
void setCameraHeight(float h);
void moveCamera(float dx, float dy);
void updateCamera();

//void setTime(double time);
double getTime();

}

#endif //CLIENT_GFX_H
