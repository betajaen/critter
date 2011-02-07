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

#include "CritterCharacterBase.h"
#include "CritterRenderSystem.h"
#include "CritterNode.h"

                                                                                       

namespace Critter
{

                                                                                       

CharacterBase::CharacterBase(Node* node, RenderSystem* renderSystem)
: CharacterController(renderSystem->getScene()), mNode(node), mRenderSystem(renderSystem), mYaw(0), mUpDirection(NxOgre::Enums::Y)
{
 mScene->addSimulateListener(this, mPriority);
 mScene->addRenderListener(this, mPriority);
}

CharacterBase::~CharacterBase()
{
 mScene->removeRenderListener(this, mPriority);
 mScene->removeSimulateListener(this, mPriority);
 delete mNode;
}

Enums::CharacterType CharacterBase::getType() const
{
 return Enums::CharacterType_Base;
}

bool CharacterBase::advance(float deltaTime, const NxOgre::Enums::Priority& priority, const NxOgre::Enums::SceneFunction& func)
{
 if (func == NxOgre::Enums::SceneFunction_Simulate)
  advancePhysics(deltaTime, priority);
 else
  advanceAnimation(deltaTime, priority);
 return true;
}

void CharacterBase::advancePhysics(float deltaTime, const NxOgre::Enums::Priority&)
{
 if (hasCollidedDown() == false)
  move(mScene->getGravity());
}

void CharacterBase::advanceAnimation(float deltaTime, const NxOgre::Enums::Priority&)
{
 mNode->setPosition(getPosition());
 mNode->setOrientation(mYaw, mUpDirection);
 mNode->updateAnimations(deltaTime);
}

void CharacterBase::setInput(const CharacterInput& input)
{
 mInput = input;
}
   
void CharacterBase::setInput(const CharacterInputHelper& helper)
{
 mInput = helper.input;
}

CharacterInput CharacterBase::getInput() const
{
 return mInput;
}

                                                                                       

}

                                                                                       

#endif