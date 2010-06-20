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

                                                                                       

#ifndef CRITTER_BODYDESCRIPTION_H
#define CRITTER_BODYDESCRIPTION_H

#include "CritterStable.h"
#include "CritterCommon.h"

                                                                                       

namespace Critter
{

class CritterPublicClass BodyDescription : public NxOgre::RigidBodyDescription
{
  
 public:
  
  /** \brief OGRE3DPrototype constructor
  */
  BodyDescription();
  
  /** \brief OGRE3DPrototype destructor
  */
 ~BodyDescription();
  
  /** \brief Reset the RigidBodyPrototype and OGRE3D bits to it's default state.
  */
  void reset();
  
  /** \brief Does the prototype's variables are in the range of acceptable values?
  */
  bool valid();
  
  /** \brief SceneNode to use, or NULL to create.
  */
  Ogre::SceneNode* mNode;
  
  /** \brief What to do the Node when the Body is destroyed.
  \default Enums::SceneNodeDestructorBehaviour_Destroy
  */
  Enums::SceneNodeDestructorBehaviour mSceneNodeDestructorBehaviour;

  /** \brief Drawing priority
  */
  NxOgre::Enums::Priority mRenderPriority;
  
};

} // namespace

                                                                                       

#endif
