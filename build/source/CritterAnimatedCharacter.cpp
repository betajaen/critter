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

#if NxOgreHasCharacterController == 1

#include "CritterAnimatedCharacter.h"
#include "CritterAnimatedCharacterDescription.h"
#include "CritterNode.h"

                                                                                       

namespace Critter
{

                                                                                       

AnimatedCharacter::AnimatedCharacter(const AnimatedCharacterDescription& desc, const NxOgre::Vec3& position, Ogre::Real yaw, Node* node, RenderSystem* renderSys)
: CharacterBase(node, renderSys)
{
 
 mYaw = yaw;
 mShape = desc.mShape;
 mAlternateShape = desc.mAlternateShape;
 mIsUsingAlternateShape = desc.mIsUsingAlternateShape;
 
 NxOgre::CharacterControllerDescription c_desc;
 c_desc.mCallback = this;
 c_desc.mCapsuleEasyClimbing = false;
 c_desc.mStepOffset = desc.mStepOffset;
 c_desc.mSlopeLimit = desc.mSlopeLimit;
 c_desc.mUpDirection = desc.mUpDirection;
 
 mActiveGroups = desc.mCollisionMask;
 mIsJumping = false;
 mJumpTime = 0;
 mMaxJumpVelocity = desc.mJumpVelocity;
 mJumpVelocity0.zero();
 mJumpDirectionIndex = 0;
 mUsesGravity = true;
 mMaxGroundSpeed = desc.mMaxGroundSpeed;
 mIsFalling = false;
 mFallDirectionIndex = 0;
 mAnimWait = false;
 mAnimWaitNextAnims[0] = Enums::NO_ANIMATION;
 mAnimWaitNextAnims[1] = Enums::NO_ANIMATION;
 createCharacterController(position, mShape, c_desc);
 
 mNode->setAnimation(0, Enums::StockAnimationID_Idle);
 mNode->setAnimation(1, Enums::StockAnimationID_Idle);
 
 mRay.mDirection.y = -1;
}

AnimatedCharacter::~AnimatedCharacter()
{
}

void AnimatedCharacter::setPosition(const Ogre::Vector3& position)
{
 CharacterController::setPosition(position);
}

void AnimatedCharacter::setPosition(const NxOgre::Vec3& position)
{
 CharacterController::setPosition(position);
}

void AnimatedCharacter::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
 CharacterController::setPosition(NxOgre::Vec3(x,y,z));
}

Ogre::Vector3 AnimatedCharacter::getPosition() const
{
 return CharacterController::getPosition().as<Ogre::Vector3>();
}

NxOgre::Vec3 AnimatedCharacter::getPositionAsVec3() const
{
 return CharacterController::getPosition();
}

void AnimatedCharacter::playAnimation(size_t section0, size_t section1, size_t recoveryAnim0, size_t recoveryAnim1, bool freezeMovementWhenPlaying)
{
 
 mAnimWait = freezeMovementWhenPlaying;
 
 if (section0 != Enums::NO_ANIMATION)
 {
  if (section0 != mNode->getCurrentAnimation(0))
  {
   mNode->setAnimation(0, section0, true);
   mAnimWaitNextAnims[0] = recoveryAnim0;
  }
 }
 
 if (section1 != Enums::NO_ANIMATION)
 {
  if (section0 != mNode->getCurrentAnimation(1))
  {
   mNode->setAnimation(1, section1, true);
   mAnimWaitNextAnims[1] = recoveryAnim1;
  }
 }
 
}

