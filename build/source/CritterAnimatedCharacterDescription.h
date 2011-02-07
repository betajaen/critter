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

/*! class. AnimatedCharacter
*/
class CritterPublicClass AnimatedCharacterDescription
{
  
 public:
  
  /** \brief OGRE3DPrototype constructor
  */
  AnimatedCharacterDescription();
  
  /** \brief OGRE3DPrototype destructor
  */
 ~AnimatedCharacterDescription();
  
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
  NxOgre::Real mSlopeLimit;
  
  /*! variable. mStepOffset
      desc.
          Maximum height that the character can climb.
      default.
          0.25
  */
  NxOgre::Real mStepOffset;
  
  /*! variable. mMaxGroundSpeed
      desc.
          Maximum speed the character can travel via user input when on ground.
      default.
          4.0
  */
  NxOgre::Real mMaxGroundSpeed;
  
  /*! variable. mJumpVelocity
      desc.
          Initial velocity of the character when jumping.
      default.
          4.425 m/s (about 1m peak height, and about 0.9 seconds of flight time).
  */
  NxOgre::Real mJumpVelocity;
  
  void  setJumpVelocityFromMaxHeight(NxOgre::Real gravity, NxOgre::Real maxHeight)
  {
   // sqrt(h*g/2) * 2
   mJumpVelocity = NxOgre::Math::sqrt( (NxOgre::Math::abs(maxHeight) * NxOgre::Math::abs(gravity)) * 0.5f) * 2.0f;
   std::cout << " --> Result = " << mJumpVelocity << "\n";
  }
  
  void  setJumpVelocityFromMaxTime(NxOgre::Real gravity, NxOgre::Real maxTime)
  {
   // h = 1/2 gt^2
   NxOgre::Real h = 0.5f * NxOgre::Math::abs(gravity) * (maxTime * maxTime);
   // v = (h*sqrt(g/2h))*2
   mJumpVelocity = ( h * NxOgre::Math::sqrt( NxOgre::Math::abs(gravity) / (2 * h) ) ) * 2;
  }

  /*! variable. mUpDirection
      desc.
          What is "up" to the character?
      default.
          NxOgre::Enums::Y
  */
  NxOgre::Enums::Axis mUpDirection;
};

} // namespace Critter

                                                                                       

#endif
#endif
