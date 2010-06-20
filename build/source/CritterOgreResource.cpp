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
#include "CritterOgreResource.h"

                                                                                       

namespace Critter
{

                                                                                       

OgreResource::OgreResource(const NxOgre::Path& path, NxOgre::ResourceProtocol* protocol, NxOgre::Enums::ResourceAccess ra)
: Resource(path, protocol, ra)
{
}

OgreResource::~OgreResource()
{
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened)
  close();

}

NxOgre::Enums::ResourceAccess OgreResource::getAccess() const
{
 return mAccess;
}

NxOgre::Enums::ResourceStatus OgreResource::getStatus() const
{
 return mStatus;
}

NxOgre::Enums::ResourceDirectionality OgreResource::getDirectionality() const
{
 return mDirectionality;
}

void OgreResource::open()
{
 
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened)
  close();
  
 mStatus = NxOgre::Enums::ResourceStatus_Opening;
 mDirectionality = NxOgre::Enums::ResourceDirectionality_Unknown;
 mNbReadBytes = 0;
 mNbWroteBytes = 0;

 std::string resourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
 
 if (mPath.getNbDirectories() == 1)
  resourceGroup = mPath.getDirectory(0);

 NxOgre_DebugPrint_Resources("Opening '" << mPath.getString() << "' as ReadWrite (forced).");
 mStream = Ogre::ResourceGroupManager::getSingletonPtr()->openResource(mPath.getFilename(), resourceGroup);
 
 if (mStream.isNull() || (!mStream->isReadable() && mStream->isWriteable()))
 {
  NxOgre_ThrowException(NxOgre::PathInvalidException, NxOgre::Reason::Exceptionise(mPath.getString(), mStatus), Critter::Enums::_OgreResource);
  return;
 }
 
 mStatus = NxOgre::Enums::ResourceStatus_Opened;
 mDirectionality = NxOgre::Enums::ResourceDirectionality_Omidirectional;
 
}

void OgreResource::close()
{
 mStatus = NxOgre::Enums::ResourceStatus_Closing;
 mDirectionality = NxOgre::Enums::ResourceDirectionality_Unknown;
 if (mStream.isNull() == false)
  mStream->close();
 
 mStatus = NxOgre::Enums::ResourceStatus_Closed;

 NxOgre_DebugPrint_Resources("Closing '" << mPath.getString() << "', Read " << mNbReadBytes << "b and Wrote " << mNbWroteBytes << "b. ");
 
}

size_t OgreResource::getSize() const
{
 if (mStatus != NxOgre::Enums::ResourceStatus_Opened)
  return 0;
 
 return mStream->size();
}

bool OgreResource::seek(size_t to)
{
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened && mDirectionality == NxOgre::Enums::ResourceDirectionality_Omidirectional)
 {
  mStream->seek(to);
  return true;
 }
 return false;
}

bool OgreResource::seekBeginning()
{
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened && mDirectionality == NxOgre::Enums::ResourceDirectionality_Omidirectional)
 {
  mStream->seek(0);
  return true;
 }
 return false;
}

bool OgreResource::seekEnd()
{
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened && mDirectionality == NxOgre::Enums::ResourceDirectionality_Omidirectional)
 {
  mStream->seek(mStream->size());
  return true;
 }
 return false;
}

bool OgreResource::atBeginning() const
{
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened && mDirectionality == NxOgre::Enums::ResourceDirectionality_Omidirectional)
 {
  return mStream->tell() == 0;
 }
 return false;
}

bool OgreResource::atEnd() const
{
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened && mDirectionality == NxOgre::Enums::ResourceDirectionality_Omidirectional)
 {
  return mStream->eof();
 }
 return false;
}

size_t OgreResource::at() const
{
 if (mStatus == NxOgre::Enums::ResourceStatus_Opened && mDirectionality == NxOgre::Enums::ResourceDirectionality_Omidirectional)
 {
  return mStream->tell();
 }
 return NxOgre::Constants::ResourceSizeUnknown;
}

bool OgreResource::write(const void* src, size_t src_size)
{
 mNbWroteBytes += src_size;
 return mStream->write(src, src_size) == src_size;
}

bool OgreResource::writeNull()
{
 mNbWroteBytes += sizeof(char);
 return mStream->write(&NULL_BYTE, sizeof(char)) == sizeof(char);
}

bool OgreResource::writeBool(bool v)
{
 mNbWroteBytes += sizeof(bool);
 return mStream->write(&v, sizeof(bool)) == sizeof(bool);
}