void AnimatedCharacter::advancePhysics(float deltaTime, const NxOgre::Enums::Priority&)
{
 
 // Wait for any animation to finish, then switch to a new one.
 if (mAnimWait)
 {
  if (mNode->getCurrentAnimation(0) != Enums::NO_ANIMATION)
  {
    if (mNode->getCurrentAnimationEnded(0) == false && mNode->getCurrentAnimationLoops(0) == false)
    {
     return;
    }
  }
  if (mNode->getCurrentAnimation(1) != Enums::NO_ANIMATION)
  {
    if (mNode->getCurrentAnimationEnded(1) == false && mNode->getCurrentAnimationLoops(1) == false)
    {
     return;
    }
  }
  mAnimWait = false;
  mNode->setAnimation(0, mAnimWaitNextAnims[0]);
  mNode->setAnimation(1, mAnimWaitNextAnims[1]);
 }
 
 bool hasUserMovement = mInput.left_right != 0 || mInput.forward_backward != 0;
 bool hasUserJumpMovement = mInput.up == 1;
 
 NxOgre::Vec3 displacement;
 
 // Check for falling.
 if ( (!hasCollidedDown() && !hasPreviouslyCollidedDown()) && mUsesGravity && !mIsJumping)
 {
  if (mIsFalling == false)
  {
   mIsFalling = true;
   mFallTime = 0;
   playAnimation(Enums::StockAnimationID_Fall, Enums::StockAnimationID_Fall);
   // V = gt = 0
  }
  else
  {
   mFallTime += deltaTime;
   // V = gt
   displacement = mScene->getGravity() * mFallTime;
   displacement += mAirUserDirection;
   move(displacement * deltaTime);
  }
  return;
 }
 
 if (mIsFalling)
 {
  mIsFalling = false;
  mAirUserDirection.zero();
  playAnimation(Enums::StockAnimationID_Land, Enums::StockAnimationID_Land, Enums::StockAnimationID_Idle,Enums::StockAnimationID_Idle, true);
  return;
 }
 
 // Fall if gravity is enabled.
 if (mUsesGravity)
  displacement += mScene->getGravity();
 
 if (mIsJumping)
 {
  mJumpTime += deltaTime;
  
  // v = v0 - gt
  displacement = mJumpVelocity0 - (-mScene->getGravity()) * mJumpTime;
  
  // if v <= 0 then now is falling, mIsJumping is false.
  if (displacement[mJumpDirectionIndex] <= 0.0f)
  {
   mIsJumping = false;
   mIsFalling = true;
   mFallTime = 0;
   playAnimation(Enums::StockAnimationID_Fall, Enums::StockAnimationID_Fall);
   return;
  }
  
  displacement += mAirUserDirection;
  
  move(displacement * deltaTime);
  return;
 }
 else if (hasUserJumpMovement && mIsJumping == false)
 {
  mIsJumping = true;
  mJumpTime = 0;
  mJumpVelocity0 = mScene->getGravity().used() * mMaxJumpVelocity;
  mJumpDirectionIndex = mJumpVelocity0.axis();
  mAirUserDirection.x = float(mInput.left_right) * ReciprocalOf127 * mMaxGroundSpeed;
  mAirUserDirection.z = float(mInput.forward_backward) * ReciprocalOf127 * mMaxGroundSpeed;
  playAnimation(Enums::StockAnimationID_Jump, Enums::StockAnimationID_Jump, Enums::StockAnimationID_Fall, Enums::StockAnimationID_Fall);
  return;
 }

 // Switch to idle pose if no user movement has been requested and forward animation is playing.
 if (hasUserMovement == false)
 {
  playAnimation(Enums::StockAnimationID_Idle, Enums::StockAnimationID_Idle);
 }
 // Switch to forward pose (if idle), calculate new displacement vector from user movement.
 else
 {
  playAnimation(Enums::StockAnimationID_Forward, Enums::StockAnimationID_Forward);
  
  static  Ogre::Vector3 userDirection;
  userDirection = Ogre::Vector3::ZERO;
  
  if (mInput.is_turning && mInput.left_right != 0)
  {
   // y += v * 0.013 * dt
   mYaw += Ogre::Radian(float(mInput.left_right) * 0.013 * deltaTime); // temp.
   userDirection.x = 0;
  }
  else if (mInput.is_turning == false)
  {
   // dS_x = v * 127^-1 * v_max
   userDirection.x = float(mInput.left_right) * ReciprocalOf127 * mMaxGroundSpeed;
  }
  
  // dS_z = v * 127^-1 * v_max
  userDirection.z = float(mInput.forward_backward) * ReciprocalOf127 * mMaxGroundSpeed;
    
  userDirection = Ogre::Quaternion(mYaw, Ogre::Vector3::UNIT_Y) * userDirection;

  
  displacement.x += userDirection.x;
  displacement.z += userDirection.z;

 }
 
 
 // sS = S * dT
 move(displacement * deltaTime);
 
 
}

                                                                                       

}

                                                                                       

#endif