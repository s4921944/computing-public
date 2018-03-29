#ifndef BOID_H_
#define BOID_H_

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <memory>
#include <ngl/VAOPrimitives.h>

class boid
{

public:
  boid( int _id );
  ~boid();

  void move( ngl::Vec3 );
  void looping();
  void draw();

  int getID() const;
  ngl::Vec3 getPosition() const;
  ngl::Vec3 getVelocity() const;
  float     getSpeed() const;

private:
  int m_id;
  ngl::Vec3 m_position;
  ngl::Vec3 m_velocity;
  float m_speed;

};

#endif
