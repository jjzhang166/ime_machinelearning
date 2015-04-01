#ifndef CLIENT_BODY_H
#define CLIENT_BODY_H

#include <memory>

#include <Box2D/Box2D.h>

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

  b2Vec2 getPosition() const;

  void render();

protected:
  World& world_;

  b2Body* body_;

  bool active_;
private:
};

}

#endif //CLIENT_BODY_H
