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
#include "CritterNode.h"
#include "CritterRenderSystem.h"
#include "CritterAnimationState.h"

size_t Critter::Node::sUniqueID = 0;

                                                                                       

namespace Critter
{

                                                                                       

Node::Node(Ogre::SceneManager* scene_mgr, RenderSystem* renderSys)
: mSceneMgr(scene_mgr), mRenderSystem(renderSys)
{
 mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
}

Node::Node(Ogre::SceneManager* scene_mgr, Ogre::SceneNode* parent, RenderSystem* renderSys)
: mSceneMgr(scene_mgr), mRenderSystem(renderSys)
{
 mNode = parent->createChildSceneNode();
}

Node::~Node()
{
 if (mNodeBehaviour == Enums::SceneNodeDestructorBehaviour_Destroy)
  destroyNode(mNode);
 mNode = 0;
}

void Node::destroyNode(Ogre::SceneNode* node)
{
 
 if (node == 0)
  return;
 
 Ogre::SceneNode::ObjectIterator object_it = node->getAttachedObjectIterator();
 Ogre::MovableObject *m;
 
 while(object_it.hasMoreElements())
 {
  m = object_it.getNext();

  // Move Cameras to RootSceneNodes
  if (m->getMovableType() == "Camera")
  {
   node->detachObject(m);
   mSceneMgr->getRootSceneNode()->attachObject(m);
  }
  // Everything else gets destroyed.
  else
   mSceneMgr->destroyMovableObject(m);
 }
 
 node->detachAllObjects();
 node->removeAndDestroyAllChildren();
 
 mSceneMgr->destroySceneNode(node);
}

Ogre::Entity* Node::fetchEntity(const Ogre::String& name)
{
 Ogre::Entity* entity = NULL;
 
 if (Ogre::StringUtil::endsWith(name, ".mesh")) // Is it a mesh?
 {
  std::stringstream new_name;
  new_name << "critter_entity_" << sUniqueID++;
  entity = mSceneMgr->createEntity(new_name.str(), name);
 }
 else if (mSceneMgr->hasEntity(name)) // Could it be an entity?
 {
  entity = mSceneMgr->getEntity(name);
  
  if (entity->isAttached())
   entity->getParentSceneNode()->detachObject(entity);
 }
 else // It must be a mesh.
 {
  std::stringstream new_name;
  new_name << "critter_entity_" << sUniqueID++;
  entity = mSceneMgr->createEntity(new_name.str(),  name);
 }
 
 return entity;
}

void Node::parseMovableObject(Ogre::MovableObject* mo)
{
 if (mo == 0)
  return;
 
 if (mo->isAttached())
  mo->getParentSceneNode()->detachObject(mo);
}

Ogre::Entity* Node::getEntityAt(unsigned short index)
{
 return static_cast<Ogre::Entity*>(mNode->getAttachedObject(index));
}

Ogre::Camera* Node::getCameraAt(unsigned short index)
{
 return static_cast<Ogre::Camera*>(mNode->getAttachedObject(index));
}

Ogre::ParticleSystem* Node::getParticleSystemAt(unsigned short index)
{
 return static_cast<Ogre::ParticleSystem*>(mNode->getAttachedObject(index));
}

Ogre::Light* Node::getLightAt(unsigned short index)
{
 return static_cast<Ogre::Light*>(mNode->getAttachedObject(index));
}

Ogre::MovableObject* Node::getAt(unsigned short index)
{
 return mNode->getAttachedObject(index);
}

unsigned short Node::numAttachedObjects() const
{
 return mNode->numAttachedObjects();
}

unsigned short Node::numChildren() const
{
 return mNode->numChildren();
}

void Node::setPosition(const Ogre::Vector3& position)
{
 mNode->setPosition(position);
}

Ogre::SceneNode* Node::createChildSceneNode()
{
 return mNode->createChildSceneNode();
}

void Node::createAndAttachEntity(const Ogre::String& mesh_name)
{
 addEntity(mSceneMgr->createEntity(mesh_name));
 _setupAnimations();
}

void Node::createAndAttachEntity(const Ogre::String& entity_name, const Ogre::String& mesh_name)
{
 addEntity(mSceneMgr->createEntity(entity_name, mesh_name));
 _setupAnimations();
}

void Node::setPose(const NxOgre::Vec3& position, const NxOgre::Quat& orientation)
{
 mNode->setPosition(position.as<Ogre::Vector3>());
 mNode->setOrientation(orientation.as<Ogre::Quaternion>());
}

void Node::setPose(const Ogre::Matrix4& m)
{
 mNode->setPosition(m.getTrans());
 mNode->setOrientation(m.extractQuaternion());
}

void Node::setPose(const NxOgre::Matrix44& m)
{
 mNode->setPosition(NxOgre::Vec3(m).as<Ogre::Vector3>());
 mNode->setOrientation(NxOgre::Vec3(m).as<Ogre::Quaternion>());
}

void Node::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
 mNode->setPosition(x,y,z);
}

