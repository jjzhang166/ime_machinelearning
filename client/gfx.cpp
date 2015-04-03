#include <cstdio>

#include "client/gfx.h"
#include "client/gui/imgui.h"

namespace gfx
{

GLFWwindow* window = nullptr;
float window_w,
      window_h;
float camera_x,
      camera_y,
      camera_w,
      camera_h;
bool mousePressed[2] = {false, false};

void (*custom_keycallback)(GLFWwindow*, int, int, int, int) = nullptr;

// Private
namespace priv
{
static void errorCallback(int error, const char* description)
{
  fprintf(stderr, "Error %d: %s\n", error, description);
}

static void mouseButtonCallback(GLFWwindow* /*window*/, int button,
                                int action, int /*mods*/)
{
  // ImGui
  if (action == GLFW_PRESS && button >= 0 && button < 2)
    mousePressed[button] = true;
}

static void scrollCallback(GLFWwindow* /*window*/, double /*xoffset*/,
                               double yoffset)
{
  // ImGui
  // Use fractional mouse wheel, 1.0 unit 5 lines.
  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheel += (float)yoffset;
}

static void keyCallback(GLFWwindow* window, int key, int scancode,
                        int action, int mods)
{
  // ImGui
  ImGuiIO& io = ImGui::GetIO();
  if (action == GLFW_PRESS)
    io.KeysDown[key] = true;
  if (action == GLFW_RELEASE)
    io.KeysDown[key] = false;
  io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
  io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;

  // Custom key_callback
  if (custom_keycallback)
    custom_keycallback(window, key, scancode, action, mods);

  // TODO(naum): Use this for test only!
#ifndef NDEBUG
  // Custom
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    glfwSetWindowShouldClose(window, 1);
#endif
}

static void charModsCallback(GLFWwindow* /*window*/, unsigned int codepoint,
                             int /*mods*/)
{
  // ImGui
  if (codepoint > 0 && codepoint < 0x10000)
    ImGui::GetIO().AddInputCharacter((unsigned short)codepoint);
}

// This is the main rendering function that you have to implement and provide to
// ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by
// (0.5f,0.5f) or (0.375f,0.375f)
static void ImImpl_RenderDrawLists(ImDrawList **const cmd_lists,
                                   int cmd_lists_count)
{
  if (cmd_lists_count == 0)
    return;

  // We are using the OpenGL fixed pipeline to make the example code simpler to
  // read!
  // A probable faster way to render would be to collate all vertices from all
  // cmd_lists into a single vertex buffer.
  // Setup render state: alpha-blending enabled, no face culling, no depth
  // testing, scissor enabled, vertex/texcoord/color pointers.
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnable(GL_TEXTURE_2D);

  // Setup orthographic projection matrix
  const float width = ImGui::GetIO().DisplaySize.x;
  const float height = ImGui::GetIO().DisplaySize.y;
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Render command lists
  for (int n = 0; n < cmd_lists_count; n++) {
    const ImDrawList *cmd_list = cmd_lists[n];
    const unsigned char *vtx_buffer =
        (const unsigned char *)&cmd_list->vtx_buffer.front();

#define OFFSETOF(TYPE, ELEMENT) ((size_t) & (((TYPE *)0)->ELEMENT))
    glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert),
                    (void *)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert),
                      (void *)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert),
                   (void *)(vtx_buffer + OFFSETOF(ImDrawVert, col)));
#undef OFFSETOF

    int vtx_offset = 0;
    for (size_t cmd_i = 0; cmd_i < cmd_list->commands.size(); cmd_i++) {
      const ImDrawCmd *pcmd = &cmd_list->commands[cmd_i];
      glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t) pcmd->texture_id);
      glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w),
                (int)(pcmd->clip_rect.z - pcmd->clip_rect.x),
                (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
      glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
      vtx_offset += pcmd->vtx_count;
    }
  }

  // Restore modified state
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

static void loadFontsTexture()
{
  ImGuiIO &io = ImGui::GetIO();

  unsigned char *pixels;
  int width, height;
  io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

  GLuint tex_id;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, pixels);

  io.Fonts->TexID = (void *)(intptr_t) tex_id;
}

static void initGui()
{
  ImGuiIO &io = ImGui::GetIO();
  io.DeltaTime = 1.0f / 60.0f; // Time elapsed since last frame, in seconds (in
                               // this sample app we'll override this every
                               // frame because our time step is variable)
  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB; // Keyboard mapping. ImGui will use
                                          // those indices to peek into the
                                          // io.KeyDown[] array.
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

  io.RenderDrawListsFn = ImImpl_RenderDrawLists;

  loadFontsTexture();
}

