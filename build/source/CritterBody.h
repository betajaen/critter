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

                                                                                       

#ifndef CRITTER_BODY_H
#define CRITTER_BODY_H

#include "CritterStable.h"
#include "CritterCommon.h"

#include "CritterBodyDescription.h"

                                                                                       

namespace Critter
{

/*! class. Body
    desc.
        A Body is just an Actor that is "rendered" on the screen. Not only containing
        the standard physics properties and functions of an Actor. It also contains some
        information describing the object visually; what shape it is, it's dimensions and
        it's colour. Once combined that with the getPose() function from Actor. It's pretty
        easy to use all of that information to draw something on the screen. Of course in
        different types of RenderSystems available; the render information would be different.
        Ogre's for example wouldn't be as low-level, and would only contain a pointer to a node
        on a scenegraph, as well as reference to the mesh it is using.
*/
class CritterPublicClass Body : public NxOgre::Actor, public NxOgre::TimeListener
{
 
 public:
 
  
  friend class RenderSystem;
  
  NXOGRE_GC_FRIEND_NEW4
  NXOGRE_GC_FRIEND_DELETE
  
 public:
   
   /** \brief Returns "RigidBodyType_Body" as the type of RigidBody.
*/
   virtual unsigned int getRigidBodyType() const;
   
   /** \brief Get the SceneManager that the SceneNode is in.
*/
   Ogre::SceneManager* getSceneManager();
   
   /** \brief Get the SceneNode is in, or NULL if there isn't one.
*/
   Ogre::SceneNode* getSceneNode();
    
   /** \brief Set the SceneNode to a new one
*/
   void setSceneNode(Ogre::SceneNode*, Enums::SceneNodeDestructorBehaviour = Enums::SceneNodeDestructorBehaviour_Inherit);
   
   /** \brief Get the behaviour of the Scenenode when calling setSceneNode or the destructor.
*/
   Enums::SceneNodeDestructorBehaviour getSceneNodeDestructorBehaviour() const;
   
   /** \brief Get the behaviour of the Scenenode when calling setSceneNode or the destructor.
*/
   void setSceneNodeDestructorBehaviour(Enums::SceneNodeDestructorBehaviour);
   
   /** \brief "Drawing" function, called by the TimeListener - Changes the SceneNode pose to the same one as the NxActor's.
*/
   bool advance(float, const NxOgre::Enums::Priority&, const NxOgre::Enums::SceneFunction&);
   
 protected:
   
   /** \internal. Use RenderSystem::createBody
*/
   Body(const NxOgre::ShapeDescription&, const NxOgre::Matrix44& pose, const BodyDescription&, RenderSystem*);
     
   /** \internal. Use RenderSystem::createBody
*/
   Body(const NxOgre::ShapeDescriptions&, const NxOgre::Matrix44& pose, const BodyDescription&, RenderSystem*);
   
   /** \internal. Use RenderSystem::destroyBody
*/
   virtual ~Body();
   
   void _destructNode(Enums::SceneNodeDestructorBehaviour);
   
 protected:
     
     RenderSystem* mRenderSystem; //< \brief Body's Rendersystem
     Ogre::SceneManager* mSceneManager; //< \brief Scenenode's SceneManager.
     Ogre::SceneNode* mNode; //< \brief Scenenode itself..
     NxOgre::Enums::Priority mRenderPriority; //< \brief Render priority.
     Enums::SceneNodeDestructorBehaviour mSceneNodeDestructorBehaviour; //< \brief Behaviour when deleting the SceneNode.
     NxOgre::Matrix44 mAlphaPose; //< \brief Interpolation pose.


};

} // namespace

                                                                                       

#endif
