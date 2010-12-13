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

                                                                                       

#ifndef CRITTER_BACKGROUND_CHARACTER_DESCRIPTION_H
#define CRITTER_BACKGROUND_CHARACTER_DESCRIPTION_H

#include "CritterStable.h"

#if NxOgreHasCharacterController == 1

#include "CritterCommon.h"

                                                                                       

namespace Critter
{

/*! class. BackgroundCharacter
*/
class CritterPublicClass BackgroundCharacterDescription
{
  
 public:
  
  /** \brief OGRE3DPrototype constructor
  */
  BackgroundCharacterDescription();
  
  /** \brief OGRE3DPrototype destructor
  */
 ~BackgroundCharacterDescription();
  
  /** \brief Reset the RigidBodyPrototype and OGRE3D bits to it's default state.
  */
  void reset();
  
  /** \brief Does the prototype's variables are in the range of acceptable values?
  */
  bool valid();
  
  /*! variable. mRenderPriority
      desc.
          Drawing priority
  */
  NxOgre::Enums::Priority mPriority;
  
  /*! variable. mShape
      desc.
          Primary Shape.
  */
  NxOgre::SimpleCapsule mShape;
  
  /*! variable. mAlternateShape
      desc.
          Alternate Shape.
  */
  NxOgre::SimpleCapsule mAlternateShape;
  
  /*! variable. mAlternateShape
      desc.
          Alternate Shape.
  */
  bool mIsUsingAlternateShape;
  
  /*! variable. mCollisionMask
      desc.
          Masked used for collisions.
  */
  unsigned int mCollisionMask;
  
  /*! variable. mSlopeLimit
      desc.
          Maximum angle (in radians) of a slope that the character can climb.
      default.
          0.707 (~40 degrees)
  */
  Ogre::Real mSlopeLimit;
  
  /*! variable. mStepOffset
      desc.
          Maximum height that the character can climb.
      default.
          0.25
  */
  Ogre::Real mStepOffset;
  
  /*! variable. mMaxGroundSpeed
      desc.
          Maximum speed the character can travel via user input when on ground.
      default.
          4.0
  */
  Ogre::Real mMaxGroundSpeed;
  
};

} // namespace Critter

                                                                                       

#endif
#endif
