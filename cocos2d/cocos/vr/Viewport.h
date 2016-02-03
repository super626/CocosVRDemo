/**
 Copyright (c) 2014 Peter Tribe.
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Original file from Peter Tribe
 
 This file was modified to fit the cocos2d-x project by super626
 */

#ifndef __CardboardVR_iOS__Viewport__
#define __CardboardVR_iOS__Viewport__


#include <string>

class Viewport
{
public:
    int x;
    int y;
    int width;
    int height;
public:
    void setViewport(int x, int y, int width, int height);
    void setGLViewport();
    void setGLScissor();
    std::string toString();
};

#endif 