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

                                                                                       

#ifndef CRITTER_RENDERABLE_H
#define CRITTER_RENDERABLE_H

#include "CritterStable.h"
#include "CritterCommon.h"

#include "CritterBodyDescription.h"

                                                                                       

namespace Critter
{

class CritterPublicClass Renderable  : public NxOgre::Renderable, public Ogre::SimpleRenderable
{
    
  public:
     
     enum VertexDeclaration
     {
      VertexDeclaration_Position = 0,
      VertexDeclaration_TextureCoordinates = 1,
      VertexDeclaration_Normal = 2,
      VertexDeclaration_Colour = 3,
     };

     struct RenderProfile
     {
      RenderProfile();
      // operation, normals, colour, indexes, texture-coords, 16bit indexes.
      RenderProfile(Ogre::RenderOperation::OperationType op, bool n, bool c, bool i, bool t, bool _16);
      bool usesNormals;
      bool usesColourVertices;
      bool usesIndexes;
      bool uses16BitIndexes;
      bool usesTextureCoordinates;
      Ogre::RenderOperation::OperationType mRenderOperation;
     };
     
     static RenderProfile NXOGRE_VISUALDEBUGGER;
     static RenderProfile NXOGRE_PHYSXMESH;
     static RenderProfile NXOGRE_PARTICLE_POINTS;
     static RenderProfile NXOGRE_PARTICLE_VELOCITIES;
     static RenderProfile NXOGRE_SOFTBODY;
     
     /** \brief
*/
     Renderable(int);
     
     /** \brief
*/
    ~Renderable();
          
     /** \brief Text
*/
     void drawSoftBodySimple(NxOgre::MeshRenderable*, const NxOgre::Bounds3&);
     
     /** \brief Text
*/
     void drawCloth(NxOgre::MeshRenderable*, const NxOgre::Bounds3&);
     
     /** \brief Text
*/
     void drawClothFast(NxOgre::MeshRenderable*, const NxOgre::Bounds3&);
     
     /** \brief Text
*/
     void drawVisualDebugger(NxOgre::VisualDebuggerMeshData*);
     
     /** \brief Text
*/
     void drawFluid(NxOgre::PhysXParticleData*, const NxOgre::Bounds3&);
     
     /** \brief Text
*/
     Ogre::Real getBoundingRadius() const;
     
     /** \brief
*/
     Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
     
  protected: // functions
     
     inline void _createProfile(int);
     inline void _initialise();
     inline void _resize(size_t vertexCount = 0, size_t indexCount = 0);
     
  protected: // functions
     
     RenderProfile mProfile;
     
     /** \brief Maximum capacity of the currently allocated vertex buffer.
*/
     size_t mVertexBufferCapacity;
     
     /** \brief Maximum capacity of the currently allocated index buffer.
*/
     size_t mIndexBufferCapacity;
     
     //
     Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
     
     //
     Ogre::HardwareVertexBufferSharedPtr mVertexColourBuffer;
     
     //
     Ogre::HardwareVertexBufferSharedPtr mNormalBuffer;
     
     //
     Ogre::HardwareVertexBufferSharedPtr mTextureCoordsBuffer;
     
     //
     Ogre::HardwareIndexBufferSharedPtr mIndexBuffer;
    
  private:
     
     inline void _autoResize(NxOgre::MeshRenderable*);
     inline void _writeVertices(NxOgre::MeshRenderable*);
     inline void _writeNormals(NxOgre::MeshRenderable*);
     inline void _writeTextureCoords(NxOgre::MeshRenderable*);
     inline void _writeIndexes(NxOgre::MeshRenderable*);
     inline void _setBounds(const NxOgre::Bounds3&);
     
}; // class


} // namespace

                                                                                       

#endif
