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
#include "CritterRenderable.h"

                                                                                       

Critter::Renderable::RenderProfile  Critter::Renderable::NXOGRE_VISUALDEBUGGER =
                  Critter::Renderable::RenderProfile (Ogre::RenderOperation::OT_LINE_LIST, false, true, false, false, false);

Critter::Renderable::RenderProfile  Critter::Renderable::NXOGRE_PHYSXMESH =
                  Critter::Renderable::RenderProfile (Ogre::RenderOperation::OT_TRIANGLE_LIST, true, false, true, true, false);

Critter::Renderable::RenderProfile  Critter::Renderable::NXOGRE_PARTICLE_POINTS =
                  Critter::Renderable::RenderProfile (Ogre::RenderOperation::OT_POINT_LIST, false, false, false, false, false);

Critter::Renderable::RenderProfile  Critter::Renderable::NXOGRE_PARTICLE_VELOCITIES =
                  Critter::Renderable::RenderProfile (Ogre::RenderOperation::OT_LINE_LIST, false, false, false, false, false);

Critter::Renderable::RenderProfile  Critter::Renderable::NXOGRE_SOFTBODY =
                  Critter::Renderable::RenderProfile (Ogre::RenderOperation::OT_TRIANGLE_LIST, false, false, true, false, false);

#define ABORT_IF_EMPTY if (renderable->vertices.count() < 3) { mBox.setExtents(Ogre::Vector3::ZERO, Ogre::Vector3::ZERO); return; }

                                                                                       

