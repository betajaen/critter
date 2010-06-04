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
#include "CritterPointRenderable.h"

#include "CritterRenderSystem.h"

                                                                                       

namespace Critter
{

                                                                                       

PointRenderable::PointRenderable(RenderSystem* renderSystem, const Ogre::String& ogre_mesh_name)
: mRenderSystem(renderSystem), mNode(0)
{
 mNode = mRenderSystem->getSceneManager()->getRootSceneNode()->createChildSceneNode();
 addEntity(fetchEntity(ogre_mesh_name));
}


PointRenderable::PointRenderable(RenderSystem* renderSystem, Ogre::MovableObject* movable_object)
{
 mNode = mRenderSystem->getSceneManager()->getRootSceneNode()->createChildSceneNode();
 addMovableObject(movable_object);
}

PointRenderable::~PointRenderable()
{
 destroyNode(mNode);
 mNode = 0;
}

void PointRenderable::render(const NxOgre::Vec3& position, const NxOgre::Quat& orientation)
{
 mNode->setPosition(position.as<Ogre::Vector3>());
 mNode->setOrientation(orientation.as<Ogre::Quaternion>());
}

void PointRenderable::destroyNode(Ogre::SceneNode* node)
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
   mRenderSystem->getSceneManager()->getRootSceneNode()->attachObject(m);
  }
  // Everything else gets destroyed.
  else
   mRenderSystem->getSceneManager()->destroyMovableObject(m);
 }
 
 node->detachAllObjects();
 node->removeAndDestroyAllChildren();
 
 mRenderSystem->getSceneManager()->destroySceneNode(node);
}

Ogre::Entity* PointRenderable::fetchEntity(const Ogre::String& name)
{
 Ogre::Entity* entity = NULL;
 
 if (Ogre::StringUtil::endsWith(name, ".mesh")) // Is it a mesh?
 {
  entity = mRenderSystem->getSceneManager()->createEntity(mRenderSystem->getUniqueName("Entity"), name);
 }
 else if (mRenderSystem->getSceneManager()->hasEntity(name)) // Could it be an entity?
 {
  entity = mRenderSystem->getSceneManager()->getEntity(name);
  
  if (entity->isAttached())
   entity->getParentSceneNode()->detachObject(entity);
 }
 else // It must be a mesh.
 {
  entity = mRenderSystem->getSceneManager()->createEntity(mRenderSystem->getUniqueName("Entity"), name);
 }
 
 return entity;
}

Ogre::SceneNode* PointRenderable::getNode()
{
 return mNode;
}

void PointRenderable::parseMovableObject(Ogre::MovableObject* mo)
{
 if (mo == 0)
 return;

 if (mo->isAttached())
  mo->getParentSceneNode()->detachObject(mo);
}

void PointRenderable::addSceneNode(const Ogre::String& name)
{
  
  if (mRenderSystem->getSceneManager()->hasSceneNode(name) == false)
   return;
  
  Ogre::SceneNode* node = mRenderSystem->getSceneManager()->getSceneNode(name);
  Ogre::SceneNode* pnode = node->getParentSceneNode();
  
  if (pnode)
   pnode->removeChild(node);
  
  mNode->addChild(node);

}

void PointRenderable::addEntity(Ogre::Entity* entity)
{
 parseMovableObject(entity);
 mNode->attachObject(entity);
}

void PointRenderable::addMovableObject(Ogre::MovableObject* mo)
{
 parseMovableObject(mo);
 mNode->attachObject(mo);
}

void PointRenderable::removeSceneNode(Ogre::SceneNode* node)
{

 if (node == 0)
  return;

 Ogre::SceneNode* pnode = node->getParentSceneNode();

 if (pnode == 0)
  return;

 if (pnode == mNode)
  pnode->removeChild(node);

}

void PointRenderable::removeSceneNode(const Ogre::String& name)
{
 if (mRenderSystem->getSceneManager()->hasSceneNode(name) == false)
  return;

 Ogre::SceneNode* node = mRenderSystem->getSceneManager()->getSceneNode(name);
 Ogre::SceneNode* pnode = node->getParentSceneNode();

 if (pnode == 0)
  return;

 if (pnode == mNode)
  pnode->removeChild(node);

}

void PointRenderable::removeEntity(Ogre::Entity* entity)
{
 removeMovableObject(entity);
}

void PointRenderable::removeEntity(const Ogre::String& name)
{
 removeMovableObject(name);
}

void PointRenderable::destroyEntity(Ogre::Entity* entity)
{
 destroyMovableObject(entity);
}

void PointRenderable::destroyEntity(const Ogre::String& name)
{
 destroyMovableObject(name);
}

void PointRenderable::removeMovableObject(Ogre::MovableObject* mo)
{
 if (mo == 0)
  return;
 
 mNode->detachObject(mo);
}

void PointRenderable::removeMovableObject(const Ogre::String& name)
{
 mNode->detachObject(name);
}

void PointRenderable::destroyMovableObject(Ogre::MovableObject* mo)
{
 removeMovableObject(mo);
 mRenderSystem->getSceneManager()->destroyMovableObject(mo);
}

void PointRenderable::destroyMovableObject(const Ogre::String& name)
{
 Ogre::MovableObject* mo = mNode->getAttachedObject(name);
 destroyMovableObject(mo);
}

                                                                                       

} // namespace

                                                                                       
