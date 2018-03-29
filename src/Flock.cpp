#include "Flock.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Mat4.h>
#include <cstdlib>

/*
TODO:

- Order of execution:
    - updateBoids() iterates through the list and calculates
      the velocity of each boid in the flock.
    - updateBoids() calls boid.move() which sets the new position
      of the boid.
    - drawBoids() constructs a matrix for the transformation of
      the boid, using the boids new position.
    - drawBoids() loads this new matrix to the shader then draws
      the boid VAO.
    - Calculate rotation matrix from difference in directions??

- Implement cohesion and separation algorithms.
- Transform the camera matrices by the tx matrices.
- Work out what this velocity is and what it should be
  initiliased at.
- Make the boid number changeable.

*/

// Global variables that will eventually be loaded by config.
float boidInfluenceRadius = 33.0f;
int numberOfBoids = 100;





//Initialise flock with view and projection matrices.
flock::flock( const ngl::Mat4 &_view , const ngl::Mat4 &_projection )
            : m_view(_view), m_projection(_projection)
{
  for (int i = 0; i < numberOfBoids; i++)
  {
    boid b(i);
    m_boids.push_back(b);
  }
}




// For each boid
void flock::updateBoids()
{
  for( auto & b : m_boids )
  {
    ngl::Vec3 boidVelocity = b.getVelocity();
    boidVelocity += flockingBehaviour( b );
    //boidVelocity += ngl::Vec3(0.0f, 0.0f, -1.0f);
    boidVelocity.normalize();
    b.move(boidVelocity);
  }
}



//SOMETHING IN HERE IS CALLING BOID DESTRUCTORS
//AND I DON'T KNOW WHAT OR WHY, BUT IT'S AN ISSUE.

ngl::Vec3 flock::flockingBehaviour( const boid currentBoid )
{
  unsigned int neighbourCount = 0;
  ngl::Vec3 outVectorAlignment;
  ngl::Vec3 outVectorCohesion;
  ngl::Vec3 outVectorSeparation;

  for( auto const & b : m_boids ) //m_boids is a vector of boids
  {
    if( b.getID() != currentBoid.getID() )
    {

      float distance = (b.getPosition() - currentBoid.getPosition()).length();

      if( distance < boidInfluenceRadius )
      {
        outVectorAlignment  += b.getVelocity();
        outVectorCohesion   += b.getPosition();
        outVectorSeparation += b.getPosition() - currentBoid.getPosition();
        neighbourCount++;
      }
    }
  }

  if( neighbourCount == 0 ) return currentBoid.getVelocity();

  outVectorAlignment  /= float(neighbourCount);
  outVectorCohesion   /= float(neighbourCount);
  outVectorCohesion   -= currentBoid.getPosition();
  outVectorSeparation /= float(neighbourCount);
  outVectorSeparation *= -1;

  return outVectorAlignment + outVectorCohesion + outVectorSeparation;
}





// Build the boid VAO in the flock so when drawing we can bind
// once :)
void flock::buildVAO()
{
  std::array<ngl::Vec3,18> verts=
  {{
    //Back left
    ngl::Vec3(-0.5,0,0.5),
    ngl::Vec3(0,0.25,0.25),
    ngl::Vec3(0,-0.25,0.25),
    //Back right
    ngl::Vec3(0,0.25,0.25),
    ngl::Vec3(0.5,0,0.5),
    ngl::Vec3(0,-0.25,0.25),
    //Front top left
    ngl::Vec3(-0.5,0,0.5),
    ngl::Vec3(0,0.0,-1.0),
    ngl::Vec3(0,0.25,0.25),
    //Front top right
    ngl::Vec3(0,0.25,0.25),
    ngl::Vec3(0,0,-1),
    ngl::Vec3(0.5,0,0.5),
    //Front bottom left
    ngl::Vec3(0,-0.25,0.25),
    ngl::Vec3(0,0,-1),
    ngl::Vec3(-0.5,0,0.5),
    //Front bottom right
    ngl::Vec3(0,0,-1),
    ngl::Vec3(0.5,0,0.5),
    ngl::Vec3(0,-0.25,0.25)
  }};
  std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
  std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
  // create a vao as a series of GL_TRIANGLES
  m_boidShape.reset(ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_TRIANGLES) );
  m_boidShape->bind();

  // in this case we are going to set our data as the vertices above
  m_boidShape->setData(ngl::SimpleVAO::VertexData(verts.size()*sizeof(ngl::Vec3),verts[0].m_x));
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

  m_boidShape->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

  // divide by 2 as we have both verts and normals
  m_boidShape->setNumIndices(verts.size());

 // now unbind
  m_boidShape->unbind();
}





/*ngl::Vec3 flock::cohesion( boid currentBoid )
{
  //Start neighbour count at 0.
  unsigned int neighbourCount = 0;
  ngl::Vec3 outVector;

  for( auto & b : m_boids )
  {
    if( b.getID() != currentBoid.getID() )
    {
      float distance = (b.getPosition() - currentBoid.getPosition()).length();

      if( distance < boidInfluenceRadius )
      {
        outVector += b.getPosition();
        neighbourCount++;
      }
    }
  }

  if( neighbourCount == 0 )
    return outVector;

  outVector /= float(neighbourCount);
  outVector -= currentBoid.getPosition();
  outVector.normalize();
  return outVector;

}*/

/*ngl::Vec3 flock::separation( boid currentBoid )
{
  //Start neighbour count at 0.
  unsigned int neighbourCount = 0;
  ngl::Vec3 outVector;

  for( auto & b : m_boids )
  {
    if( b.getID() != currentBoid.getID() )
    {
      float distance = (b.getPosition() - currentBoid.getPosition()).length();

      if( distance < boidInfluenceRadius )
      {
        outVector += b.getPosition() - currentBoid.getPosition();
        neighbourCount++;
      }
    }
  }

  if( neighbourCount == 0 )
    return outVector;

  outVector /= float(neighbourCount);
  outVector -= currentBoid.getPosition();
  outVector *= -1;
  outVector.normalize();
  return outVector;
}*/