static void updateGui()
{
  ImGuiIO& io = ImGui::GetIO();

  // Setup resolution (every frame to accommodate for window resizing)
  int w, h;
  int display_w, display_h;
  glfwGetWindowSize(window, &w, &h);
  glfwGetFramebufferSize(window, &display_w, &display_h);
  io.DisplaySize = ImVec2(
      (float)display_w, (float)
      display_h); // Display size, in pixels. For clamping windows positions.

  // Setup time step
  static double time = 0.0f;
  const double current_time = glfwGetTime();
  io.DeltaTime = (float)(current_time - time);
  time = current_time;

  // Setup inputs
  double mouse_x, mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  mouse_x *= (float)display_w / w; // Convert mouse coordinates to pixels
  mouse_y *= (float)display_h / h;
  io.MousePos = ImVec2((float)mouse_x, (float)mouse_y); // Mouse position, in
                                                        // pixels (set to -1,-1
                                                        // if no mouse / on
                                                        // another screen, etc.)

  io.MouseDown[0] = mousePressed[0] ||
                    glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) !=
                        0; // If a mouse press event came, always pass it as
                           // "mouse held this frame", so we don't miss
                           // click-release events that are shorter than 1
                           // frame.
  io.MouseDown[1] = mousePressed[1] ||
                    glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != 0;

  // Start the frame
  ImGui::NewFrame();
}

static void renderGui()
{
  ImGuiIO& io = ImGui::GetIO();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);

  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  ImGui::Render();

  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

}
// End of Private

bool initialize()
{
  if (!glfwInit())
  {
    fprintf(stderr, "Could not initialize GLFW!\n");
    return false;
  }

  return true;
}

void terminate()
{
  // Gui
  ImGui::Shutdown();

  // GLFW
  glfwTerminate();
}

// TODO(naum): Add custom error callback?
/*
void setErrorCallback(void (*error_callback)(int error, const char* description))
{
  glfwSetErrorCallback(error_callback);
}
*/

void setKeyCallback(void (*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mods))
{
  //glfwSetKeyCallback(window, key_callback);
  custom_keycallback = key_callback;
}

bool createWindow(int w, int h, const char* title)
{
  // TODO(naum): Change this
  glfwWindowHint(GLFW_SAMPLES, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window_w = w;
  window_h = h;
  window = glfwCreateWindow(window_w, window_h, title, 0, 0);
  if (!window)
  {
    fprintf(stderr, "Could not initialize GLFW Window!\n");
    terminate();
    return false;
  }

  // callbacks
  glfwSetErrorCallback(priv::errorCallback);
  glfwSetKeyCallback(window, priv::keyCallback);
  glfwSetCharModsCallback(window, priv::charModsCallback);
  glfwSetMouseButtonCallback(window, priv::mouseButtonCallback);
  glfwSetScrollCallback(window, priv::scrollCallback);

  glfwMakeContextCurrent(window);

  // Setup OpenGL
  camera_w = window_w;
  camera_h = window_h;
  setCameraPosition(w / 2.f, h / 2.f);

  // Setup GUI
  priv::initGui();

  return true;
}

bool windowShouldClose()
{
  return glfwWindowShouldClose(window);
}

float getWindowWidth()
{
  return window_w;
}

float getWindowHeight()
{
  return window_h;
}

void pollEvents()
{
  mousePressed[0] = mousePressed[1] = false;

  glfwPollEvents();
  priv::updateGui();
}

void swapBuffers()
{
  priv::renderGui();
  glfwSwapBuffers(window);
  glClear(GL_COLOR_BUFFER_BIT);
}

void setCameraPosition(float x, float y)
{
  camera_x = x;
  camera_y = y;
  updateCamera();
}

void setCameraWidth(float w)
{
  camera_w = w;
  camera_h = window_h * w / window_w;
  updateCamera();
}

void setCameraHeight(float h)
{
  camera_h = h;
  camera_w = window_w * h / window_h;
  updateCamera();
}

void moveCamera(float dx, float dy)
{
  camera_x += dx;
  camera_y += dy;
  updateCamera();
}

void updateCamera()
{
  // XXX(naum): Use modern pipeline?
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(camera_x - camera_w / 2.f, camera_x + camera_w / 2.f,
          camera_y - camera_h / 2.f, camera_y + camera_h / 2.f,
          1.f, -1.f);
  glMatrixMode(GL_MODELVIEW);
}

float getCameraWidth()
{
  return camera_w;
}

float getCameraHeight()
{
  return camera_h;
}

/*
>>>>>>> gfx
void setTime(double time)
{
  glfwSetTime(time);
}
*/

double getTime()
{
  return glfwGetTime();
}

}
