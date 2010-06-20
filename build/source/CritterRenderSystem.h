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

                                                                                       

#ifndef CRITTER_RENDERSYSTEM_H
#define CRITTER_RENDERSYSTEM_H

#include "CritterStable.h"
#include "CritterCommon.h"

#include "CritterBodyDescription.h"

                                                                                       

namespace Critter
{

class CritterPublicClass RenderSystem : public NxOgre::UserBigClassAllocatable, public NxOgre::TimeListener
{
 public:

   typedef NxOgre::multihashmap<Body*, NxOgre::GC::HasGarbageCollection> Bodies;

   typedef NxOgre::multihashmap_iterator<Body*> BodyIterator;

   typedef NxOgre::multihashmap<KinematicBody*, NxOgre::GC::HasGarbageCollection> KinematicBodies;

   typedef NxOgre::multihashmap_iterator<KinematicBody*> KinematicBodyIterator;

   typedef NxOgre::vector<Renderable*> Renderables;

   typedef NxOgre::vector_iterator<Renderable*> RenderableIterator;

   typedef NxOgre::vector<PointRenderable*> PointRenderables;

   typedef NxOgre::vector_iterator<PointRenderable*> PointRenderableIterator;

   RenderSystem(NxOgre::Scene*, Ogre::SceneManager* = ::Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext());

  ~RenderSystem();

   NxOgre::Scene* getScene();

   /** \brief Create a Body.
   */
   Body* createBody(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, BodyDescription& = BodyDescription());

   /** \brief Create a Body.
   */
   Body* createBody(const NxOgre::ShapeDescriptions&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, BodyDescription& = BodyDescription());

   /** \brief Create a Body; alternate helper function, Which creates the SceneNode and Entity "meshName" for you.
   */
   Body* createBody(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, const Ogre::String& meshName = Ogre::StringUtil::BLANK, BodyDescription& = BodyDescription());

   /** \brief Create a Body; alternate helper function, Which creates the SceneNode and Entity "meshName" for you.
   */
   Body* createBody(const NxOgre::ShapeDescriptions&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, const Ogre::String& meshName = Ogre::StringUtil::BLANK, BodyDescription& = BodyDescription());

   /** \brief Destroy a Body.
   */
   void destroyBody(Body*);

   /** \brief Get body iterator
   */
   BodyIterator getBodies();

   /** \brief Create and manage a Renderable.
   */
   Renderable* createRenderable(int renderableType, const Ogre::String& materialName = "BaseWhiteNoLighting");

   /** \brief Destroy a Renderable.
   */
   void destroyRenderable(Renderable*);

   /** \brief Create and manage a point Renderable.
   */
   PointRenderable* createPointRenderable(const Ogre::String& ogre_mesh_name);

   /** \brief Create and manage a point Renderable.
   */
   PointRenderable* createPointRenderable(Ogre::MovableObject*);

   /** \brief Destroy a Renderable.
   */
   void destroyPointRenderable(PointRenderable*);

   /** \brief Create a KinematicBody, a KinematicActor as a Body.
   */
   KinematicBody* createKinematicBody(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& pose, const Ogre::String& meshName, BodyDescription& = BodyDescription());

   /** \brief Create a KinematicBody, a KinematicActor as a Body.
   */
   KinematicBody* createKinematicBody(const NxOgre::ShapeDescriptions&, const NxOgre::Matrix44& pose, const Ogre::String& meshName, BodyDescription& = BodyDescription());

   /** \brief Destroy a KinematicBody.
   */
   void destroyKinematicBody(KinematicBody*);

   /** \brief Helper function to create a cloth using the Renderable
   */
   NxOgre::Cloth* createCloth(const NxOgre::ClothDescription&, const Ogre::String& material = "BaseWhiteNoLighting");

   /** \brief Helper function to destroya cloth with a Renderable
   */
   void destroyCloth(NxOgre::Cloth*);

   /** \brief Helper function to create a soft body using the Renderable
   */
   NxOgre::SoftBody* createSoftBody(const NxOgre::SoftBodyDescription&, const Ogre::String& material = "BaseWhiteNoLighting");

   /** \brief Helper function to destroya soft body with a Renderable
   */
   void destroySoftBody(NxOgre::SoftBody*);

   /** \brief Create a fluid
   */
   NxOgre::Fluid* createFluid(const NxOgre::FluidDescription&, const Ogre::String& material = "BaseWhiteNoLighting", ::Critter::Enums::FluidRenderableType = ::Critter::Enums::FluidType_Position);

   /** \brief Destroy a fluid that has been created with the createFluid. Cleans up the Renderable, which Scene::createFluid would not.
   */
   void destroyFluid(NxOgre::Fluid*);

   /** \brief Helper function to create a Ogre SceneNode and Entity pair.
   */
   Ogre::SceneNode* createSceneNodeEntityPair(const Ogre::String& meshName, const Ogre::Vector3& = Ogre::Vector3::ZERO, const Ogre::Quaternion& = Ogre::Quaternion::IDENTITY);
   Ogre::SceneNode* createSceneNodeEntityPair(const NxOgre::String& meshName, const NxOgre::Vec3& = NxOgre::Vec3::ZERO, const NxOgre::Quat& = NxOgre::Quat::IDENTITY);

#if Critter_UsesOgreTerrain == 1
   /*! function. Create Terrain from the Ogre 1.7 terrain system.
      
   */
   NxOgre::SceneGeometry* createTerrain(Ogre::Terrain*);
#endif

   /** \brief Helper function for Debug Visualisation.
   */
   void setVisualisationMode(NxOgre::Enums::VisualDebugger);

   /** \brief Is the Visual Debuggger active?
   */
   bool hasDebugVisualisation() const;

   /** \internal Do not call.
   */
   bool advance(float user_deltaTime, const NxOgre::Enums::Priority&, const NxOgre::Enums::SceneFunction&);

   /** \brief
   */
   Ogre::SceneManager* getSceneManager();

   /** \brief Create an unique name
   */
   Ogre::String getUniqueName(const Ogre::String& prefix) const;

  protected:
     
     NxOgre::Scene* mScene;
     
     Ogre::SceneManager* mSceneManager;
     
     Bodies mBodies;
     
     KinematicBodies mKinematicBodies;
     
     Renderables mRenderables;
     
     PointRenderables mPointRenderables;
     
     Renderable* mVisualDebuggerRenderable;
     
     Ogre::SceneNode* mVisualDebuggerNode;
     
     bool mVisualDebuggerShown;
     
     static unsigned int mUniqueIdentifier;

}; // class

} // namespace

                                                                                       

#endif
