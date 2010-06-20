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

                                                                                       

#ifndef CRITTER_ENUMS_H
#define CRITTER_ENUMS_H

                                                                                       

namespace Critter
{

                                                                                       

namespace Enums
{

                                                                                       

enum SceneNodeDestructorBehaviour
{
 SceneNodeDestructorBehaviour_Destroy, //< \brief Destroy the SceneNode, it's children, and it's movable objects.
 SceneNodeDestructorBehaviour_Remove, //< \brief Remove itself from the Root SceneNode.
 SceneNodeDestructorBehaviour_Inherit //< \brief Use the behaviour as given by the class.
};

enum ClassTypes
{
 _OgreResource          = NxOgre::Classes::RENDERSYSTEMS_CLASSES_BEGIN + 1500,
 _OgreResourceProtocol,

};

enum RigidBodyTypes
{
 RigidBodyType_Body          = NxOgre::Enums::RigidBodyType_USER_BEGIN + 8371,
 RigidBodyType_KinematicBody,
 RigidBodyType_AnimatedCharacter
};

enum FluidRenderableType
{
 FluidType_Position     = NxOgre::Enums::RenderableType_ParticlePoints,
 FluidType_Velocity     = FluidType_Position + 100,
 FluidType_OgreParticle = FluidType_Position + 101,
};

enum AnimatedCharacterState
{
 AnimatedCharacterState_Idle,
 AnimatedCharacterState_Walk,
 AnimatedCharacterState_Run,
 AnimatedCharacterState_JumpBegin,
 AnimatedCharacterState_JumpLoop,
 AnimatedCharacterState_JumpEnd,
 AnimatedCharacterState_FallBegin,
 AnimatedCharacterState_FallLoop,
 AnimatedCharacterState_FallEnd,
};

                                                                                       

} // enums

                                                                                       

} // critter

                                                                                       

#endif
