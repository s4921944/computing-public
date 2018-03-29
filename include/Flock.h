#ifndef FLOCK_H_
#define FLOCK_H_

#include <vector>
#include <memory>
#include "Boid.h"
#include <ngl/VAOPrimitives.h>

class flock
{

public:
  flock(const ngl::Mat4 &_view, const ngl::Mat4 &_projection);
  void buildVAO();
  void move();
  void updateBoids();
  std::vector<boid> m_boids;
  std::unique_ptr<ngl::AbstractVAO> m_boidShape;

private:
  const ngl::Mat4 &m_view;
  const ngl::Mat4 &m_projection;
  ngl::Vec3 flockingBehaviour(const boid );

};

#endif
