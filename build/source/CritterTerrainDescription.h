/** File: OGRE3DRenderable.h
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
    
    Please Note:
    
    This code and the class known as "OGRE3DRenderable" was derived from the DynamicRenderable class
    posted on the Ogre3D Wiki http://www.ogre3d.org/wiki/index.php/DynamicGrowingBuffers
*/

#ifndef OGRE3D_RENDERABLE_H
#define OGRE3D_RENDERABLE_H

                                                                                       

#include "NxOgre.h"
#include "CritterCommon.h"
#include <OgreSimpleRenderable.h>

                                                                                       

#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 7

#include <OGRE/Terrain/OgreTerrain.h>

/*! class. OgreTerrainDescription
    
*/
class OGRE3DExportClass OgreTerrainDescription
{
  
  public:
  
  /*! constructor. OgreTerrainDescription
      desc.
  */
  OgreTerrainDescription();

  /*! constructor. OgreTerrainDescription
      desc.
  */
  OgreTerrainDescription(Ogre::Terrain*);

  /*! destructor.
      desc.
          Required destructor
  */
  ~OgreTerrainDescription();
  
  void reset();
  
  NxOgre::String  mName;
  
  float*          mHeightfieldData;
  
  size_t          mHeightfieldDataLength;
  
  float           mWorldSize;
  
  float           mMinHeight;
  
  float           mMaxHeight;
  
  NxOgre::Vec3    mPosition;
  
};
#endif

                                                                                       

#endif
