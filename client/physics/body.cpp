#include <cstdio>
#include <cmath>

#include <Box2D/Box2D.h>

#include "client/gfx.h"
#include "client/draw.h"
#include "client/physics/body.h"
#include "client/physics/world.h"

namespace gfx
{

Body::
Body(World& world, float x, float y, float angle) :
  world_ {world}, active_ {false}
{
  world_.registerBody(this);

  // Body definition
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;

  bodyDef.position.Set(x, y);
  bodyDef.angle = angle * M_PI / 180.f;

  bodyDef.linearDamping = 0.f;
  bodyDef.angularDamping = 0.01f;

  body_ = world_.world_.CreateBody(&bodyDef);

  // Set active
  active_ = true;
}

Body::
~Body()
{
  world_.unregisterBody(this);
  if (body_)
    world_.world_.DestroyBody(body_);
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

b2Vec2 Body::
getPosition() const
{
  return body_->GetPosition();
}

void Body::
render()
{
  if (!body_)
    return;

  for (auto f = body_->GetFixtureList(); f != nullptr; f = f->GetNext())
  {
    b2Shape::Type type = f->GetType();
    if (type == b2Shape::e_circle)
    {
      auto shape = dynamic_cast<b2CircleShape*>(f->GetShape());
      auto origin = body_->GetWorldPoint(shape->m_p);
      auto radius = shape->m_radius;

      glPushMatrix();
      glTranslatef(origin.x, origin.y, 0.f);
      glRotatef(body_->GetAngle(), 0, 0, 1.f);
      drawFilledCircle(radius);
      glPopMatrix();
    }
    else if (type == b2Shape::e_polygon)
    {
      auto shape = dynamic_cast<b2PolygonShape*>(f->GetShape());
      auto n = shape->GetVertexCount();

      // TODO(naum): Change this
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
