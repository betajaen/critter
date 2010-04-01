/** File: OGRE3DRenderSystem.h
    Created on: 25-Nov-08
    Author: Robin Southern "betajaen"
    

    Copyright (c) 2008-2009 Robin Southern

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

#ifndef OGRE3D_RENDERSYSTEM_H
#define OGRE3D_RENDERSYSTEM_H

                                                                                       

#include "CritterCommon.h"
#include "CritterRigidBodyDescription.h"

#include "NxOgre.h"
#include "NxOgreArray.h"
#include "OGRE/Ogre.h"

#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 7

#include <OGRE/Terrain/OgreTerrain.h>

#endif

                                                                                       

class OGRE3DExportClass OGRE3DRenderSystem : public NxOgre::UserBigClassAllocatable, public NxOgre::TimeListener
{
  public:
     
     typedef NxOgre::ptr_multihashmap<OGRE3DBody>                       Bodies;
     
     typedef NxOgre::ptr_multihashmap<OGRE3DBody>::iterator_t           BodyIterator;
     
     typedef NxOgre::ptr_multihashmap<OGRE3DKinematicBody>              KinematicBodies;
     
     typedef NxOgre::ptr_multihashmap<OGRE3DKinematicBody>::iterator_t  KinematicBodyIterator;
     
     typedef NxOgre::ptr_vector<OGRE3DRenderable>                       Renderables;
     
     typedef NxOgre::ptr_vector<OGRE3DRenderable>::iterator_t           RenderableIterator;
     
     typedef NxOgre::ptr_vector<OGRE3DPointRenderable>                  PointRenderables;
     
     typedef NxOgre::ptr_vector<OGRE3DPointRenderable>::iterator_t      PointRenderableIterator;
     
     /** \brief OGRE3DRenderSystem constructor.
         \param Scene* Scene to work with
         \param SceneManager SceneManager, or 0 to use the first available.
         
     */
     OGRE3DRenderSystem(NxOgre::Scene*, Ogre::SceneManager* = ::Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext());
     
     /** \brief
     */
    ~OGRE3DRenderSystem(void);
     
     /** \brief Get the scene.
     */
     NxOgre::Scene*                       getScene(void);
     
     /** \brief Create a Body.
     */
     OGRE3DBody*                          createBody(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, OGRE3DRigidBodyDescription& = OGRE3DRigidBodyDescription());
     
     /** \brief Create a Body.
     */
     OGRE3DBody*                          createBody(const NxOgre::ShapeDescriptions&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, OGRE3DRigidBodyDescription& = OGRE3DRigidBodyDescription());
     
     /** \brief Create a Body; alternate helper function, Which creates the SceneNode and Entity "meshName" for you.
     */
     OGRE3DBody*                          createBody(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, const Ogre::String& meshName = Ogre::StringUtil::BLANK, OGRE3DRigidBodyDescription& = OGRE3DRigidBodyDescription());
     
     /** \brief Create a Body; alternate helper function, Which creates the SceneNode and Entity "meshName" for you.
     */
     OGRE3DBody*                          createBody(const NxOgre::ShapeDescriptions&, const NxOgre::Matrix44& = NxOgre::Matrix44::IDENTITY, const Ogre::String& meshName = Ogre::StringUtil::BLANK, OGRE3DRigidBodyDescription& = OGRE3DRigidBodyDescription());
     
     /** \brief Destroy a Body.
     */
     void                                 destroyBody(OGRE3DBody*);
     
     /** \brief Get body iterator
     */
     BodyIterator                         getBodies();

     /** \brief Create and manage a Renderable.
     */
     OGRE3DRenderable*                    createRenderable(int renderableType, const Ogre::String& materialName = "BaseWhiteNoLighting");
     
     /** \brief Destroy a Renderable.
     */
     void                                 destroyRenderable(OGRE3DRenderable*);
     
     /** \brief Create and manage a point Renderable.
     */
     OGRE3DPointRenderable*               createPointRenderable(const Ogre::String& ogre_mesh_name);
     
     /** \brief Create and manage a point Renderable.
     */
     OGRE3DPointRenderable*               createPointRenderable(Ogre::MovableObject*);
     
     /** \brief Destroy a Renderable.
     */
     void                                 destroyPointRenderable(OGRE3DPointRenderable*);
     
     /** \brief Create a KinematicBody, a KinematicActor as a Body.
     */
     OGRE3DKinematicBody*                 createKinematicBody(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& pose, const Ogre::String& meshName, OGRE3DRigidBodyDescription& = OGRE3DRigidBodyDescription());
     
     /** \brief Destroy a KinematicBody.
     */
     void                                 destroyKinematicBody(OGRE3DKinematicBody*);
     
     /** \brief Helper function to create a cloth using the OGRE3DRenderable
     */
     NxOgre::Cloth*                       createCloth(const NxOgre::ClothDescription&, const Ogre::String& material = "BaseWhiteNoLighting");
     
     /** \brief Helper function to destroya cloth with a OGRE3DRenderable
     */
     void                                 destroyCloth(NxOgre::Cloth*);
     
     /** \brief Helper function to create a soft body using the OGRE3DRenderable
     */
     NxOgre::SoftBody*                    createSoftBody(const NxOgre::SoftBodyDescription&, const Ogre::String& material = "BaseWhiteNoLighting");
     
     /** \brief Helper function to destroya soft body with a OGRE3DRenderable
     */
     void                                 destroySoftBody(NxOgre::SoftBody*);
     
     /** \brief Create a fluid
     */
     NxOgre::Fluid*                       createFluid(const NxOgre::FluidDescription&, const Ogre::String& material = "BaseWhiteNoLighting", OGRE3DFluidRenderableType = OGRE3DFluidType_Position);
     
     /** \brief Destroy a fluid that has been created with the createFluid. Cleans up the Renderable, which Scene::createFluid would not.
     */
     void                                 destroyFluid(NxOgre::Fluid*);


     /** \brief Helper function for Debug Visualisation.
     */
     void                                 setVisualisationMode(NxOgre::Enums::VisualDebugger);
     
     /** \brief Is the Visual Debuggger active?
     */
     bool                                 hasDebugVisualisation(void) const;
     
     /** \internal Do not call.
     */
     bool                                 advance(float user_deltaTime, const NxOgre::Enums::Priority&);
     
     /** \brief
     */
     Ogre::SceneManager*                  getSceneManager();
     
     /** \brief Create an unique name
     */
     Ogre::String                         getUniqueName(const Ogre::String& prefix) const;

#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 7
     /*! function.  createOgreTerrainGeometry
         desc.
             Duplicates the Ogre 1.7.0 geometry as a NxOgre Heightfield.
             See: http://www.ogre3d.org/wiki/index.php/BloodyMess_Ogre::Terrain_Heightfield
         return.
             
     */
     NxOgre::SceneGeometry*               createOgreTerrainGeometry(const OgreTerrainDescription&);
#endif
    
  protected:
     
     NxOgre::Scene*                        mScene;
     
     Ogre::SceneManager*                   mSceneManager;
     
     Bodies                                mBodies;
     
     KinematicBodies                       mKinematicBodies;
     
     Renderables                           mRenderables;
     
     PointRenderables                      mPointRenderables;
     
     OGRE3DRenderable*                     mVisualDebuggerRenderable;
     
     Ogre::SceneNode*                      mVisualDebuggerNode;
     
     bool                                  mVisualDebuggerShown;
     
     static unsigned int                   mUniqueIdentifier;
     
};

                                                                                       

#endif
