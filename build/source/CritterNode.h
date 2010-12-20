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

                                                                                       

#ifndef CRITTER_NODE_H
#define CRITTER_NODE_H

#include "CritterStable.h"
#include "CritterCommon.h"
#include "CritterAnimationState.h"

                                                                                       

namespace Critter
{

/*! class. Node
    desc.
        A Node is a helper/wrapper class for the Ogre SceneNode class.
*/
class CritterPublicClass Node : public NxOgre::UserExtendedClassAllocatable
{
  
 public:
    
    Node(Ogre::SceneManager* scene_mgr, RenderSystem*);
    
    Node(Ogre::SceneManager* scene_mgr, Ogre::SceneNode* parent, RenderSystem*);
    
   ~Node();
    
    Ogre::Entity* getEntityAt(unsigned short index);
    
    Ogre::Camera* getCameraAt(unsigned short index);
    
    Ogre::ParticleSystem* getParticleSystemAt(unsigned short index);
    
    Ogre::Light* getLightAt(unsigned short index);
    
    Ogre::MovableObject* getAt(unsigned short index);
    
    unsigned short numAttachedObjects() const;

    unsigned short numChildren() const;

    Ogre::SceneNode* createChildSceneNode();
    
    void setPose(const Ogre::Matrix4&);

    void setPose(const NxOgre::Matrix44&);
    
    void setPose(const NxOgre::Vec3& position, const NxOgre::Quat& orientation);
    
    void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);

    void setPosition(const Ogre::Vector3& position);
    
    void setPosition(const NxOgre::Vec3& position);
    
    Ogre::Vector3 getPosition() const;

    NxOgre::Vec3 getPositionVec3() const;
    
    void setOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z);
    
    void setOrientation(const Ogre::Quaternion& orientation);

    void setOrientation(const NxOgre::Quat& orientation);
    
    void setOrientation(Ogre::Radian angle, NxOgre::Enums::Axis);
    
    void setOrientation(Ogre::Radian angle, const Ogre::Vector3& axis );

    Ogre::Quaternion getOrientation() const;

    NxOgre::Quat getOrientationQuat() const;

    void createAndAttachEntity(const Ogre::String& mesh_name);

    void createAndAttachEntity(const Ogre::String& entity_name, const Ogre::String& mesh_name);
   
    /** \brief Add a pre-existing node to the node.
    \note If this node is a child of another, it will remove the node for you.
    */
    void addSceneNode(Ogre::SceneNode*);

    /** \brief Add a pre-existing node to the node using a string as an identifier.
    \note If this node is a child of another, it will remove the node for you.
    */
    void addSceneNode(const Ogre::String&);

    /** \brief Add a pre-existing entity to the node.
    \note If this entity is already attached to another scenenode it will remove it for you.
    */
    void addEntity(Ogre::Entity*);

    /** \brief Add a movable object to this node.
    \note If this movable object is already attached to another scenenode it will remove it for you.
    */
    void addMovableObject(Ogre::MovableObject*);

    /** \brief Removes a node from the node
    */
    void removeSceneNode(Ogre::SceneNode*);

    /** \brief Removes a node from the node using a string as an identifier
    */
    void removeSceneNode(const Ogre::String&);

    /** \brief Remove an entity from the node, but don't delete it
    */
    void removeEntity(Ogre::Entity*);

    /** \brief Remove an entity from the node, but don't delete it,
    using a string as an identifier.
    */
    void removeEntity(const Ogre::String&);

    /** \brief Destroys an entity from the node.
    */
    void destroyEntity(Ogre::Entity*);

    /** \brief Destroys an entity from the node, using a string as
    an identifier.
    */
    void destroyEntity(const Ogre::String&);

    /** \brief Remove an MovableObject from the node, but don't delete it.
    */
    void removeMovableObject(Ogre::MovableObject*);

    /** \brief Remove an MovableObject from the node, but don't delete it, using a string as an identifier.
    */
    void removeMovableObject(const Ogre::String&);

    /** \brief Destroys an MovableObject from the node.
    */
    void destroyMovableObject(Ogre::MovableObject*);

    /** \brief Destroys an MovableObject from the node.
    */
    void destroyMovableObject(const Ogre::String&);

    /** \brief Get the Node.
    */
    Ogre::SceneNode* getSceneNode() const;

    /** \brief Get the SceneManager used.
    */
    Ogre::SceneManager* getSceneManager() const;
    
    void setScale(Ogre::Real);
    
    void setScale(const Ogre::Vector3&);
    
    void _setupAnimations(size_t switchTo = 0);
    
    void updateAnimations(Ogre::Real deltaTime);
    
    void fadeAnimations(Ogre::Real deltaTime);
    
    void setAnimation(size_t section, size_t index, bool reset = false);
    
    size_t getCurrentAnimation(size_t section) const;
    
    Enums::AnimationStatus getAnimationStatus(size_t section, size_t index) const;
    
    Ogre::Real getCurrentAnimationSpeed(size_t section) const;
    
    void setCurrentAnimationSpeed(size_t section, Ogre::Real);
    
    bool getCurrentAnimationEnded(size_t section) const;
    
    bool getCurrentAnimationLoops(size_t section) const;

 protected:
    
    inline Ogre::Entity* fetchEntity(const Ogre::String& name);
    
    inline void destroyNode(Ogre::SceneNode*);
     
    inline void parseMovableObject(Ogre::MovableObject*);
    
    Ogre::SceneNode*    mNode;
    
    Ogre::SceneManager* mSceneMgr;
    
    RenderSystem*       mRenderSystem;

    Enums::SceneNodeDestructorBehaviour mNodeBehaviour;
    
    AnimationStates     mAnimations[CRITTER_MAX_ANIMATION_SECTIONS];
    
    AnimationStateIterator mAnimationIterator;

    size_t              mCurrentAnimation[CRITTER_MAX_ANIMATION_SECTIONS];
    
    Ogre::Real          mCurrentAnimationSpeed[CRITTER_MAX_ANIMATION_SECTIONS];
    
  private:
    
    static size_t  sUniqueID;
};

} // namespace Critter

                                                                                       

#endif
