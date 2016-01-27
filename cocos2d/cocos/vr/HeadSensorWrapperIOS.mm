//
//
//

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "HeadSensorWrapper.h"
#include "MagnetSensor.h"
#include "HeadTracker.h"


HeadSensorWrapper::HeadSensorWrapper()
{
    _magnetSensor = new MagnetSensor();
    _headTracker = new HeadTracker();
}

HeadSensorWrapper::~HeadSensorWrapper()
{
    delete _magnetSensor;
    delete _headTracker;
}

void HeadSensorWrapper::startTracking()
{
    _isTracking = true;
    
    _headTracker->startTracking();
    _magnetSensor->start();
}

void HeadSensorWrapper::stopTracking()
{
    _isTracking = false;
    
    _headTracker->stopTracking();
    _magnetSensor->stop();

}

const cocos2d::Mat4& HeadSensorWrapper::getLastHeadView()
{
    _headView = _headTracker->getLastHeadView();
    
    return _headView;
}

void HeadSensorWrapper::setHeadView(const cocos2d::Mat4& headView)
{
    _headView = headView;
}
#endif
