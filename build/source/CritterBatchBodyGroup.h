#if 0
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

                                                                                       

#ifndef CRITTER_BODY_H
#define CRITTER_BODY_H

#include "CritterStable.h"
#include "CritterCommon.h"

#include "CritterBodyDescription.h"

                                                                                       

namespace Critter
{

class CritterPublicClass BatchBody : public NxOgre::Actor
{
  
 friend class BatchBodyGroup;
 template<class T> friend inline void ::NxOgre::Functions::safe_delete(T*);
  
 public:
  

 protected:
  
  BatchBody(NxOgre::ShapeDescriptions&, NxOgre::RigidBodyDescription&, NxOgre::Matrix44 globalPose, NxOgre::Scene*);
  
 ~BatchBody();
  
};

class CritterPublicClass BatchBodyGroup : public NxOgre::TimeListener
{
 
 public:
  
  friend class RenderSystem;
  template<class T> friend inline void ::NxOgre::Functions::safe_delete(T*);
  
 public:
   
   BatchBody create(const NxOgre::Matrix44& globalPose);
   
   /** \brief "Drawing" function, called by the TimeListener - Changes the SceneNode pose to the same one as the NxActor's.
*/
   bool advance(float, const NxOgre::Enums::Priority&);
   
 protected:
   
   /** \internal. Use RenderSystem::createBody
   */
   BatchBodyGroup(const NxOgre::ShapeDescription&, Ogre::MeshPtr, const NxOgre::RigidBodyDescription&, RenderSystem*);
     
   /** \internal. Use RenderSystem::createBody
*/
   BatchBodyGroup(const NxOgre::ShapeDescriptions&, Ogre::MeshPtr, const NxOgre::RigidBodyDescription&, RenderSystem*);
   
   /** \internal. Use RenderSystem::destroyBody
*/
   ~BatchBodyGroup(void);
   
 protected:
   
   RenderSystem*                  mRenderSystem;
   
   Ogre::SceneNode*               mNode;
   
   Ogre::SimpleRenderable*        mRenderable;
   
   NxOgre::ShapeDescriptions      mShapeDescriptions;
   
   NxOgre::ptr_vector<BatchBody>  mBodies;

   NxOgre::RigidBodyDescription   mRigidBodyDescription;
   
   NxOgre::MeshData*              mMeshData;
   
   void writeVertices(float*, const NxOgre::Matrix44& pose);
   void writeNormals(float*);
   void writeTextureCoords(float*);
   void writeIndexes(unsigned int*);

};

} // namespace

                                                                                       

#endif
#endif