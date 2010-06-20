/** 
    
    This file is part of Critter.
    
    Copyright (c) 2009 Robin Southern, http://www.nxogre.org
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
    
*/

#include "CritterStable.h"
#include "CritterParticleRenderable.h"
#if NxOgreHasFluids == 1
#include "CritterRenderSystem.h"

                                                                                       

namespace Critter
{

                                                                                       

ParticleRenderable::ParticleRenderable(const Ogre::String& material, RenderSystem* renderSystem)
: NxOgre::Renderable(Enums::FluidType_OgreParticle),
  mMaterialName(material),
  mRenderSystem(renderSystem),
  mParticleSystem(0)
{
}

ParticleRenderable::~ParticleRenderable()
{
 
 if (mParticleSystem)
 {
  if (mParticleSystem->isAttached())
   mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
  
  mRenderSystem->getSceneManager()->destroyParticleSystem(mParticleSystem);
  mParticleSystem = 0;
 }
 
}

void ParticleRenderable::initialise(NxOgre::Fluid* fluid)
{
 
 mFluid = fluid;
 
 mNode = mRenderSystem->getSceneManager()->getRootSceneNode()->createChildSceneNode();
 mParticleSystem = mRenderSystem->getSceneManager()->createParticleSystem(
   mRenderSystem->getUniqueName("ParticleRenderable"),
   mFluid->getMaxParticles() * 2);
 mNode->attachObject(mParticleSystem);
 
 mParticleSystem->setDefaultDimensions(0.2f, 0.2f);
 mParticleSystem->setMaterialName(mMaterialName);
 mParticleSystem->setParticleQuota(mFluid->getMaxParticles());
 mParticleSystem->setSpeedFactor(0.0f);
 
}

void ParticleRenderable::drawFluid(NxOgre::PhysXParticleData* data, const NxOgre::Bounds3& bounds)
{
 if (data->getNbParticles() > mParticleSystem->getNumParticles())
 {
   unsigned int newParticleCount = data->getNbParticles() - mParticleSystem->getNumParticles();
   for (unsigned int i=0;i < newParticleCount;i++)
   {
    Ogre::Particle* particle = mParticleSystem->createParticle();
    if (particle)
    {
     particle->timeToLive = 1.0f;
    }
   }
 }
 
 float* positions = data->getPositions();
 float* velocities = data->getVelocities();

 unsigned int i = 0;
 Ogre::ParticleIterator it = mParticleSystem->_getIterator();
 unsigned int m = data->getNbParticles() * 3;
 while(!it.end())
 {
  
  Ogre::Particle* particle = it.getNext();
  if (i < m)
  {
   particle->position.x = positions[i];
   particle->position.y = positions[i+1];
   particle->position.z = positions[i+2];
   particle->direction.x = velocities[i];
   particle->direction.y = velocities[i+1];
   particle->direction.z = velocities[i+2];
  }
  else
  {
   particle->timeToLive = 0;
  }
  
  i+=3;
  
 }
 
 if (!bounds.isEmpty())
   mParticleSystem->setBounds(Ogre::AxisAlignedBox(bounds.min.as<Ogre::Vector3>(), bounds.max.as<Ogre::Vector3>()));
 
}

Ogre::SceneNode* ParticleRenderable::getNode()
{
 return mNode;
}

RenderSystem* ParticleRenderable::getRenderSystem()
{
 return mRenderSystem;
}


Ogre::ParticleSystem* ParticleRenderable::getParticleSystem()
{
 return mParticleSystem;
}


                                                                                       

} // namespace Critter

                                                                                       

#endif
