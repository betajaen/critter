
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
#include "CritterBatchBodyGroup.h"

#if 0
#include "CritterRenderSystem.h"
#include "CritterMeshFunctions.h"

                                                                                       

namespace Critter
{

                                                                                       

BatchBodyGroup::BatchBodyGroup(const NxOgre::ShapeDescription& shape, Ogre::MeshPtr mesh, const NxOgre::RigidBodyDescription& description, RenderSystem* rendersystem)
: mRenderSystem(rendersystem), mMeshData(0)
{
 description.copy_into(&mRigidBodyDescription);
 mMeshData = Critter::MeshFunctions::read(mesh);
 mShapeDescriptions.push_back(shape.duplicate());
 mNode = mRenderSystem->getSceneManager()->createSceneNode();
}

BatchBodyGroup::BatchBodyGroup(const NxOgre::ShapeDescriptions& shapes, Ogre::MeshPtr mesh, const NxOgre::RigidBodyDescription& description, RenderSystem* rendersystem)
: mRenderSystem(rendersystem), mMeshData(0)
{
 description.copy_into(&mRigidBodyDescription);
 mMeshData = Critter::MeshFunctions::read(mesh);
 for (NxOgre::ShapeDescriptions::const_iterator shape = shapes.begin(); shape != shapes.end();++shape)
  mShapeDescriptions.push_back((*shape)->duplicate());
 mNode = mRenderSystem->getSceneManager()->createSceneNode();
}

BatchBodyGroup::~BatchBodyGroup(void)
{
 
 mNode->detachAllObjects();
 mNode->getParentSceneNode()->removeChild(mNode);
 mRenderSystem->getSceneManager()->destroySceneNode(mNode);
 
 NxOgre::Vectors::safe_erase(mShapeDescriptions);
 NXOGRE_DELETE_NXOGRE(mMeshData);
}

BatchBody::BatchBody(NxOgre::ShapeDescriptions& shapes, NxOgre::RigidBodyDescription& description, NxOgre::Matrix44 globalPose, NxOgre::Scene* scene)
: NxOgre::Actor(scene)
{
 createDynamic(globalPose, description, scene, shapes);
}

BatchBody::~BatchBody()
{ // Actor destructor will destroy the RigidBody
}

                                                                                       

}

                                                                                       

#endif