void Node::setPosition(const NxOgre::Vec3& position)
{
 mNode->setPosition(position.as<Ogre::Vector3>());
}


Ogre::Vector3 Node::getPosition() const
{
 return mNode->getPosition();
}

NxOgre::Vec3 Node::getPositionVec3() const
{
 return NxOgre::Vec3(mNode->getPosition());
}

void Node::setOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
 mNode->setOrientation(w,x,y,z);
}

void Node::setOrientation(const Ogre::Quaternion& orientation)
{
 mNode->setOrientation(orientation);
}

void Node::setOrientation(const NxOgre::Quat& orientation)
{
 mNode->setOrientation(orientation.as<Ogre::Quaternion>());
}

void Node::setOrientation(Ogre::Radian angle, NxOgre::Enums::Axis axis)
{
 if (axis == NxOgre::Enums::X)
  mNode->setOrientation(Ogre::Quaternion(angle, Ogre::Vector3::UNIT_X));
 else if (axis == NxOgre::Enums::Y)
  mNode->setOrientation(Ogre::Quaternion(angle, Ogre::Vector3::UNIT_Y));
 else if (axis == NxOgre::Enums::Z)
  mNode->setOrientation(Ogre::Quaternion(angle, Ogre::Vector3::UNIT_Z));
}

void Node::setOrientation(Ogre::Radian angle, const Ogre::Vector3& axis )
{
 mNode->setOrientation(Ogre::Quaternion(angle, axis));
}

Ogre::Quaternion Node::getOrientation() const
{
 return mNode->getOrientation();
}

NxOgre::Quat Node::getOrientationQuat() const
{
 return NxOgre::Quat(mNode->getOrientation());
}

void Node::addSceneNode(Ogre::SceneNode* node)
{
 if (node == 0)
  return;
 Ogre::SceneNode* pnode = node->getParentSceneNode();
  
 if (pnode)
  pnode->removeChild(node);
  
 mNode->addChild(node);
}

void Node::addSceneNode(const Ogre::String& name)
{
  
  if (mSceneMgr->hasSceneNode(name) == false)
   return;
  
  Ogre::SceneNode* node = mSceneMgr->getSceneNode(name);
  Ogre::SceneNode* pnode = node->getParentSceneNode();
  
  if (pnode)
   pnode->removeChild(node);
  
  mNode->addChild(node);
}

void Node::addEntity(Ogre::Entity* entity)
{
 parseMovableObject(entity);
 mNode->attachObject(entity);
}

void Node::addMovableObject(Ogre::MovableObject* mo)
{
 parseMovableObject(mo);
 mNode->attachObject(mo);
}

void Node::removeSceneNode(Ogre::SceneNode* node)
{
if (node == 0)
  return;

 Ogre::SceneNode* pnode = node->getParentSceneNode();

 if (pnode == 0)
  return;

 if (pnode == mNode)
  pnode->removeChild(node);
}

void Node::removeSceneNode(const Ogre::String& name)
{
 if (mSceneMgr->hasSceneNode(name) == false)
  return;

 Ogre::SceneNode* node = mSceneMgr->getSceneNode(name);
 Ogre::SceneNode* pnode = node->getParentSceneNode();

 if (pnode == 0)
  return;

 if (pnode == mNode)
  pnode->removeChild(node);
}

void Node::removeEntity(Ogre::Entity* entity)
{
 removeMovableObject(entity);
}

void Node::removeEntity(const Ogre::String& name)
{
 removeMovableObject(name);
}

void Node::destroyEntity(Ogre::Entity* entity)
{
 destroyMovableObject(entity);
}

void Node::destroyEntity(const Ogre::String& name)
{
 destroyMovableObject(name);
}

void Node::removeMovableObject(Ogre::MovableObject* mo)
{
 if (mo == 0)
  return;
 
 mNode->detachObject(mo);
}

void Node::removeMovableObject(const Ogre::String& name)
{
 mNode->detachObject(name);
}

void Node::destroyMovableObject(Ogre::MovableObject* mo)
{
 removeMovableObject(mo);
 mSceneMgr->destroyMovableObject(mo);
}

void Node::destroyMovableObject(const Ogre::String& name)
{
 Ogre::MovableObject* mo = mNode->getAttachedObject(name);
 destroyMovableObject(mo);
}

Ogre::SceneNode* Node::getSceneNode() const
{
 return mNode;
}

Ogre::SceneManager* Node::getSceneManager() const
{
 return mSceneMgr;
}

void Node::setScale(Ogre::Real m)
{
 mNode->setScale(m,m,m);
}

void Node::setScale(const Ogre::Vector3& scale)
{
 mNode->setScale(scale);
}

