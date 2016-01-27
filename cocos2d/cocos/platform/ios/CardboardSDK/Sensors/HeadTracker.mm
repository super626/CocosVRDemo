//
//  HeadTracker.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-25.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#include "HeadTracker.h"

HeadTracker::HeadTracker()
{
    this->manager = [[CMMotionManager alloc] init];

    this->referenceTimestamp = 0;
    this->lastGyroEventTimestamp = 0;
    // this->tracker = new OrientationEKF();
    // the inertial reference frame has z up and x forward, while the world has z out and x right
    this->worldToInertialReferenceFrame = this->getRotateEulerMatrix(-90.f, 0.f, 90.f);
    // this assumes the device is landscape with the home button on the right
    this->deviceToDisplay = this->getRotateEulerMatrix(0.f, 0.f, -90.f);
}

HeadTracker::~HeadTracker()
{
    // delete this->tracker;
}

cocos2d::Mat4 HeadTracker::getRotateEulerMatrix(float x, float y, float z)
{
    x *= (float)(M_PI / 180.0f);
    y *= (float)(M_PI / 180.0f);
    z *= (float)(M_PI / 180.0f);
    float cx = (float) cos(x);
    float sx = (float) sin(x);
    float cy = (float) cos(y);
    float sy = (float) sin(y);
    float cz = (float) cos(z);
    float sz = (float) sin(z);
    float cxsy = cx * sy;
    float sxsy = sx * sy;
    cocos2d::Mat4 matrix;
    matrix.m[0] = cy * cz;
    matrix.m[1] = -cy * sz;
    matrix.m[2] = sy;
    matrix.m[3] = 0.0f;
    matrix.m[4] = cxsy * cz + cx * sz;
    matrix.m[5] = -cxsy * sz + cx * cz;
    matrix.m[6] = -sx * cy;
    matrix.m[7] = 0.0f;
    matrix.m[8] = -sxsy * cz + sx * sz;
    matrix.m[9] = sxsy * sz + sx * cz;
    matrix.m[10] = cx * cy;
    matrix.m[11] = 0.0f;
    matrix.m[12] = 0.0f;
    matrix.m[13] = 0.0f;
    matrix.m[14] = 0.0f;
    matrix.m[15] = 1.0f;
    return matrix;
}

void HeadTracker::startTracking()
{
    // this->tracker->reset();
    
    if (this->manager.isDeviceMotionAvailable && !this->manager.isDeviceMotionActive)
    {
        [this->manager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryZVertical];
    }
}

void HeadTracker::stopTracking()
{
    [this->manager stopDeviceMotionUpdates];
}

cocos2d::Mat4 HeadTracker::glMatrixFromRotationMatrix(CMRotationMatrix rotationMatrix)
{
    cocos2d::Mat4 glRotationMatrix;
    
    glRotationMatrix.m[0] = rotationMatrix.m11;
    glRotationMatrix.m[1] = rotationMatrix.m12;
    glRotationMatrix.m[2] = rotationMatrix.m13;
    glRotationMatrix.m[3] = 0.0f;
    
    glRotationMatrix.m[4] = rotationMatrix.m21;
    glRotationMatrix.m[5] = rotationMatrix.m22;
    glRotationMatrix.m[6] = rotationMatrix.m23;
    glRotationMatrix.m[7] = 0.0f;
    
    glRotationMatrix.m[8] = rotationMatrix.m31;
    glRotationMatrix.m[9] = rotationMatrix.m32;
    glRotationMatrix.m[10] = rotationMatrix.m33;
    glRotationMatrix.m[11] = 0.0f;

    glRotationMatrix.m[12] = 0.0f;
    glRotationMatrix.m[13] = 0.0f;
    glRotationMatrix.m[14] = 0.0f;
    glRotationMatrix.m[15] = 1.0f;

    return glRotationMatrix;
}

cocos2d::Mat4 HeadTracker::getLastHeadView()
{
//    if (this->referenceTimestamp == 0)
//    {
//        return GLKMatrix4Identity;
//    }
//    
//    double secondsSinceLastGyroEvent = [[NSDate date] timeIntervalSinceReferenceDate] - this->referenceTimestamp - this->lastGyroEventTimestamp;
//    // NSLog(@"%f", secondsSinceLastGyroEvent);
//    double secondsToPredictForward = secondsSinceLastGyroEvent + 0.03333333333333333;
//    GLKMatrix4 tempHeadView = this->tracker->getPredictedGLMatrix(secondsToPredictForward);
    CMDeviceMotion *motion = this->manager.deviceMotion;
    
    // NSLog(@"%.3f %.3f %.3f", motion.attitude.roll, motion.attitude.pitch, motion.attitude.yaw);
    
    CMRotationMatrix rotationMatrix = motion.attitude.rotationMatrix;
    cocos2d::Mat4 inertialReferenceFrameToDevice;
    inertialReferenceFrameToDevice.m[0] = rotationMatrix.m11;
    inertialReferenceFrameToDevice.m[1] = rotationMatrix.m21;
    inertialReferenceFrameToDevice.m[2] = rotationMatrix.m31;
    
    inertialReferenceFrameToDevice.m[4] = rotationMatrix.m12;
    inertialReferenceFrameToDevice.m[5] = rotationMatrix.m22;
    inertialReferenceFrameToDevice.m[6] = rotationMatrix.m32;
    
    inertialReferenceFrameToDevice.m[8] = rotationMatrix.m13;
    inertialReferenceFrameToDevice.m[9] = rotationMatrix.m23;
    inertialReferenceFrameToDevice.m[10] = rotationMatrix.m33;
    
    cocos2d::Mat4 worldToDevice;
    cocos2d::Mat4::multiply(inertialReferenceFrameToDevice, worldToInertialReferenceFrame, &worldToDevice);
    
    cocos2d::Mat4 worldToDisplay = (deviceToDisplay * worldToDevice);
    // GLKMatrix4 outMatrix = GLKMatrix4Multiply(glRotationMatrix, this->ekfToHeadTracker);
    // NSLog(@"%@", NSStringFromGLKMatrix4(outMatrix));
    return worldToDisplay;
}