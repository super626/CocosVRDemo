//
//  HeadTransform.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#include "HeadTransform.h"

HeadTransform::HeadTransform()
{
    this->headView = cocos2d::Mat4::IDENTITY;
}

void HeadTransform::setHeadView(const cocos2d::Mat4& headview)
{
    this->headView = headview;
}

const cocos2d::Mat4& HeadTransform::getHeadView() const
{
    return this->headView;
}

cocos2d::Vec3 HeadTransform::getTranslation()
{
    return cocos2d::Vec3(this->headView.m[12], this->headView.m[13], this->headView.m[14]);
}

cocos2d::Vec3 HeadTransform::getForwardVector()
{
    return cocos2d::Vec3(-this->headView.m[8], -this->headView.m[9], -this->headView.m[10]);
}

cocos2d::Vec3 HeadTransform::getUpVector()
{
    return cocos2d::Vec3(this->headView.m[4], this->headView.m[5], this->headView.m[6]);
}

cocos2d::Vec3 HeadTransform::getRightVector()
{
    return cocos2d::Vec3(this->headView.m[0], this->headView.m[1], this->headView.m[2]);
}

cocos2d::Quaternion HeadTransform::getQuaternion()
{
    float t = this->headView.m[0] + this->headView.m[5] + this->headView.m[10];
    float s, w, x, y, z;
    if (t >= 0.0f) {
        s = sqrtf(t + 1.0f);
        w = 0.5F * s;
        s = 0.5F / s;
        x = (this->headView.m[9] - this->headView.m[6]) * s;
        y = (this->headView.m[2] - this->headView.m[8]) * s;
        z = (this->headView.m[4] - this->headView.m[1]) * s;
    } else {
        if ((this->headView.m[0] > this->headView.m[5]) && (this->headView.m[0] > this->headView.m[10])) {
            s = sqrtf(1.0f + this->headView.m[0] - this->headView.m[5] - this->headView.m[10]);
            x = s * 0.5f;
            s = 0.5f / s;
            y = (this->headView.m[4] + this->headView.m[1]) * s;
            z = (this->headView.m[2] + this->headView.m[8]) * s;
            w = (this->headView.m[9] - this->headView.m[6]) * s;
        } else {
            if (this->headView.m[5] > this->headView.m[10]) {
                s = sqrtf(1.0f + this->headView.m[5] - this->headView.m[0] - this->headView.m[10]);
                y = s * 0.5f;
                s = 0.5f / s;
                x = (this->headView.m[4] + this->headView.m[1]) * s;
                z = (this->headView.m[9] + this->headView.m[6]) * s;
                w = (this->headView.m[2] - this->headView.m[8]) * s;
            } else {
                s = sqrtf(1.0f + this->headView.m[10] - this->headView.m[0] - this->headView.m[5]);
                z = s * 0.5f;
                s = 0.5f / s;
                x = (this->headView.m[2] + this->headView.m[8]) * s;
                y = (this->headView.m[9] + this->headView.m[6]) * s;
                w = (this->headView.m[4] - this->headView.m[1]) * s;
            }
        }
    }
    return cocos2d::Quaternion(x, y, z, w);
}

cocos2d::Vec3 HeadTransform::getEulerAngles()
{
    float yaw, roll, pitch = asinf(this->headView.m[6]);
    if (sqrtf(1.0f - this->headView.m[6] * this->headView.m[6]) >= 0.01f) {
        yaw = atan2f(-this->headView.m[2], this->headView.m[10]);
        roll = atan2f(-this->headView.m[4], this->headView.m[5]);
    } else {
        yaw = 0.0f;
        roll = atan2f(this->headView.m[1], this->headView.m[0]);
    }
    return cocos2d::Vec3(-pitch, -yaw, -roll);
}