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

#include "CritterRenderSystem.h"
#include "CritterBody.h"
#include "CritterBodyDescription.h"
#include "CritterPointRenderable.h"
#include "CritterParticleRenderable.h"
#include "CritterRenderable.h"
#include "CritterKinematicBody.h"

                                                                                       

unsigned int Critter::RenderSystem::mUniqueIdentifier = 0;

                                                                                       

namespace Critter
{

RenderSystem::RenderSystem(NxOgre::Scene* scene, Ogre::SceneManager* sceneMgr)
: mScene(scene), mVisualDebuggerRenderable(0), mVisualDebuggerNode(0), mVisualDebuggerShown(false), mSceneManager(sceneMgr)
{

 ::NxOgre::TimeController::getSingleton()->addTimeListener(this, ::NxOgre::Enums::Priority_MediumLow);
}

RenderSystem::~RenderSystem(void)
{
 
 ::NxOgre::TimeController::getSingleton()->removeTimeListener(this, ::NxOgre::Enums::Priority_MediumLow);
 
 if (mVisualDebuggerRenderable)
 {
  mVisualDebuggerNode->detachObject(mVisualDebuggerRenderable);
  mSceneManager->getRootSceneNode()->removeAndDestroyChild(mVisualDebuggerNode->getName());
  destroyRenderable(mVisualDebuggerRenderable);
 }
 
 mBodies.clear();
 mRenderables.clear();
 mPointRenderables.clear();
 
}

NxOgre::Scene* RenderSystem::getScene(void)
{
 return mScene;
}

Body* RenderSystem::createBody(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, const Ogre::String& meshName, BodyDescription& description)
{
 description.mNode = mSceneManager->getRootSceneNode()->createChildSceneNode(NxOgre::Vec3(pose).as<Ogre::Vector3>(), NxOgre::Quat(pose).as<Ogre::Quaternion>());
 if (meshName.length())
  description.mNode->attachObject(mSceneManager->createEntity(getUniqueName("entity"), meshName));
 
 Body* body = NXOGRE_NEW_NXOGRE Body(shape, pose, description, this);
 
 mBodies.insert(body->getNameHash(), body);

 return body;
}


Body* RenderSystem::createBody(const NxOgre::ShapeDescriptions& shapes, const NxOgre::Matrix44& pose, const Ogre::String& meshName, BodyDescription& description)
{
 description.mNode = mSceneManager->getRootSceneNode()->createChildSceneNode(NxOgre::Vec3(pose).as<Ogre::Vector3>(), NxOgre::Quat(pose).as<Ogre::Quaternion>());
 if (meshName.length())
  description.mNode->attachObject(mSceneManager->createEntity(getUniqueName("entity"), meshName));
 
 Body* body = NXOGRE_NEW_NXOGRE Body(shapes, pose, description, this);
 
 mBodies.insert(body->getNameHash(), body);

 return body;
}

Body* RenderSystem::createBody(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, BodyDescription& description)
{

 Body* body = NXOGRE_NEW_NXOGRE Body(shape, pose, description, this);
 mBodies.insert(body->getNameHash(), body);

 return body;
}


Body* RenderSystem::createBody(const NxOgre::ShapeDescriptions& shapes, const NxOgre::Matrix44& pose, BodyDescription& description)
{

 Body* body = NXOGRE_NEW_NXOGRE Body(shapes, pose, description, this);
 mBodies.insert(body->getNameHash(), body);

 return body;
}

void RenderSystem::destroyBody(Body* body)
{
 if (body == 0 || body->getRigidBodyType() != Enums::RigidBodyType_Body)
  return;
 
 mBodies.erase(body->getNameHash());
}

RenderSystem::BodyIterator RenderSystem::getBodies()
{
 return BodyIterator(mBodies);
}

Renderable* RenderSystem::createRenderable(int type, const Ogre::String& materialName)
{
 Renderable* renderable = NXOGRE_NEW Renderable(type);
 renderable->setMaterial(materialName);
 mRenderables.push_back(renderable);
 return renderable;
}

void RenderSystem::destroyRenderable(Renderable* renderable)
{
 if (renderable == 0)
  return;
 mRenderables.erase(renderable);
}

KinematicBody* RenderSystem::createKinematicBody(const NxOgre::ShapeDescription& shape, const NxOgre::Matrix44& pose, const Ogre::String& meshName,BodyDescription& description)
{
 description.mNode = mSceneManager->getRootSceneNode()->createChildSceneNode(NxOgre::Vec3(pose).as<Ogre::Vector3>(), NxOgre::Quat(pose).as<Ogre::Quaternion>());
 if (meshName.length())
  description.mNode->attachObject(mSceneManager->createEntity(getUniqueName("entity"), meshName));
 
 KinematicBody* kb = NXOGRE_NEW_NXOGRE KinematicBody(shape, pose, description, this);
 
 mKinematicBodies.insert(kb->getNameHash(), kb);

 return kb;
}

void RenderSystem::destroyKinematicBody(KinematicBody* kinematicBody)
{
 if (kinematicBody == 0 || kinematicBody->getRigidBodyType() != Enums::RigidBodyType_KinematicBody)
  return;
 
 mKinematicBodies.erase(kinematicBody->getNameHash());
 
}

bool RenderSystem::advance(float, const NxOgre::Enums::Priority&)
{
 if (mVisualDebuggerRenderable && mVisualDebuggerShown)
 {
  ::NxOgre::World::getWorld()->getVisualDebugger()->draw();
  mVisualDebuggerNode->needUpdate();
 }
 return true;
}

void RenderSystem::setVisualisationMode(NxOgre::Enums::VisualDebugger type)
{

 if (Ogre::MaterialManager::getSingletonPtr()->resourceExists("RenderSystem.VisualDebugger") == false)
 {
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->create("RenderSystem.VisualDebugger", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  material->getTechnique(0)->getPass(0)->setDepthBias(1);
  material->getTechnique(0)->getPass(0)->setAmbient(1,1,1);
  material->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,1);
  material->getTechnique(0)->setLightingEnabled(false);
 }

 if (mVisualDebuggerRenderable == 0)
 {
  mVisualDebuggerRenderable = createRenderable(NxOgre::Enums::RenderableType_VisualDebugger, "RenderSystem.VisualDebugger");
  ::NxOgre::World::getWorld()->getVisualDebugger()->setRenderable(mVisualDebuggerRenderable);
  mVisualDebuggerRenderable->setCastShadows(false);
  mVisualDebuggerNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
  mVisualDebuggerNode->attachObject(mVisualDebuggerRenderable);
 }
 
 ::NxOgre::World::getWorld()->getVisualDebugger()->setVisualisationMode(type);
 
 if (type == NxOgre::Enums::VisualDebugger_ShowNone)
  mVisualDebuggerNode->setVisible(false);
 else
  mVisualDebuggerNode->setVisible(true);
 
 mVisualDebuggerShown = (type != NxOgre::Enums::VisualDebugger_ShowNone);
 
}

bool RenderSystem::hasDebugVisualisation(void) const
{
 return mVisualDebuggerRenderable && mVisualDebuggerShown;
}

PointRenderable* RenderSystem::createPointRenderable(const Ogre::String& ogre_mesh_name)
{
 PointRenderable* renderable = NXOGRE_NEW_NXOGRE PointRenderable(this, ogre_mesh_name);
 mPointRenderables.push_back(renderable);
 return renderable;
}

PointRenderable* RenderSystem::createPointRenderable(Ogre::MovableObject* movable_object)
{
 PointRenderable* renderable = NXOGRE_NEW_NXOGRE PointRenderable(this, movable_object);
 mPointRenderables.push_back(renderable);
 return renderable;
}

void RenderSystem::destroyPointRenderable(PointRenderable* renderable)
{
 if (renderable == 0)
  return;
 mPointRenderables.erase(renderable);
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

                                                                                       

} // namespace
