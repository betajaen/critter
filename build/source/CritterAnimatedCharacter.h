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

                                                                                       

#ifndef CRITTER_BACKGROUND_CHARACTER_H
#define CRITTER_BACKGROUND_CHARACTER_H

#include "CritterStable.h"

#if NxOgreHasCharacterController == 1

#include "CritterCommon.h"
#include "CritterCharacterBase.h"

                                                                                       

namespace Critter
{

/*! class. AnimatedCharacter
*/
class CritterPublicClass AnimatedCharacter : public CharacterBase
{
 
 public:
 
  
  friend class RenderSystem;
  
  NXOGRE_GC_FRIEND_NEW5
  NXOGRE_GC_FRIEND_DELETE
  
 public:
   
   /*! function. playAnimation
       desc.
           Play an animation for section 0 and 1, and optionally freeze all movement when playing.
   */
   void playAnimation(size_t section0, size_t section1, size_t recoveryAnim0 = Critter::Enums::StockAnimationID_Idle, size_t recoveryAnim1 = Critter::Enums::StockAnimationID_Idle, bool freezeMovementWhenPlaying = false);
   
   void setPosition(const Ogre::Vector3& position);
   
   void setPosition(const NxOgre::Vec3& position);
   
   void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
   
   Ogre::Vector3 getPosition() const;
   
   NxOgre::Vec3 getPositionAsVec3() const;
   
 protected:
   
   /** \internal. Use RenderSystem::createAnimatedCharacter
   */
   AnimatedCharacter(const AnimatedCharacterDescription&, const NxOgre::Vec3& position, Ogre::Real yaw, Node*, RenderSystem*);
   
   /** \internal. Use RenderSystem::destroyAnimatedCharacter
   */
  ~AnimatedCharacter();
   
   void advancePhysics(float deltaTime, const NxOgre::Enums::Priority&);
   
 protected:
   
   Ogre::Real     mMaxJumpVelocity, mJumpTime, mFallTime;
   NxOgre::Vec3   mAirUserDirection, mJumpVelocity0;
   size_t         mJumpDirectionIndex;
   bool           mIsJumping, mIsFalling;
   size_t         mFallDirectionIndex;
   bool           mUsesGravity;
   Ogre::Real     mMaxGroundSpeed;
   bool           mAnimWait;
   size_t         mAnimWaitNextAnims[2];
   NxOgre::Ray    mRay;
};

} // namespace

                                                                                       

#endif
#endif
