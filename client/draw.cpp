#include <cmath>

#include "client/draw.h"
#include "client/gfx.h"

namespace gfx
{

void drawCircle(float radius, float thickness, int divisions)
{
  drawArc(radius, thickness, 360.f, divisions);
}

void drawFilledCircle(float radius, int divisions)
{
  glBegin(GL_POLYGON);
  for (int i = 0; i <= divisions; ++i)
    glVertex3f(radius * cosf(i * 2 * M_PI / divisions),
               radius * sinf(i * 2 * M_PI / divisions),
               0.f);
  glEnd();
}

void drawArc(float radius, float thickness, float angle, int divisions)
{
  const auto a = 2 * M_PI * angle / 360.f;
  glBegin(GL_QUAD_STRIP);
  float c, s;
  for (int i = 0; i <= divisions; ++i)
  {
    c = cosf(i * a / divisions);
    s = sinf(i * a / divisions);
    glVertex3f(radius * c,
               radius * s,
               0.f);
    glVertex3f((radius + thickness) * c,
               (radius + thickness) * s,
               0.f);
  }
  glEnd();
}

void drawRect(float w, float h)
{
  glBegin(GL_QUADS);
  glVertex3f(-w / 2,  h / 2, 0.f);
  glVertex3f( w / 2,  h / 2, 0.f);
  glVertex3f( w / 2, -h / 2, 0.f);
  glVertex3f(-w / 2, -h / 2, 0.f);
  glEnd();
}

}
