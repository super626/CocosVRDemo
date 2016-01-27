//
//  EyeTransform.h
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#ifndef __CardboardVR_iOS__EyeTransform__
#define __CardboardVR_iOS__EyeTransform__

#include "math/Mat4.h"

class EyeParams;

class EyeTransform
{
public:
    EyeTransform(EyeParams *params);
    const cocos2d::Mat4& getEyeView() const;
    void setEyeView(const cocos2d::Mat4& eyeView);
    const cocos2d::Mat4& getPerspective() const;
    void setPerspective(const cocos2d::Mat4& perspective);
    EyeParams* getParams();
private:
    EyeParams *eyeParams;
    cocos2d::Mat4 eyeView;
    cocos2d::Mat4 perspective;
};

#endif 