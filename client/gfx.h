#ifndef CLIENT_GFX_H
#define CLIENT_GFX_H

#include <GLFW/glfw3.h>

namespace client
{

bool initializeGfx();
void terminateGfx();

void setErrorCallback(void (*error_callback)(int error, const char* description));
void setKeyCallback(void (*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mods));

bool createWindow(int w, int h, const char* title);
bool windowShouldClose();
void pollEvents();
void swapBuffers();

void setCameraPosition(int x, int y);

void setTime(double time);
double getTime();

}

#endif //CLIENT_GFX_H