void Node::_setupAnimations(size_t switchTo)
{
 
 Ogre::Entity* entity = getEntityAt(0);
 if (entity->hasSkeleton() == false)
  return;
 
 entity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
 Ogre::String meshName = entity->getMesh()->getName();
 
 for (size_t i=0;i < CRITTER_MAX_ANIMATION_SECTIONS;i++)
 {
  mAnimations[i].remove_all();
  mCurrentAnimation[i] = Enums::NO_ANIMATION;
  mCurrentAnimationSpeed[i] = 1.0f;
  mRenderSystem->getAnimations(meshName, i, entity, mAnimations[i]);
 }
 
 if (switchTo != Enums::NO_ANIMATION)
 {
  for (size_t i=0;i < CRITTER_MAX_ANIMATION_SECTIONS;i++)
   setAnimation(i, switchTo);
 }
}

void Node::updateAnimations(Ogre::Real deltaTime)
{
 for (size_t i=0;i < CRITTER_MAX_ANIMATION_SECTIONS;i++)
 {
  size_t index = mCurrentAnimation[i];
  if (index == Enums::NO_ANIMATION)
   continue;
  
  if (mAnimations[i].has(index) == false)
  {
   mCurrentAnimation[i] = Enums::NO_ANIMATION;
   continue;
  }
  
  mAnimations[ i ][ index ].mState->addTime(deltaTime * mCurrentAnimationSpeed[i]);
 }

 fadeAnimations(deltaTime);
}

void Node::fadeAnimations(Ogre::Real deltaTime)
{
 AnimationState* state = 0;
 for (size_t i=0; i < CRITTER_MAX_ANIMATION_SECTIONS;i++)
 {
  for (mAnimationIterator = mAnimations[i].elements(); mAnimationIterator != mAnimationIterator.end(); mAnimationIterator++)
  {
   
   state = &(*mAnimationIterator);
   
   if (state->mFadeIn)
   {
    Ogre::Real w = state->mState->getWeight() + (state->mAnimation->mFadeSpeed * deltaTime * mCurrentAnimationSpeed[i]);
    if (w >= 1.0f)
    {
     w = 1.0f;
     state->mFadeIn = false;
    }
    state->mState->setWeight(w);
   } // if
   
   if (state->mFadeOut)
   {
    Ogre::Real w = state->mState->getWeight() - (state->mAnimation->mFadeSpeed * deltaTime * mCurrentAnimationSpeed[i]);
    if (w <= 0.0f)
    {
     w = 0.0f;
     state->mFadeOut = false;
    }
    state->mState->setWeight(w);
    if (state->mFadeOut == false)
     state->mState->setEnabled(false);
   } // if
   
  } // for
 } // for
}

void Node::setAnimation(size_t section, size_t index, bool reset)
{
 AnimationState* state = 0;
 if (mCurrentAnimation[section] != Enums::NO_ANIMATION)
 {
  if (mAnimations[section].has(mCurrentAnimation[section]))
  {
   state = &mAnimations[section].at(mCurrentAnimation[section]);
   state->mFadeIn = false;
   state->mFadeOut = true;
  } // if
 } // if
 
 mCurrentAnimation[section] = index;
 
 if (index != Enums::NO_ANIMATION)
 {
  if (mAnimations[section].has(mCurrentAnimation[section]))
  {
   state = &mAnimations[section].at(mCurrentAnimation[section]);
   state->mState->setEnabled(true);
   state->mState->setWeight(0.0f);
   state->mState->setLoop(state->mAnimation->mLoops);
   state->mFadeIn = true;
   state->mFadeOut = false;
   if (reset)
    state->mState->setTimePosition(0.0f);
  } // if
 } // if
 
}

size_t Node::getCurrentAnimation(size_t section) const
{
 return mCurrentAnimation[section];
}

Enums::AnimationStatus Node::getAnimationStatus(size_t section, size_t index) const
{
 
 const AnimationState* state = 0;
 if (mAnimations[section].has(mCurrentAnimation[section]) == false)
  return Enums::AnimationStatus_Disabled;
 
 state = &mAnimations[section].at(index);
 
 if (state->mFadeIn)
  return Enums::AnimationStatus_FadeIn;
 else if (state->mFadeOut)
  return Enums::AnimationStatus_FadeOut;
 else if (state->mState->getEnabled())
  return Enums::AnimationStatus_Enabled;
 return Enums::AnimationStatus_Disabled;
 
}

Ogre::Real Node::getCurrentAnimationSpeed(size_t section) const
{
 return mCurrentAnimationSpeed[section];
}

void Node::setCurrentAnimationSpeed(size_t section, Ogre::Real speed)
{
 mCurrentAnimationSpeed[section] = speed;
}

bool Node::getCurrentAnimationEnded(size_t section) const
{
 if (mCurrentAnimation[section] == Enums::NO_ANIMATION)
  return false;
 return mAnimations[section].at(mCurrentAnimation[section]).mState->hasEnded();
}

bool Node::getCurrentAnimationLoops(size_t section) const
{
 if (mCurrentAnimation[section] == Enums::NO_ANIMATION)
  return false;
 return mAnimations[section].at(mCurrentAnimation[section]).mState->getLoop();
}


                                                                                       

} 

                                                                                       

