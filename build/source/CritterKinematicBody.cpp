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
#include "CritterKinematicBody.h"
#include "CritterRenderSystem.h"
#include "CritterBodyDescription.h"

                                                                                       

unsigned int Critter::KinematicBody::mNextBodyID = 0;

namespace Critter
{

                                                                                       

KinematicBody::KinematicBody(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, const BodyDescription& description, RenderSystem* rendersystem)
: KinematicActor(rendersystem->getScene()),
                                   // Take notice of the constructor we are using, it's designed for
                                   // classes that inherit from Actor. 
 mNode(0),
 mSceneManager(0),
 mRenderPriority(description.mRenderPriority), 
 mSceneNodeDestructorBehaviour(description.mSceneNodeDestructorBehaviour)
{
 // Implement the prototype (it's being casted back into a RigidBodyPrototype) so it's treated
 // as a normal RigidBody. 
 
 createKinematic(pose, description, shape);
 
 // Since NxOgre doesn't know or care about our Ogre stuff, we copy it over. This is the correct time to create
 // or turn on things related to the KinematicBody.
 
 mSceneManager = rendersystem->getSceneManager();
 
 mNode = description.mNode;
 
 // And let the Scene know we want this renderered. So it will call the advance function when it's 
 // time to render.
 mScene->addRenderListener(this, mRenderPriority);
}

KinematicBody::KinematicBody(const NxOgre::ShapeDescriptions& shapes, const NxOgre::Matrix44& pose, const BodyDescription& description, RenderSystem* rendersystem)
: KinematicActor(rendersystem->getScene()),
                                   // Take notice of the constructor we are using, it's designed for
                                   // classes that inherit from Actor. 
 mNode(0),
 mSceneManager(0),
 mRenderPriority(description.mRenderPriority), 
 mSceneNodeDestructorBehaviour(description.mSceneNodeDestructorBehaviour)
{
 // Implement the prototype (it's being casted back into a RigidBodyPrototype) so it's treated
 // as a normal RigidBody. 
 
 createKinematic(pose, description, shapes);
 
 // Since NxOgre doesn't know or care about our Ogre stuff, we copy it over. This is the correct time to create
 // or turn on things related to the KinematicBody.
 
 mSceneManager = rendersystem->getSceneManager();
 
 mNode = description.mNode;
 
 // And let the Scene know we want this renderered. So it will call the advance function when it's 
 // time to render.
 mScene->addRenderListener(this, mRenderPriority);
}

KinematicBody::~KinematicBody()
{
 
 // Stop NxOgre calling the advance function in the future, otherwise bad things would happen.
 mScene->removeRenderListener(this, mRenderPriority);
 
 // In here, we would clean up any rendering stuff.
 _destructNode(mSceneNodeDestructorBehaviour);
 
 // We leave the physics stuff to the Actors destructor, including freeing the shapes.
}

void KinematicBody::_destructNode(Enums::SceneNodeDestructorBehaviour behaviour)
{
 
 if (behaviour == Enums::SceneNodeDestructorBehaviour_Inherit)
  behaviour = mSceneNodeDestructorBehaviour;
 
 if (behaviour == Enums::SceneNodeDestructorBehaviour_Destroy)
 {
  // Remove all attachments.
  if (mNode->numAttachedObjects())
   mNode->detachAllObjects();
  
  // Destroy all child scenenodes.
  if (mNode->numChildren())
   mNode->removeAndDestroyAllChildren();
  
  // Destroy this Scene node.
  mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());
  mNode = 0;
 }
 else
 {
  mNode->getParentSceneNode()->removeChild(mNode);
  mNode = 0;
 }
 
}

unsigned int KinematicBody::getRigidBodyType() const
{
 return Enums::RigidBodyType_KinematicBody;
}

Ogre::SceneManager* KinematicBody::getSceneManager()
{
 return mSceneManager;
}

Ogre::SceneNode* KinematicBody::getSceneNode()
{
 return mNode;
}

bool KinematicBody::advance(float, const NxOgre::Enums::Priority&, const NxOgre::Enums::SceneFunction&)
{
 mNode->setPosition( getGlobalPosition().as<Ogre::Vector3>() );
 mNode->setOrientation( getGlobalOrientationQuat().as<Ogre::Quaternion>() );
 return true;
}



                                                                                       

} // namespace

                                                                                       

