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

#include "CritterOgreResourceProtocol.h"
#include "CritterOgreResource.h"

                                                                                       

namespace Critter
{

                                                                                       

OgreResourceProtocol::OgreResourceProtocol(void)
{
 mProtocolName = "ogre";
 mProtocolHash = NxOgre::Strings::hash(mProtocolName);
}

OgreResourceProtocol::~OgreResourceProtocol(void)
{
}

NxOgre::Resource* OgreResourceProtocol::open(const NxOgre::Path& path, NxOgre::Enums::ResourceAccess access)
{
 OgreResource* resource = NXOGRE_NEW_NXOGRE(OgreResource)(path, this, access);
 addResource(resource);
 resource->open();
 return resource;
}

void OgreResourceProtocol::close(NxOgre::Resource* resource)
{
 OgreResource* oresource = static_cast<OgreResource*>(resource);
 oresource->close();
 removeResource(oresource); // Removing it will automatically delete it as well.
}

NxOgre::String OgreResourceProtocol::getProtocol(void) const
{
 return mProtocolName;
}

NxOgre::StringHash OgreResourceProtocol::getProtocolHash(void) const
{
 return mProtocolHash;
}

bool OgreResourceProtocol::usesNamelessResources(void) const
{
 return false;
}

void OgreResourceProtocol::initialise(void)
{
}


                                                                                       

} // namespace

                                                                                       

