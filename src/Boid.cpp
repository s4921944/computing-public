#include "Boid.h"
#include <iostream>
#include <cstdlib>


boid::boid( int _id )
{
  m_id = _id;
  std::cout<<"Assigning boid ID "<<_id<<'\n';
  //Random distribution
  m_position = ngl::Vec3(rand()%10, rand()%10, rand()%10);
  m_velocity = ngl::Vec3(rand()%10, rand()%10, rand()%10);
}

boid::~boid()
{
  std::cout<<"Deleting boid "<<m_id<<'\n';
}

void boid::draw()
{

}

//Used to set boids velocity and facing direction.
void boid::move( ngl::Vec3 _velocity )
{
    m_position += _velocity;
    looping();
}

void boid::looping()
{
    if( m_position.m_x > 150 )
        m_position.m_x = -150;
    if( m_position.m_x < -150 )
        m_position.m_x = 150;
    if( m_position.m_y > 150 )
        m_position.m_y = -150;
    if( m_position.m_y < -150 )
        m_position.m_y = 150;
    if( m_position.m_z > 150 )
        m_position.m_z = -150;
    if( m_position.m_z < -150 )
        m_position.m_z = 150;
}

int boid::getID() const
{
  return m_id;
}

ngl::Vec3 boid::getPosition() const
{
  return m_position;
}

ngl::Vec3 boid::getVelocity() const
{
  return m_velocity;
}

float boid::getSpeed() const
{
  return m_speed;
}
