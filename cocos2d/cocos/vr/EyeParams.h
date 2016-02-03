/**
 Copyright (c) 2014 Peter Tribe.
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Original file from Peter Tribe
 
 This file was modified to fit the cocos2d-x project by super626
 */


#ifndef __CardboardVR_iOS__EyeParams__
#define __CardboardVR_iOS__EyeParams__

#include "Viewport.h"
#include "FieldOfView.h"
#include "EyeTransform.h"

typedef enum
{
    EyeParamsTypeMonocular = 0,
    EyeParamsTypeLeft = 1,
    EyeParamsTypeRight = 2
} EyeParamsType;

class EyeParams
{
public:
    EyeParams(EyeParamsType eye);
    ~EyeParams();
    EyeParamsType getEye();
    Viewport* getViewport();
    FieldOfView* getFov();
    EyeTransform* getTransform();
private:
    EyeParamsType eye;
    Viewport *viewport;
    FieldOfView *fov;
    EyeTransform *eyeTransform;
};

#endif