namespace Critter
{

                                                                                       

Renderable::RenderProfile::RenderProfile()
: mRenderOperation(Ogre::RenderOperation::OT_POINT_LIST),
  usesNormals(false),
  usesColourVertices(false),
  usesIndexes(false),
  uses16BitIndexes(false),
  usesTextureCoordinates(false)
{
}

Renderable::RenderProfile::RenderProfile(Ogre::RenderOperation::OperationType op, bool n, bool c, bool i, bool t, bool _16)
: mRenderOperation(op),
  usesNormals(n),
  usesColourVertices(c),
  usesIndexes(i),
  uses16BitIndexes(_16),
  usesTextureCoordinates(t)
{
}

Renderable::Renderable(int type)
: NxOgre::Renderable(type), Ogre::SimpleRenderable()
{
 _createProfile(mType);
 _initialise();
}

Renderable::~Renderable()
{
 
 if (mRenderOp.vertexData)
  delete mRenderOp.vertexData;
 
 if (mRenderOp.indexData)
  delete mRenderOp.indexData;
 
}

void Renderable::_autoResize(NxOgre::MeshRenderable* renderable)
{
 _resize(renderable->vertices.capacity(), renderable->indexes.count());
}

void Renderable::_writeVertices(NxOgre::MeshRenderable* renderable)
{
 mVertexBuffer->writeData(
   0,
   renderable->vertices.bytes(),
   renderable->vertices.vertices()
 );
}

void Renderable::_writeNormals(NxOgre::MeshRenderable* renderable)
{
 mNormalBuffer->writeData(
   0,
   renderable->vertices.bytes(),
   renderable->normals.normals()
 );
}

void Renderable::_writeTextureCoords(NxOgre::MeshRenderable* renderable)
{
 if (renderable->texture_coordinates.capacity())
 {
  mTextureCoordsBuffer->writeData(
   0,
   renderable->texture_coordinates.capacity() * sizeof(float),
   renderable->texture_coordinates.texture_coords()
  );
 }
}

void Renderable::_writeIndexes(NxOgre::MeshRenderable* renderable)
{
 mRenderOp.indexData
   ->indexBuffer
     ->writeData(
          0,
          renderable->indexes.bytes(),
          renderable->indexes.indexes()
 );
}

void Renderable::_setBounds(const NxOgre::Bounds3& bounds)
{
 mBox.setExtents(bounds.min.as<Ogre::Vector3>(), bounds.max.as<Ogre::Vector3>());
}

void Renderable::drawSoftBodySimple(NxOgre::MeshRenderable* renderable, const NxOgre::Bounds3& bounds)
{
 // Early escape.
 ABORT_IF_EMPTY
 
 // Resize buffers if necessary.
 _autoResize(renderable);
 
 // Write the vertices.
 _writeVertices(renderable);
 
 // Write the indices.
 _writeIndexes(renderable);
 
 // Set the extents.
 _setBounds(bounds);
 
 // Done.
}

void Renderable::drawCloth(NxOgre::MeshRenderable* renderable,  const NxOgre::Bounds3& bounds)
{
 
 // Early escape.
 ABORT_IF_EMPTY
 
 _autoResize(renderable);
 
 _writeVertices(renderable);
 _writeNormals(renderable);
 _writeTextureCoords(renderable);
 _writeIndexes(renderable);
 
 _setBounds(bounds);
 
 // Done.
}

void Renderable::drawClothFast(NxOgre::MeshRenderable* renderable, const NxOgre::Bounds3& bounds)
{
 
 // Resize buffers if necessary.
 _autoResize(renderable);

 // Write the vertices.
 _writeVertices(renderable);
 
 // Write the normals.
 _writeNormals(renderable);

 // Set the extents.
 _setBounds(bounds);
 
 // Done.
}
void Renderable::drawVisualDebugger(NxOgre::VisualDebuggerMeshData* renderable)
{
 _resize(renderable->getNbLines() * 2, 0);

 // Write the vertices.
 mVertexBuffer->writeData(0, 3 * renderable->getNbLines() * 2 * sizeof(float), renderable->getLines() );

 mVertexColourBuffer->writeData(0, renderable->getNbLines() * 2 * sizeof(unsigned int), renderable->getColours() );

 mBox.setInfinite();
 
}

void Renderable::drawFluid(NxOgre::PhysXParticleData* renderable, const NxOgre::Bounds3& bounds)
{
  
 if (mType == Enums::FluidType_Position)
 {
  
  // Resize buffers if necessary.
  _resize( renderable->getNbParticles(), 0);
  
  // Write the particle positions.
  mVertexBuffer->writeData(0, 3 * renderable->getNbParticles() * sizeof(float), renderable->getPositions());
  
 }
 else if (mType == Enums::FluidType_Velocity)
 {
  
  // Resize buffers if necessary.
  _resize(renderable->getNbParticles() * 2, 0);
  
  NxOgre::Vec3 a, b, v, p;
  float d = 0.016f;
  
  float *prPos = static_cast<float*>(mVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
  
  float* positions = renderable->getPositions();
  float* velocities = renderable->getVelocities();
  
  for (unsigned int i=0; i < renderable->getNbParticles() * 3; i+=3)
  {
   p.x = positions[i];
   p.y = positions[i+1];
   p.z = positions[i+2];

   v.x = velocities[i];
   v.y = velocities[i+1];
   v.z = velocities[i+2];
   v.normalise();
   
   a = p + (-v * d);
   b = p + (v * d);
   
   *prPos++ = a.x;
   *prPos++ = a.y;
   *prPos++ = a.z;
   *prPos++ = b.x;
   *prPos++ = b.y;
   *prPos++ = b.z;
  }
  
  mVertexBuffer->unlock();
  
 }
 
 
 mBox.setInfinite();
 
}


void Renderable::_createProfile(int type)
{
 switch(type)
 {
  case NxOgre::Enums::RenderableType_VisualDebugger:
   mProfile = Renderable::NXOGRE_VISUALDEBUGGER;
  break;
  
  case NxOgre::Enums::RenderableType_PhysXMesh:
   mProfile = Renderable::NXOGRE_PHYSXMESH;
  break;
  
  case NxOgre::Enums::RenderableType_ParticlePoints:
   mProfile = Renderable::NXOGRE_PARTICLE_POINTS;
  break;

  case Enums::FluidType_Velocity:
   mProfile = Renderable::NXOGRE_PARTICLE_VELOCITIES;
  break;
  
  case NxOgre::Enums::RenderableType_SoftBody:
   mProfile = Renderable::NXOGRE_SOFTBODY;
  break;
 }
}

void Renderable::_initialise()
{
 // Op.
 mRenderOp.operationType = mProfile.mRenderOperation;

 // Vertices and other vertex bits.
 mRenderOp.vertexData  = new Ogre::VertexData;

 // Main vertex declartion. It is assumed that a Renderable has vertices.
 Ogre::VertexDeclaration* vDecl = mRenderOp.vertexData->vertexDeclaration;
 vDecl->addElement(VertexDeclaration_Position, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
 
 // Normals.
 if (mProfile.usesNormals)
  vDecl->addElement(VertexDeclaration_Normal, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
 
 // Vertex Colours.
 if (mProfile.usesColourVertices)
  vDecl->addElement(VertexDeclaration_Colour, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
 
 if (mProfile.usesTextureCoordinates)
  vDecl->addElement(VertexDeclaration_TextureCoordinates, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

 // Vertex buffer capacity is zero.
 mVertexBufferCapacity = 0;


 // Do the indexes. If it has one.
 if (mRenderOp.useIndexes = mProfile.usesIndexes)
 {
  mRenderOp.indexData = new Ogre::IndexData;
 }

 mIndexBufferCapacity = 0;
}

void Renderable::_resize(size_t vertexCount, size_t indexCount)
{
  // Prepare vertex buffer
  size_t newVertCapacity = mVertexBufferCapacity;
  if ((vertexCount > mVertexBufferCapacity) ||
      (!mVertexBufferCapacity))
  {
    // vertexCount exceeds current capacity!
    // It is necessary to reallocate the buffer.

    // Check if this is the first call
    if (!newVertCapacity)
      newVertCapacity = 1;

    // Make capacity the next power of two
    while (newVertCapacity < vertexCount)
      newVertCapacity <<= 1;
  }
  else if (vertexCount < mVertexBufferCapacity>>1) {
    // Make capacity the previous power of two
    while (vertexCount < newVertCapacity>>1)
      newVertCapacity >>= 1;
  }
  
  if (newVertCapacity != mVertexBufferCapacity) 
  {
    mVertexBufferCapacity = newVertCapacity;
    
    // Create new vertex buffer
    mVertexBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        mRenderOp.vertexData->vertexDeclaration->getVertexSize(VertexDeclaration_Position),
        mVertexBufferCapacity,
        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
    
    // Bind buffer
    mRenderOp.vertexData->vertexBufferBinding->setBinding(VertexDeclaration_Position, mVertexBuffer);
    
    // Colour vertices.
    if (this->mProfile.usesColourVertices)
    {
     // Create new vertexColour buffer
     mVertexColourBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
         mRenderOp.vertexData->vertexDeclaration->getVertexSize(VertexDeclaration_Colour),
         mVertexBufferCapacity,
         Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
     
     // Bind buffer
     mRenderOp.vertexData->vertexBufferBinding->setBinding(VertexDeclaration_Colour, mVertexColourBuffer);
    }

    // Normals.
    if (this->mProfile.usesNormals)
    {
     // Create new vertexColour buffer
     mNormalBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
         mRenderOp.vertexData->vertexDeclaration->getVertexSize(VertexDeclaration_Normal),
         mVertexBufferCapacity,
         Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
     
     // Bind buffer
     mRenderOp.vertexData->vertexBufferBinding->setBinding(VertexDeclaration_Normal, mNormalBuffer);
    }

    // Texture coords.
    if (this->mProfile.usesTextureCoordinates)
    {
     // Create new vertexColour buffer
     mTextureCoordsBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
         mRenderOp.vertexData->vertexDeclaration->getVertexSize(VertexDeclaration_TextureCoordinates),
         mVertexBufferCapacity,
         Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
     
     // Bind buffer
     mRenderOp.vertexData->vertexBufferBinding->setBinding(VertexDeclaration_TextureCoordinates, mTextureCoordsBuffer);
    }

  }
  // Update vertex count in the render operation
  mRenderOp.vertexData->vertexCount = vertexCount;
  
  if (mProfile.usesIndexes)
  {
    OgreAssert(indexCount <= std::numeric_limits<unsigned short>::max(), "indexCount exceeds 16 bit");

    size_t newIndexCapacity = mIndexBufferCapacity;
    // Prepare index buffer
    if ((indexCount > newIndexCapacity) || (!newIndexCapacity))
    {
      // indexCount exceeds current capacity!
      // It is necessary to reallocate the buffer.
      // Check if this is the first call
      if (!newIndexCapacity)
        newIndexCapacity = 1;
      // Make capacity the next power of two
      while (newIndexCapacity < indexCount)
        newIndexCapacity <<= 1;
    }
    else if (indexCount < newIndexCapacity>>1) 
    {
      // Make capacity the previous power of two
      while (indexCount < newIndexCapacity>>1)
        newIndexCapacity >>= 1;
    }
    
    if (newIndexCapacity != mIndexBufferCapacity)
    {
      mIndexBufferCapacity = newIndexCapacity;
      // Create new index buffer
      mIndexBuffer = mRenderOp.indexData->indexBuffer =
        Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
          Ogre::HardwareIndexBuffer::IT_32BIT, //Ogre::HardwareIndexBuffer::IndexType(!mProfile.uses16BitIndexes),
          mIndexBufferCapacity,
          Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
    }
    
    // Update index count in the render operation
    mRenderOp.indexData->indexCount = indexCount;
  }
}

float Renderable::getBoundingRadius() const
{
 return Ogre::Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
}

float Renderable::getSquaredViewDepth(const Ogre::Camera* cam) const
{
 Ogre::Vector3 vMin, vMax, vMid, vDist;
 vMin = mBox.getMinimum();
 vMax = mBox.getMaximum();
 vMid = ((vMax - vMin) * 0.5) + vMin;
 vDist = cam->getDerivedPosition() - vMid;
 return vDist.squaredLength();
}

#undef ABORT_IF_EMPTY

                                                                                       

} // namespace

                                                                                       
