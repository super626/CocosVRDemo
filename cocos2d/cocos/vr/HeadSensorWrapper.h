/**
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Created by super626
 */


#ifndef __HEAD_SENSOR_WRAPPER__
#define __HEAD_SENSOR_WRAPPER__


#include <string>
#include "math/Mat4.h"
#include "base/ccMacros.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
class MagnetSensor;
class HeadTracker;
#endif

class HeadSensorWrapper
{
public:
    HeadSensorWrapper();
    virtual ~HeadSensorWrapper();
    
    virtual void startTracking();
    virtual void stopTracking();
    virtual const cocos2d::Mat4& getLastHeadView();
    void setHeadView(const cocos2d::Mat4& headView);
    
    bool isTracking() const { return _isTracking; }
    
protected:
    bool _isTracking;
    cocos2d::Mat4 _headView;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    MagnetSensor* _magnetSensor;
    HeadTracker* _headTracker;
#endif

};

#endif 