bool OgreResource::writeBool(bool* v, size_t length)
{
 mNbWroteBytes += sizeof(bool) * length;
 return mStream->write(&v, sizeof(bool) * length) == (sizeof(bool) * length);
}

bool OgreResource::writeUChar(unsigned char v)
{
 mNbWroteBytes += sizeof(unsigned char);
 return mStream->write(&v, sizeof(unsigned char)) == sizeof(unsigned char);
}

bool OgreResource::writeUChar(unsigned char* v, size_t length)
{
 mNbWroteBytes += sizeof(unsigned char) * length;
 return mStream->write(&v, sizeof(unsigned char) * length) == sizeof(unsigned char) * length;
}

bool OgreResource::writeChar(char v)
{
 mNbWroteBytes += sizeof(char);
 return mStream->write(&v, sizeof(char)) == sizeof(char);
}

bool OgreResource::writeChar(char* v, size_t length)
{
 mNbWroteBytes += sizeof(unsigned char) * length;
 return mStream->write(&v, sizeof(unsigned char) * length) == sizeof(unsigned char) * length;
}

bool OgreResource::writeUShort(unsigned short v)
{
 mNbWroteBytes += sizeof(unsigned short);
 return mStream->write(&v, sizeof(unsigned short)) == 1;
}

bool OgreResource::writeUShort(unsigned short* v, size_t length)
{
 mNbWroteBytes += sizeof(unsigned short) * length;
 return mStream->write(&v, sizeof(unsigned short) * length) == sizeof(unsigned short) * length;
}

bool OgreResource::writeShort(short v)
{
 mNbWroteBytes += sizeof(short);
 return mStream->write(&v, sizeof(short)) == sizeof(short);
}

bool OgreResource::writeShort(short* v, size_t length)
{
 mNbWroteBytes += sizeof(short) * length;
 return mStream->write(&v, sizeof(short) * length) == sizeof(short) * length;
}

bool OgreResource::writeUInt(unsigned int v)
{
 mNbWroteBytes += sizeof(unsigned int);
 return mStream->write(&v, sizeof(unsigned int)) == sizeof(unsigned int);
}

bool OgreResource::writeUInt(unsigned int* v, size_t length)
{
 mNbWroteBytes += sizeof(unsigned int) * length;
 return mStream->write(&v, sizeof(unsigned int) * length) == sizeof(unsigned int) * length;
}

bool OgreResource::writeInt(int v)
{
 mNbWroteBytes += sizeof(int);
 return mStream->write(&v, sizeof(int)) == sizeof(int);
}

bool OgreResource::writeInt(int* v, size_t length)
{
 mNbWroteBytes += sizeof(int) * length;
 return mStream->write(&v, sizeof(int) * length) == sizeof(int) * length;
}

bool OgreResource::writeFloat(float v)
{
 mNbWroteBytes += sizeof(float);
 return mStream->write(&v, sizeof(float)) == sizeof(float);
}

bool OgreResource::writeFloat(float* v, size_t length)
{
 mNbWroteBytes += sizeof(float) * length;
 return mStream->write(&v, sizeof(float) * length) == sizeof(float) * length;
}

bool OgreResource::writeDouble(double v)
{
 mNbWroteBytes += sizeof(double);
 return mStream->write(&v, sizeof(double)) == sizeof(double);
}

bool OgreResource::writeDouble(double* v, size_t length)
{
 mNbWroteBytes += sizeof(double) * length;
 return mStream->write(&v, sizeof(double) * length) == sizeof(double) * length;
}

bool OgreResource::writeReal(NxOgreRealType v)
{
 mNbWroteBytes += sizeof(NxOgreRealType);
 return mStream->write(&v, sizeof(NxOgreRealType)) == sizeof(NxOgreRealType);
}

bool OgreResource::writeReal(NxOgreRealType* v, size_t length)
{
 mNbWroteBytes += sizeof(NxOgreRealType) * length;
 return mStream->write(&v, sizeof(NxOgreRealType) * length) == sizeof(NxOgreRealType) * length;
}

bool OgreResource::writeLong(long v)
{
 // printf(__FUNCTION__ "at %i\n", at());
 
 mNbWroteBytes += sizeof(long);
 return mStream->write(&v, sizeof(long)) == sizeof(long);
}

