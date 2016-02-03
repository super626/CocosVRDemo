/**
 Copyright (c) 2014 Peter Tribe.
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Original file from Peter Tribe
 
 This file was modified to fit the cocos2d-x project by super626
 */


#ifndef __CardboardVR_iOS__FieldOfView__
#define __CardboardVR_iOS__FieldOfView__

#include <string>
#include "math/Mat4.h"

class FieldOfView
{
public:
    FieldOfView();
    FieldOfView(float left, float right, float bottom, float top);
    FieldOfView(FieldOfView *other);
    void setLeft(float left);
    float getLeft();
    void setRight(float right);
    float getRight();
    void setBottom(float bottom);
    float getBottom();
    void setTop(float top);
    float getTop();
    cocos2d::Mat4 toPerspectiveMatrix(float near, float far);
    bool equals(FieldOfView *other);
    std::string toString();
private:
    float left;
    float right;
    float bottom;
    float top;
private:
    cocos2d::Mat4 frustumM(float left, float right, float bottom, float top, float near, float far);
};

#endif 
