//
//
//

#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS

#include "HeadSensorWrapper.h"

HeadSensorWrapper::HeadSensorWrapper()
{

}

HeadSensorWrapper::~HeadSensorWrapper()
{

}

void HeadSensorWrapper::startTracking()
{
    _isTracking = true;
}

void HeadSensorWrapper::stopTracking()
{
    _isTracking = false;
}

const cocos2d::Mat4& HeadSensorWrapper::getLastHeadView()
{
    return _headView;
}

void HeadSensorWrapper::setHeadView(const cocos2d::Mat4& headView)
{
    _headView = headView;
}
#endif
