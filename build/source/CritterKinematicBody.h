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

                                                                                       

#ifndef CRITTER_KINEMATICBODY_H
#define CRITTER_KINEMATICBODY_H

#include "CritterStable.h"
#include "CritterCommon.h"

#include "CritterBodyDescription.h"

                                                                                       

namespace Critter
{

class KinematicBody : public NxOgre::KinematicActor, public NxOgre::TimeListener
{
  
  NXOGRE_GC_FRIEND_NEW4
  NXOGRE_GC_FRIEND_DELETE
  
  public:
    
   /** \brief
   */
   virtual  unsigned int  getRigidBodyType() const;
   
    /** \brief Get the SceneManager that the SceneNode is in.
    */
   Ogre::SceneManager* getSceneManager();
  
    /** \brief Get the SceneNode is in, or NULL if there isn't one.
    */
   Ogre::SceneNode*    getSceneNode();
  
   void                setSceneNode(Ogre::SceneNode*, Enums::SceneNodeDestructorBehaviour = Enums::SceneNodeDestructorBehaviour_Inherit);
   
   /** \brief Get the behaviour of the Scenenode when calling setSceneNode or the destructor.
   */
   Enums::SceneNodeDestructorBehaviour getSceneNodeDestructorBehaviour() const;
   
   /** \brief Get the behaviour of the Scenenode when calling setSceneNode or the destructor.
   */
   void setSceneNodeDestructorBehaviour(Enums::SceneNodeDestructorBehaviour);
  
    /** \brief "Drawing" function, called by the TimeListener.
    */
   bool                advance(float, const NxOgre::Enums::Priority&, const NxOgre::Enums::SceneFunction&);

  protected:
  
     /** \internal. Use RenderSystem::createBody
     */
     KinematicBody(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& pose, const BodyDescription&, RenderSystem*);
     
     /** \internal. Use RenderSystem::createBody
     */
     KinematicBody(const NxOgre::ShapeDescriptions&, const NxOgre::Matrix44& pose, const BodyDescription&, RenderSystem*);
     
     /** \internal. Use RenderSystem::destroyBody
     */
    ~KinematicBody();
     
     /** \brief
     */
     void _destructNode(Enums::SceneNodeDestructorBehaviour);
     
  protected:
  
     RenderSystem*                       mRenderSystem;   //< \brief Body's Rendersystem
     Ogre::SceneManager*                 mSceneManager;   //< \brief Scenenode's SceneManager.
     Ogre::SceneNode*                    mNode;           //< \brief Scenenode itself..
     NxOgre::Enums::Priority             mRenderPriority; //< \brief Render priority.
     Enums::SceneNodeDestructorBehaviour mSceneNodeDestructorBehaviour; //< \brief Behaviour when deleting the SceneNode.
     static unsigned int                 mNextBodyID;     //< \brief Not all actors have names. So when it comes to naming
                                                          //<        nodes and entities. It will use nextBodyID as an unique
                                                          //<        name; "ogre3d-entity-1", "ogre3d-node-1".


};


} // namespace

                                                                                       

#endif
