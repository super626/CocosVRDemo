//
//  HeadTracker.h
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-25.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#ifndef __CardboardVR_iOS__HeadTracker__
#define __CardboardVR_iOS__HeadTracker__

#import <CoreMotion/CoreMotion.h>
#import <GLKit/GLKit.h>
#include <vector>
#include "Structs.h"
#include "OrientationEKF.h"
#include "math/Mat4.h"

class HeadTracker
{
public:
    HeadTracker();
    ~HeadTracker();
    void startTracking();
    void stopTracking();
    cocos2d::Mat4 getLastHeadView();
private:
    cocos2d::Mat4 glMatrixFromRotationMatrix(CMRotationMatrix rotationMatrix);
    cocos2d::Mat4 getRotateEulerMatrix(float x, float y, float z);
    CMMotionManager *manager;
    // OrientationEKF *tracker;
    cocos2d::Mat4 deviceToDisplay;
    cocos2d::Mat4 worldToInertialReferenceFrame;
    NSTimeInterval referenceTimestamp;
    NSTimeInterval lastGyroEventTimestamp;
};

#endif
