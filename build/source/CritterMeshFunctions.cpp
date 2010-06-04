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
#include "CritterMeshFunctions.h"

                                                                                       


namespace Critter
{

                                                                                       

NxOgre::MeshData* MeshFunctions::read(Ogre::MeshPtr ogre_mesh)
{
 
 NxOgre::MeshData* mesh = new NxOgre::MeshData();
 
 bool added_shared = false;
 size_t current_offset = 0;
 size_t shared_offset = 0;
 size_t next_offset = 0;
 size_t index_offset = 0;

 added_shared = false;

 // Run through the submeshes again, adding the data into the arrays
 for (unsigned short i = 0; i < ogre_mesh->getNumSubMeshes(); ++i)
 {
     Ogre::SubMesh* submesh = ogre_mesh->getSubMesh(i);

     Ogre::VertexData* vertex_data = submesh->useSharedVertices ? ogre_mesh->sharedVertexData : submesh->vertexData;

     if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
     {
         if(submesh->useSharedVertices)
         {
             added_shared = true;
             shared_offset = current_offset;
         }

         const Ogre::VertexElement* posElem =
             vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

         Ogre::HardwareVertexBufferSharedPtr vbuf =
             vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

         unsigned char* vertex =
             static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

         // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
         //  as second argument. So make it float, to avoid trouble when Ogre::Real will
         //  be comiled/typedefed as double:
         //Ogre::Real* pReal;
         float* pReal;

         for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
         {
             posElem->baseVertexPointerToElement(vertex, &pReal);
             mesh->mVertices.push_back(pReal[0]);
             mesh->mVertices.push_back(pReal[1]);
             mesh->mVertices.push_back(pReal[2]);
         }
         
         vbuf->unlock();
         next_offset += vertex_data->vertexCount;
     }


     Ogre::IndexData* index_data = submesh->indexData;
     size_t numTris = index_data->indexCount / 3;
     Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
     
     bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

     unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
     unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

     size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

     if ( use32bitindexes )
     {
         for ( size_t k = 0; k < numTris*3; ++k)
         {
             mesh->mIndexes.push_back(static_cast<unsigned long>(offset));
         }
     }
     else
     {
         for ( size_t k = 0; k < numTris*3; ++k)
         {
          mesh->mIndexes.push_back(static_cast<unsigned long>(pShort[k]) +
                                       static_cast<unsigned long>(offset));
         }
     }

     ibuf->unlock();
     current_offset = next_offset;
 }


 return mesh;
}

                                                                                       

} // namespace

                                                                                       

