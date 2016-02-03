/**
 Copyright (c) 2014 Peter Tribe.
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Original file from Peter Tribe
 
 This file was modified to fit the cocos2d-x project by super626
 */


#ifndef __CardboardVR_iOS__HeadTransform__
#define __CardboardVR_iOS__HeadTransform__

#include "math/CCMath.h"

class HeadTransform
{
public:
    HeadTransform();
    void setHeadView(const cocos2d::Mat4& headView);
    const cocos2d::Mat4& getHeadView() const;
    cocos2d::Vec3 getTranslation();
    cocos2d::Vec3 getForwardVector();
    cocos2d::Vec3 getUpVector();
    cocos2d::Vec3 getRightVector();
    cocos2d::Quaternion getQuaternion();
    cocos2d::Vec3 getEulerAngles();
private:
    cocos2d::Mat4 headView;
};

#endif 
