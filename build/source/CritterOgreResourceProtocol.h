/** 
    
    This ogre is part of Critter.
    
    Copyright (c) 2009 Robin Southern, http://www.nxogre.org
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation ogres (the "Software"), to deal
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

                                                                                       

#ifndef CRITTER_OGRERESOURCEPROTOCOL_H
#define CRITTER_OGRERESOURCEPROTOCOL_H

#include "CritterStable.h"
#include "CritterCommon.h"
#include "NxOgreResource.h"

                                                                                       

namespace Critter
{


/*! class. OgreResourceProtocol
    desc.
         Makes the Ogre resource system accessible through NxOgre, giving all the advantages of the ogretypes that Ogre supports including zip ogres. Platform independent.
    example.
         // To open a resource, you address it via the "ogre" protocol, then followed by the Resource Group and then the
         // ogre you want to access.
         Resource* resource = ResourceSystem::getSingleton()->open("ogre://General/mesh.nxs");
*/
class CritterPublicClass OgreResourceProtocol : public NxOgre::ResourceProtocol
{
  
  public: // Functions
  
  /*! constructor. OgreResourceProtocol
  */
  OgreResourceProtocol();
  
  /*! destructor. OgreResourceProtocol
  */
 ~OgreResourceProtocol();
  
  /*! function. getProtocol
      desc.
          Get the protocol "ogre"
      return.
          **String** -- The protocol, which will be "ogre".
  */
  NxOgre::String  getProtocol() const;
  
  /*! function. getProtocolHash
      desc.
          Get the protocol hash of "ogre"
      return.
          **String** -- Always "ogre" run through a hash function.
  */
  NxOgre::StringHash  getProtocolHash() const;
  
  /*! function. usesNamelessResource
      desc.
          Does the resources not use filenames?
      return
          **bool** -- Always returns false.
  */
  bool usesNamelessResources() const;
    
  protected: // Functions
  
  /*! function. open
  */
  NxOgre::Resource*  open(const NxOgre::Path&, NxOgre::Enums::ResourceAccess);
  
  /*! function. closeArchive
  */
  void  close(NxOgre::Resource*);

  /*! function. initialise
      desc.
          Required initialise function
  */
  void  initialise();
  
  protected: // Variables
  
  NxOgre::String                 mProtocolName;
  
  NxOgre::StringHash             mProtocolHash;
  
}; // class OgreResourceProtocol


} // namespace

                                                                                       

#endif