bool OgreResource::writeLong(long* v, size_t length)
{
 mNbWroteBytes += sizeof(long) * length;
 return mStream->write(&v, sizeof(long) * length) == sizeof(long) * length;
}

bool OgreResource::writeString(const NxOgre::String& str)
{
 mNbWroteBytes +=  str.length() + 1;
 return mStream->write(str.data(), str.length()) == (str.length() + 1);
}

bool OgreResource::readBool()
{
 mNbReadBytes+=sizeof(bool);
 bool t=0;mStream->read(&t, sizeof(bool));
 return t;
}

void OgreResource::readBoolArray(bool* t, size_t length)
{
 mNbReadBytes+=sizeof(bool) * length;
 mStream->read(t, sizeof(bool) * length);
}

unsigned char OgreResource::readUChar()
{
 mNbReadBytes+=sizeof(unsigned char);
 unsigned char t=0;mStream->read(&t, sizeof(unsigned char));
 return t;
}

void OgreResource::readUCharArray(unsigned char* t, size_t length)
{
 mNbReadBytes+=sizeof(unsigned char) * length;
 mStream->read(t, sizeof(unsigned char) * length);
}

char OgreResource::readChar()
{
 mNbReadBytes+=sizeof(char);
 char t=0;mStream->read(&t, sizeof(char));
 return t;
}

void OgreResource::readCharArray(char* t, size_t length)
{
 mNbReadBytes+=sizeof(char) * length;
 mStream->read(t, sizeof(char) * length);
}

unsigned short OgreResource::readUShort()
{
 mNbReadBytes+=sizeof(unsigned short);
 unsigned short t=0;mStream->read(&t, sizeof(unsigned short));
 return t;
}

void OgreResource::readUShortArray(unsigned short* t, size_t length)
{
 mNbReadBytes+=sizeof(unsigned short) * length;
 mStream->read(t, sizeof(unsigned short) * length);
}

short OgreResource::readShort()
{
 mNbReadBytes+=sizeof(short);
 short t=0;mStream->read(&t, sizeof(short));
 return t;
}

void OgreResource::readShortArray(short* t, size_t length)
{
 mNbReadBytes+=sizeof(short) * length;
 mStream->read(t, sizeof(short) * length);
}

unsigned int OgreResource::readUInt()
{
 mNbReadBytes+=sizeof(unsigned int);
 unsigned int t=0;mStream->read(&t, sizeof(unsigned int));
 return t;
}

void OgreResource::readUIntArray(unsigned int* t, size_t length)
{
 mNbReadBytes+=sizeof(unsigned int) * length;
 mStream->read(t, sizeof(unsigned int) * length);
}

int OgreResource::readInt()
{
 mNbReadBytes+=sizeof(int);
 int t=0;mStream->read(&t, sizeof(int));
 return t;
}

void OgreResource::readIntArray(int* t, size_t length)
{
 mNbReadBytes+=sizeof(int) * length;
 mStream->read(t, sizeof(int) * length);
}

float OgreResource::readFloat()
{
 mNbReadBytes+=sizeof(float);
 float t=0;mStream->read(&t, sizeof(float));
 return t;
}

void OgreResource::readFloatArray(float* t, size_t length)
{
 mNbReadBytes+=sizeof(float) * length;
 mStream->read(t, sizeof(float) * length);
}

double OgreResource::readDouble()
{
 mNbReadBytes+=sizeof(double);
 double t=0;mStream->read(&t, sizeof(double));
 return t;
}

void OgreResource::readDoubleArray(double* t, size_t length)
{
 mNbReadBytes+=sizeof(double) *length;
 mStream->read(t, sizeof(double) * length);
}

NxOgre::Real OgreResource::readReal()
{
 mNbReadBytes+=sizeof(NxOgreRealType);
 NxOgreRealType t=0;mStream->read(&t, sizeof(NxOgreRealType));
 return t;
}

void OgreResource::readRealArray(NxOgreRealType* t, size_t length)
{
 mNbReadBytes+=sizeof(NxOgreRealType) * length;
 mStream->read(t, sizeof(NxOgreRealType) * length);
}

long OgreResource::readLong()
{
 mNbReadBytes+=sizeof(long);
 long t=0;mStream->read(&t, sizeof(long));
 return t;
}

void OgreResource::readLongArray(long* t, size_t length)
{
 mNbReadBytes+=sizeof(long) * length;
 mStream->read(t, sizeof(long) * length);
}

void OgreResource::flush()
{//
}

                                                                                       

} // namespace

                                                                                       

