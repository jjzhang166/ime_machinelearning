#ifndef CLIENT_BODY_H
#define CLIENT_BODY_H

#include <memory>

class b2Shape;
class b2Body;

namespace gfx
{

class World;

class Body
{
public:
  Body(World&, float x, float y, float angle = 0.0f);
  virtual ~Body();

  void setStatic(bool isStatic);

  // TODO(naum): Add filter and mask
  // TODO(naum): Think a better way (to not use b2Shape)
  void addShape(const b2Shape* shape);
  void addFixture(const b2Shape* shape,
                  float density,
                  float friction,
                  float restitution);

  void render();

protected:
  World* world_;

  b2Body* body_;

  bool active_;
private:
};

}

#endif //CLIENT_BODY_H
