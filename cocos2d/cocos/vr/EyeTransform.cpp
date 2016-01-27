//
//  EyeTransform.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#include "EyeTransform.h"

EyeTransform::EyeTransform(EyeParams *params)
{
    this->eyeParams = params;
    this->eyeView = cocos2d::Mat4::IDENTITY;
    this->perspective = cocos2d::Mat4::IDENTITY;
}

const cocos2d::Mat4& EyeTransform::getEyeView() const
{
    return this->eyeView;
}

void EyeTransform::setEyeView(const cocos2d::Mat4& eyeView)
{
    this->eyeView = eyeView;
}

const cocos2d::Mat4& EyeTransform::getPerspective() const
{
    return this->perspective;
}

void EyeTransform::setPerspective(const cocos2d::Mat4& perspective)
{
    this->perspective = perspective;
}

EyeParams* EyeTransform::getParams()
{
    return this->eyeParams;
}
