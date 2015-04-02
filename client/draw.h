#ifndef CLIENT_DRAW_H
#define CLIENT_DRAW_H

namespace gfx
{

const int CIRCLE_DIVISIONS = 32;

void drawCircle(float radius, float thickness, int divisions = CIRCLE_DIVISIONS);
void drawFilledCircle(float radius, int divisions = CIRCLE_DIVISIONS);
void drawArc(float radius, float thickness, float angle, int divisions = CIRCLE_DIVISIONS);
//void drawPolygon();

void drawRect(float w, float h);

}

#endif //CLIENT_DRAW_H
