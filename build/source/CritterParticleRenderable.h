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


                                                                                       

#ifndef CRITTER_PARTICLERENDERABLE_H
#define CRITTER_PARTICLERENDERABLE_H

#include "CritterStable.h"
#if NxOgreHasFluids == 1
#include "CritterCommon.h"

                                                                                       

namespace Critter
{

class CritterPublicClass ParticleRenderable : public NxOgre::UserExtendedClassAllocatable, public NxOgre::Renderable
{
  
  friend class RenderSystem;
    
  public:
    
  public:
     
     void  drawSoftBodySimple(NxOgre::MeshRenderable*, const NxOgre::Bounds3&) {}
     void  drawCloth(NxOgre::MeshRenderable*, const NxOgre::Bounds3&) {}
     void  drawClothFast(NxOgre::MeshRenderable*, const NxOgre::Bounds3&) {}
     void  drawVisualDebugger(NxOgre::VisualDebuggerMeshData*) {}
     
     /** \brief Text
     */
     void                                      drawFluid(NxOgre::PhysXParticleData*, const NxOgre::Bounds3&);
     
     /** \brief Get the Node.
     */
     Ogre::SceneNode*                          getNode();
     
     /** \brief Get the SceneManager used.
     */
     RenderSystem*                       getRenderSystem();

     /** \brief
     */
     Ogre::ParticleSystem*                     getParticleSystem();

     /** \brief
     */
     ParticleRenderable(const Ogre::String& material, RenderSystem*);
     
     /** \brief
     */
    ~ParticleRenderable();
     
  protected:
     
     void                   initialise(NxOgre::Fluid* fluid);
     
  protected:
     
     /** \brief SceneNode appointed to this OGRE3DPointRenderable.
     */
     Ogre::SceneNode*       mNode;
     
     /** \brief RenderSystem
     */
     RenderSystem*    mRenderSystem;
     
     /** \brief
     */
     Ogre::ParticleSystem*  mParticleSystem;
     
     /** \brief
     */
     Ogre::String           mMaterialName;
     
     /** \brief
     */
     NxOgre::Fluid*         mFluid;
     
};



} // namespace Critter

                                                                                       

#endif

#endif