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

                                                                                       

#ifndef CRITTER_CHARACTERINPUT_H
#define CRITTER_CHARACTERINPUT_H

#include "CritterStable.h"

#if NxOgreHasCharacterController == 1

#include "CritterCommon.h"

                                                                                       

namespace Critter
{

struct CharacterInput
{
 char forward_backward : 8;
 char left_right : 8;
 unsigned char up : 1;
 unsigned char down : 1;
 unsigned char modifiers : 6;
 unsigned char user : 8;
};

struct CharacterInputHelper
{
 
 CharacterInputHelper();
 
 void reset();
 
 void forward(unsigned char = 127);
 
 void forwardFractional(Ogre::Real decimal);
 
 void backward(unsigned char = 127);
 
 void backwardFractional(Ogre::Real decimal);
 
 void left(unsigned char = 127);
 
 void leftFractional(Ogre::Real decimal);
 
 void right(unsigned char = 127);
 
 void rightFractional(Ogre::Real decimal);
 
 void up(bool = true);
  
 void down(bool = true);
 
 CharacterInput input;
 
};

} // namespace

                                                                                       

#endif
#endif
