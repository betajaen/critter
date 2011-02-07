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
#include "CritterRenderSystem.h"
#include "CritterBody.h"
#include "CritterBodyDescription.h"
#include "CritterParticleRenderable.h"
#include "CritterRenderable.h"
#include "CritterKinematicBody.h"
#include "CritterNode.h"

                                                                                       

unsigned int Critter::RenderSystem::mUniqueIdentifier = 0;

                                                                                       

namespace Critter
{

RenderSystem::RenderSystem(NxOgre::Scene* scene, Ogre::SceneManager* sceneMgr)
: mScene(scene), mVisualDebuggerRenderable(0), mVisualDebuggerNode(0), mSceneManager(sceneMgr)
{
 mScene->addRenderListener(this, NxOgre::Enums::Priority_MediumLow);
}

RenderSystem::~RenderSystem()
{
 
 mScene->removeRenderListener(this, NxOgre::Enums::Priority_MediumLow);
 
 if (mVisualDebuggerRenderable)
 {
  mVisualDebuggerNode->detachObject(mVisualDebuggerRenderable);
  mSceneManager->getRootSceneNode()->removeAndDestroyChild(mVisualDebuggerNode->getName());
  destroyRenderable(mVisualDebuggerRenderable);
 }
 
 mBodies.remove_all();
 mRenderables.remove_all();
 
#if NxOgreHasCharacterController == 1
 mAnimatedCharacters.remove_all();
#endif
 
}

NxOgre::Scene* RenderSystem::getScene()
{
 return mScene;
}

Body* RenderSystem::createBody(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, const Ogre::String& meshName, BodyDescription& description)
{
 
 if (description.mNode == 0)
  description.mNode = NxOgre::GC::safe_new2<Node>(mSceneManager, this, NXOGRE_GC_THIS);
 
 description.mNode->setPose(pose);
 
 if (meshName.length())
  description.mNode->createAndAttachEntity(meshName);
 
 Body* body = NxOgre::GC::safe_new4<Body>(shape, pose, description, this, NXOGRE_GC_THIS);
 
 mBodies.insert(body->getNameHash(), body);

 return body;
}


Body* RenderSystem::createBody(const NxOgre::ShapeDescriptions& shapes, const NxOgre::Matrix44& pose, const Ogre::String& meshName, BodyDescription& description)
{
 
 if (description.mNode == 0)
  description.mNode = NxOgre::GC::safe_new2<Node>(mSceneManager, this, NXOGRE_GC_THIS);
 
 description.mNode->setPose(pose);
 
 if (meshName.length())
  description.mNode->createAndAttachEntity(meshName);
 
 Body* body = NxOgre::GC::safe_new4<Body>(shapes, pose, description, this, NXOGRE_GC_THIS);
 
 mBodies.insert(body->getNameHash(), body);

 return body;
}

Body* RenderSystem::createBody(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, BodyDescription& description)
{
 if (description.mNode == 0)
  description.mNode = NxOgre::GC::safe_new2<Node>(mSceneManager, this, NXOGRE_GC_THIS);
 
 description.mNode->setPose(pose);
 
 Body* body = NxOgre::GC::safe_new4<Body>(shape, pose, description, this, NXOGRE_GC_THIS);
 mBodies.insert(body->getNameHash(), body);

 return body;
}


Body* RenderSystem::createBody(const NxOgre::ShapeDescriptions& shapes, const NxOgre::Matrix44& pose, BodyDescription& description)
{
 if (description.mNode == 0)
  description.mNode = NxOgre::GC::safe_new2<Node>(mSceneManager, this, NXOGRE_GC_THIS);
 
 Body* body = NxOgre::GC::safe_new4<Body>(shapes, pose, description, this, NXOGRE_GC_THIS);
 mBodies.insert(body->getNameHash(), body);

 return body;
}

void RenderSystem::destroyBody(Body* body)
{
 if (body == 0 || body->getRigidBodyType() != Enums::RigidBodyType_Body)
  return;
 
 mBodies.remove(body->getNameHash(), body);
}

RenderSystem::BodyIterator RenderSystem::getBodies()
{
 return mBodies.elements();
}

Renderable* RenderSystem::createRenderable(int type, const Ogre::String& materialName)
{
 Renderable* renderable = NxOgre::GC::safe_new1<Renderable>(type, NXOGRE_GC_THIS);
 renderable->setMaterial(materialName);
 mRenderables.push_back(renderable);
 return renderable;
}

void RenderSystem::destroyRenderable(Renderable* renderable)
{
 if (renderable == 0)
  return;
 mRenderables.remove(mRenderables.index(renderable));
}

KinematicBody* RenderSystem::createKinematicBody(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, const Ogre::String& meshName,BodyDescription& description)
{
 
 if (description.mNode == 0)
  description.mNode = NxOgre::GC::safe_new2<Node>(mSceneManager, this, NXOGRE_GC_THIS);
 
 description.mNode->setPose(pose);

 if (meshName.length())
  description.mNode->createAndAttachEntity(meshName);
 
 KinematicBody* kb = NxOgre::GC::safe_new4<KinematicBody>(shape, pose, description, this, NXOGRE_GC_THIS);
 
 mKinematicBodies.insert(kb->getNameHash(), kb);

 return kb;
}

KinematicBody* RenderSystem::createKinematicBody(const NxOgre::ShapeDescriptions& shapes, const NxOgre::Matrix44& pose, const Ogre::String& meshName,BodyDescription& description)
{
 
 if (description.mNode == 0)
  description.mNode = NxOgre::GC::safe_new2<Node>(mSceneManager, this, NXOGRE_GC_THIS);
 
 description.mNode->setPose(pose);
 
 if (meshName.length())
  description.mNode->createAndAttachEntity(meshName);
 
 KinematicBody* kb = NxOgre::GC::safe_new4<KinematicBody>(shapes, pose, description, this, NXOGRE_GC_THIS);
 
 mKinematicBodies.insert(kb->getNameHash(), kb);

 return kb;
}

void RenderSystem::destroyKinematicBody(KinematicBody* kinematicBody)
{
 if (kinematicBody == 0 || kinematicBody->getRigidBodyType() != Enums::RigidBodyType_KinematicBody)
  return;
 
 mKinematicBodies.remove(kinematicBody->getNameHash(), kinematicBody);
 
}

bool RenderSystem::advance(float, const NxOgre::Enums::Priority&, const NxOgre::Enums::SceneFunction& func)
{
 if (mVisualDebuggerRenderable && func == NxOgre::Enums::SceneFunction_Render)
 {
  ::NxOgre::World::getWorld()->getVisualDebugger()->draw();
  mVisualDebuggerNode->needUpdate();
 }
 return true;
}

NxOgre::VisualDebugger* RenderSystem::createVisualDebugger(const NxOgre::VisualDebuggerDescription& desc)
{
 
 if (Ogre::MaterialManager::getSingletonPtr()->resourceExists("Critter.VisualDebuggerMaterial") == false)
 {
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->create("Critter.VisualDebuggerMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  material->getTechnique(0)->getPass(0)->setDepthBias(1);
  material->getTechnique(0)->getPass(0)->setAmbient(1,1,1);
  material->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,1);
  material->getTechnique(0)->getPass(0)->setDiffuse(1,1,1,1);
  material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
 }
 
 if (mVisualDebuggerRenderable == 0)
 {
  mVisualDebuggerRenderable = createRenderable(NxOgre::Enums::RenderableType_VisualDebugger, "Critter.VisualDebuggerMaterial");
  ::NxOgre::World::getWorld()->getVisualDebugger()->setRenderable(mVisualDebuggerRenderable);
  mVisualDebuggerRenderable->setCastShadows(false);
  mVisualDebuggerNode = mSceneManager->getRootSceneNode()->createChildSceneNode("Critter.VisualDebuggerNode");
  mVisualDebuggerNode->attachObject(mVisualDebuggerRenderable);
 }
 
 mVisualDebuggerNode->setVisible(true);
 NxOgre::VisualDebugger* vdebugger = NxOgre::World::getWorld()->getVisualDebugger();
 vdebugger->loadFromDescription(desc);
 vdebugger->enable();
 return vdebugger;
}

void RenderSystem::destroyVisualDebugger()
{
 if (mVisualDebuggerRenderable == 0)
  return;

 NxOgre::World::getWorld()->getVisualDebugger()->disable();
 mVisualDebuggerNode->detachAllObjects();
 destroyRenderable(mVisualDebuggerRenderable);
 mVisualDebuggerRenderable = 0;
 mSceneManager->getRootSceneNode()->removeAndDestroyChild(mVisualDebuggerNode->getName());
}

bool RenderSystem::hasDebugVisualisation() const
{
 return mVisualDebuggerRenderable != 0;
}

Ogre::SceneManager* RenderSystem::getSceneManager()
{
 return mSceneManager;
}

Ogre::String RenderSystem::getUniqueName(const Ogre::String& prefix) const
{
 return prefix + "-" + Ogre::StringConverter::toString(mUniqueIdentifier++);
}

NxOgre::Cloth* RenderSystem::createCloth(const NxOgre::ClothDescription& description, const Ogre::String& materialName)
{
 Renderable* renderable = createRenderable(NxOgre::Enums::RenderableType_PhysXMesh);
 mSceneManager->getRootSceneNode()->attachObject(renderable);
 renderable->setMaterial(materialName);
 return mScene->createCloth(description, renderable, NxOgre::Enums::Priority_MediumLow);
}

void RenderSystem::destroyCloth(NxOgre::Cloth* cloth)
{
 if (cloth == 0)
  return;

 Renderable* renderable = static_cast<Renderable*>(cloth->getRenderable());

 mSceneManager->getRootSceneNode()->detachObject(renderable);
 destroyRenderable(renderable);
 mScene->destroyCloth(cloth);
}

NxOgre::SoftBody* RenderSystem::createSoftBody(const NxOgre::SoftBodyDescription& description, const Ogre::String& materialName)
{
 Renderable* renderable = createRenderable(NxOgre::Enums::RenderableType_SoftBody);
 mSceneManager->getRootSceneNode()->attachObject(renderable);
 renderable->setMaterial(materialName);
 return mScene->createSoftBody(description, renderable, NxOgre::Enums::Priority_MediumLow);
}

void RenderSystem::destroySoftBody(NxOgre::SoftBody* cloth)
{
 if (cloth == 0)
  return;

 Renderable* renderable = static_cast<Renderable*>(cloth->getRenderable());

 mSceneManager->getRootSceneNode()->detachObject(renderable);
 destroyRenderable(renderable);
 mScene->destroySoftBody(cloth);
}


NxOgre::Fluid* RenderSystem::createFluid(const NxOgre::FluidDescription& description, const Ogre::String& materialName, Enums::FluidRenderableType renderable_type)
{

 NxOgre::Fluid* fluid;

 if (renderable_type == Enums::FluidType_OgreParticle)
 {
  ParticleRenderable* renderable = new ParticleRenderable(materialName, this);
  fluid = mScene->createFluid(description, renderable);
  renderable->initialise(fluid);
 }
 else
 {
  Renderable* renderable = createRenderable(renderable_type);
  mSceneManager->getRootSceneNode()->attachObject(renderable);
  renderable->setMaterial(materialName);
  fluid = mScene->createFluid(description, renderable);
 }
 
 return fluid;
}

void RenderSystem::destroyFluid(NxOgre::Fluid* fluid)
{
 
 if (fluid == 0)
  return;
 
 NxOgre::Renderable* renderable = fluid->getRenderable();
 
 if (renderable->getType() == Enums::FluidType_OgreParticle)
 {
  delete renderable;
 }
 else
 {
  Renderable* ogre_renderable = static_cast<Renderable*>(renderable);
  mSceneManager->getRootSceneNode()->detachObject(ogre_renderable);
  destroyRenderable(ogre_renderable);
 }
 
 mScene->destroyFluid(fluid);
 
}

Ogre::SceneNode* RenderSystem::createSceneNodeEntityPair(const Ogre::String& meshName, const Ogre::Vector3& vec, const Ogre::Quaternion& quat)
{
 Ogre::SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode(vec, quat);
 node->attachObject(mSceneManager->createEntity(meshName));
 return node;
}

Ogre::SceneNode* RenderSystem::createSceneNodeEntityPair(const NxOgre::String& meshName, const NxOgre::Vec3& vec, const NxOgre::Quat& quat)
{
 Ogre::SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode(vec.as<Ogre::Vector3>(), quat.as<Ogre::Quaternion>());
 node->attachObject(mSceneManager->createEntity(meshName));
 return node;
}

#if Critter_UsesOgreTerrain == 1
NxOgre::SceneGeometry* RenderSystem::createTerrain(Ogre::Terrain* terrain)
{
 
 NxOgre::HeightFieldData data;
 data.mNbColumns = terrain->getSize();
 data.mNbRows = terrain->getSize();
 
 const float normMin = -32768.0f;
 const float normMax = 32767.0f;
 const float minHeight = terrain->getMinHeight();
 const float maxHeight = terrain->getMaxHeight();
 const float heightDiff = maxHeight - minHeight;
 const float normDiff = normMax - normMin;
 const unsigned short size = terrain->getSize();

 float* ter_data = terrain->getHeightData();
 
 NxOgre::HeightFieldSample sample;
 
 sample.mMaterial0 = 0;
 sample.mMaterial1 = 0;
 sample.mTessellationFlag = NxOgre::Enums::HeightFieldTesselation_NW_SE;
 
 NxOgre_DebugPrint_HeightFields("Attempting to sample data from heightfield, size = " << size);
 
 data.mSamples.resize(size * size);
 
 for(int x = 0; x < size; ++x)
 {
  sample.mTessellationFlag = NxOgre::Enums::HeightFieldTesselation_NW_SE;
  for(int z = size-1; z >= 0; --z)
  {
   float height = ter_data[(size * z) + x];
   sample.mHeight = (short) ( ( (height - minHeight) / heightDiff) * normDiff + normMin);
   data.mSamples.push_back(sample);
   sample.mTessellationFlag = !sample.mTessellationFlag;
  }
 }
 
 NxOgre_DebugPrint_HeightFields("Attempting to cook heightfield from Ogre terrain");
 NxOgre::HeightField* hf = data.cookQuickly();
 
 const float hf_size = float(terrain->getWorldSize()) + (float(terrain->getWorldSize()) / float(size));
 const float hf_height = (maxHeight - minHeight) / 2.0f;
 NxOgre::Vec3 pose(terrain->getPosition());
 pose.x -= float(size) / 2.0f;
 pose.y += (maxHeight + minHeight) / 2.0f;
 pose.z -= float(size) / 2.0f;
 
 NxOgre::HeightFieldGeometryDescription hf_desc;
 hf_desc.mHeightField = hf;
 hf_desc.mLocalPose.set(pose);
 hf_desc.mDimensions.set(hf_size, hf_height, hf_size);


 NxOgre_DebugPrint_HeightFields("Attempting to create scene geometry from Ogre terrain");

 return mScene->createSceneGeometry(hf_desc);
}
#endif

void RenderSystem::addAnimation(const Ogre::String& mesh_name, size_t section, size_t index, const Ogre::String& animation_name, Ogre::Real fade_speed, bool loops)
{
 Animation anim;
 anim.mLoops = loops;
 anim.mName = animation_name;
 anim.mFadeSpeed = fade_speed;
 addAnimation(mesh_name, section, index, anim);
}

void RenderSystem::addAnimation(const Ogre::String& mesh_name, size_t section, size_t index, const Animation& anim)
{
 
 AnimationProperties* anims = 0;
 size_t hashed_name = NxOgre::Strings::hash(mesh_name);
 
 if (mAnimations[section].has(hashed_name) == false)
 {
  anims = new AnimationProperties();
  mAnimations[section].insert(hashed_name, anims);
 }
 else
  anims = mAnimations[section].at(hashed_name);
 
 Animation* an = 0;
 bool isNew = false;
 
 if (anims->has(index))
 {
  an = anims->at(index);
 }
 else
 {
  an = new Animation();
  
  anims->insert(index, an);
 }
 
 an->mFadeSpeed = anim.mFadeSpeed;
 an->mLoops = anim.mLoops;
 an->mName = anim.mName;
 
}

void RenderSystem::removeAnimation(const Ogre::String& mesh_name, size_t section, size_t index)
{
 
}

Animation* RenderSystem::getAnimation(const Ogre::String& mesh_name, size_t section, size_t index) const
{
 return 0;
}

void RenderSystem::getAnimations(const Ogre::String& mesh_name, size_t section, Ogre::Entity* entity, AnimationStates& states)
{
 AnimationProperties* anims = 0;
 size_t hashed_name = NxOgre::Strings::hash(mesh_name);
 if (mAnimations[section].has(hashed_name) == false)
  return;
 
 anims = mAnimations[section].at(hashed_name);
 
 for (AnimationPropertyIterator it = anims->elements();it != it.end(); it++)
 {
  AnimationState state;
  state.mState = entity->getAnimationState( (*it)->mName );
  state.mAnimation = (*it);
  state.mFadeIn = false;
  state.mFadeOut = false;
  
  states.insert(it.hashed_key(), state);
 }
 
}

Node* RenderSystem::createNode()
{
 return new Node(mSceneManager, this);
}

void RenderSystem::destroyNode(Node* n)
{
 if (n)
  delete n;
}

#if NxOgreHasCharacterController == 1

AnimatedCharacter* RenderSystem::createAnimatedCharacter(const Ogre::Vector3& position, const Ogre::Radian& yaw, const Ogre::String& mesh_name, const AnimatedCharacterDescription& desc)
{
 Node* node = createNode();
 node->createAndAttachEntity(mesh_name);
 return createAnimatedCharacter(position, yaw, node, desc);
}

AnimatedCharacter* RenderSystem::createAnimatedCharacter(const Ogre::Vector3& position, const Ogre::Radian& yaw, Node* node, const AnimatedCharacterDescription& desc)
{
 AnimatedCharacter* character = NxOgre::GC::safe_new5<AnimatedCharacter>(desc, position, yaw.valueRadians(), node, this, NXOGRE_GC_THIS);
 mAnimatedCharacters.push_back(character);
 return character;
}

void RenderSystem::destroyAnimatedCharacter(AnimatedCharacter* character)
{
 if (character == 0)
  return;
 mAnimatedCharacters.remove(mAnimatedCharacters.index(character));
}

#endif

                                                                                       

} // namespace Critter
