//
//
//

#ifndef __HEAD_SENSOR_WRAPPER_IOS__
#define __HEAD_SENSOR_WRAPPER_IOS__


#include <string>
#include "math/Mat4.h"
#include "base/ccMacros.h"
#include "HeadSensorWrapper.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
class MagnetSensor;
class HeadTracker;
#endif

class HeadSensorWrapperIOS : public HeadSensorWrapper
{
public:
    HeadSensorWrapperIOS();
    virtual ~HeadSensorWrapperIOS();
    
    void startTracking();
    void stopTracking();
    const cocos2d::Mat4& getLastHeadView();
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