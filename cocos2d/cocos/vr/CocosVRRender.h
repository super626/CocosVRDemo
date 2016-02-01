
#ifndef __CardboardVR_Render__
#define __CardboardVR_Render__

#include "math/Mat4.h"

class MagnetSensor;
class HeadTracker;
class HeadTransform;
class HeadMountedDisplay;
class EyeParams;
class DistortionRenderer;
class FieldOfView;
class VRRenderer;
class HeadSensorWrapper;
class HeadTransform;

class CocosVRRender
{
public:
    CocosVRRender();
    ~CocosVRRender();
    
    void startHeadTracking();
    
    void stopHeadTracking();
    
    bool isVRModeEnable() { return isVRModeEnabled; }
    
    void setVRModeEnable(bool isEnable);
    
    void render();
    
    const float* getHeadEuler();
    
    
    void setHeadView(const cocos2d::Mat4& headview);
    
protected:
    
    void updateFovs(FieldOfView* left, FieldOfView* right);
    
    HeadSensorWrapper* headSensor;
    HeadTransform *headTransform;
    HeadMountedDisplay *headMountedDisplay;
    
    EyeParams *monocularParams;
    EyeParams *leftEyeParams;
    EyeParams *rightEyeParams;
    
    DistortionRenderer *distortionRenderer;
    VRRenderer* renderer;
    
    bool distortionCorrectionEnabled;
    float distortionCorrectionScale;
    
    float zNear;
    float zFar;
    
    float headEuler[3];
    
    bool isVRModeEnabled;
    bool projectionChanged;
};
#endif
