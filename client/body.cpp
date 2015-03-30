#include <cstdio>
#include <cmath>

#include "client/defines.h"
#include "client/body.h"
#include "client/world.h"

#include <Box2D/Box2D.h>

#include "client/gfx.h"

namespace client
{

Body::
Body(World& world, float x, float y, float angle) : world_ {&world}, active_ {false}
{
  world_->registerBody(this);

  // Body definition
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;

  bodyDef.position.Set(x, y);
  bodyDef.angle = angle;

  bodyDef.linearDamping = 0.f;
  bodyDef.angularDamping = 0.01f;

  body_ = world_->world_.CreateBody(&bodyDef);

  // Set active
  active_ = true;
}

Body::
~Body()
{
  world_->unregisterBody(this);
  if (body_)
    world_->world_.DestroyBody(body_);
}

void Body::
setStatic(bool isStatic)
{
  body_->SetType(isStatic ? b2_staticBody : b2_dynamicBody);
}

void Body::
addShape(const b2Shape* shape)
{
  body_->CreateFixture(shape, 0.f);
}

void Body::
addFixture(const b2Shape* shape,
                float density,
                float friction,
                float restitution)
{
  b2FixtureDef fixtureDef;
  fixtureDef.shape = shape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;
  fixtureDef.restitution = restitution;
  body_->CreateFixture(&fixtureDef);
}

void Body::
render()
{
  if (!body_)
    return;

  for (auto f = body_->GetFixtureList(); f->GetBody() != nullptr; ++f)
  {
    b2Shape::Type type = f->GetType();
    if (type == b2Shape::e_circle)
    {
      auto shape = dynamic_cast<b2CircleShape*>(f->GetShape());
      auto pos = body_->GetPosition();

      glBegin(GL_TRIANGLE_FAN);

      auto origin = shape->m_p;
      auto radius = shape->m_radius;
      glVertex3f(origin.x + pos.x, origin.y + pos.y, 0.f);

      for (int i = 0; i < CIRCLE_DIVISIONS; ++i)
      {
        glVertex3f(
          origin.x + radius * cosf(i * 2 * M_PI / CIRCLE_DIVISIONS) + pos.x,
          origin.y + radius * sinf(i * 2 * M_PI / CIRCLE_DIVISIONS) + pos.y,
          0.f
        );
      }
      glVertex3f(origin.x + radius + pos.x, origin.y + pos.y, 0.f);
      glEnd();
    }
    else if (type == b2Shape::e_polygon)
    {
      auto shape = dynamic_cast<b2PolygonShape*>(f->GetShape());
      auto n = shape->GetVertexCount();

      glBegin(GL_POLYGON);
      b2Vec2 vertex;
      for (int i = 0; i < n; ++i)
      {
        vertex = body_->GetWorldPoint(shape->GetVertex(i));
        glVertex3f(vertex.x, vertex.y, 0.f);
      }
      glEnd();
    }
  }
}

}
