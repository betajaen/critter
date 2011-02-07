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

                                                                                       

#ifndef CRITTER_CHARACTERBASE_H
#define CRITTER_CHARACTERBASE_H

#include "CritterStable.h"

#if NxOgreHasCharacterController == 1

#include "CritterCommon.h"
#include "CritterCharacterInput.h"

                                                                                       

namespace Critter
{

/*! class. Body
*/
class CritterPublicClass CharacterBase : protected NxOgre::CharacterController, protected NxOgre::TimeListener, protected NxOgre::Callback
{
 
 public:
 
  
  friend class RenderSystem;
  
  NXOGRE_GC_FRIEND_NEW4
  NXOGRE_GC_FRIEND_DELETE
  
 public:
   
   void setInput(const CharacterInput&);
   
   void setInput(const CharacterInputHelper&);

   CharacterInput getInput() const;

 protected:
  
  CharacterBase(Node*, RenderSystem*);
  
  virtual ~CharacterBase();
  
  virtual void advancePhysics(float deltaTime, const NxOgre::Enums::Priority&);
  
  virtual void advanceAnimation(float deltaTime, const NxOgre::Enums::Priority&);
  
  virtual Enums::CharacterType getType() const;

 protected:
  
  bool advance(float deltaTime, const NxOgre::Enums::Priority&, const NxOgre::Enums::SceneFunction&);
  
  RenderSystem*            mRenderSystem;
  Critter::Node*           mNode;
  NxOgre::Enums::Priority  mPriority;
  Ogre::Radian             mYaw;
  NxOgre::SimpleCapsule    mShape, mAlternateShape;
  bool                     mIsUsingAlternateShape;
  CharacterInput           mInput;
  NxOgre::Enums::Axis      mUpDirection;
};

} // namespace

                                                                                       

#endif
#endif
