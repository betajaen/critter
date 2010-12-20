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
#include "CritterBody.h"
#include "CritterRenderSystem.h"
#include "CritterBodyDescription.h"
#include "CritterNode.h"

                                                                                       

namespace Critter
{

                                                                                       

Body::Body(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, const BodyDescription& description, RenderSystem* rendersystem)
: Actor(rendersystem->getScene()), // Take notice of the constructor we are using, it's designed for
                                   // classes that inherit from Actor. 
 mRenderPriority(description.mRenderPriority),
 mRenderSystem(rendersystem)
{
 // Set the name and hash.
 mName = description.mName;
 mNameHash = NxOgre::Strings::hash(mName);
 
 // Create a dynamic RigidBody with the pose, description and shape.
 // We can pass on the BodyDescription as a RigidBodyDescription because it inherits from it,
 createDynamic(pose, description, shape);

 // Obviously NxOgre won't know about the Ogre bits, so this is what the next line is for:
 mNode = description.mNode;

 // And let the Scene know we want this renderered. So it will call the advance function when it's 
 // time to render.
 mScene->addRenderListener(this, mRenderPriority);
}

Body::Body(const NxOgre::ShapeDescriptions& shapes, const NxOgre::Matrix44& pose, const BodyDescription& description, RenderSystem* rendersystem)
: Actor(rendersystem->getScene()), // Take notice of the constructor we are using, it's designed for
                                   // classes that inherit from Actor. 
 mRenderPriority(description.mRenderPriority),
 mRenderSystem(rendersystem)
{
 // Set the name and hash.
 mName = description.mName;
 mNameHash = NxOgre::Strings::hash(mName);
 
 // Create a dynamic RigidBody with the pose, description and shapes.
 // We can pass on the BodyDescription as a RigidBodyDescription because it inherits from it,
 createDynamic(pose, description, shapes);
 mAlphaPose = pose;
 
 // Obviously NxOgre won't know about the Ogre bits, so this is what the next line is for:
 mNode = description.mNode;
 
 // And let the Scene know we want this renderered. So it will call the advance function when it's 
 // time to render.
 mScene->addRenderListener(this, mRenderPriority);
}

Body::~Body()
{
 // Stop NxOgre calling the advance function in the future, otherwise bad things would happen.
 mScene->removeRenderListener(this, mRenderPriority);

 // In here, we would clean up any rendering stuff, and things that Actor couldn't possiblty know about.
 delete mNode;
 
 // As we inherit from Actor, it's destructor will be called now. Including the "destroy()" function which
 // cleans up the Physics.
}

unsigned int Body::getRigidBodyType() const
{
 return Enums::RigidBodyType_Body;
}

Node* Body::getNode() const
{
 return mNode;
}

bool Body::advance(float deltaTime, const NxOgre::Enums::Priority&, const NxOgre::Enums::SceneFunction& func)
{
 if (func == NxOgre::Enums::SceneFunction_Render)
 {
  mNode->setPosition( getGlobalPosition() );
  mNode->setOrientation( getGlobalOrientationQuat() );
  mNode->updateAnimations(deltaTime);
 }
 return true;
}

                                                                                       

}

                                                                                       

