//
//  CardboardDeviceParams.h
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#ifndef __CardboardVR_iOS__CardboardDeviceParams__
#define __CardboardVR_iOS__CardboardDeviceParams__

#include "Distortion.h"
#include <string>

class CardboardDeviceParams
{
public:
    CardboardDeviceParams();
    CardboardDeviceParams(CardboardDeviceParams* params);
    ~CardboardDeviceParams();
    void setVendor(const std::string& vendor);
    const std::string& getVendor() const;
    void setModel(const std::string& model);
    const std::string& getModel() const;
    void setVersion(const std::string& version);
    const std::string& getVersion() const;
    void setInterpupillaryDistance(float interpupillaryDistance);
    float getInterpupillaryDistance();
    void setVerticalDistanceToLensCenter(float verticalDistanceToLensCenter);
    float getVerticalDistanceToLensCenter();
    void setVisibleViewportSize(float visibleViewportSize);
    float getVisibleViewportSize();
    void setFovY(float fovY);
    float getFovY();
    void setLensDiameter(float lensDiameter);
    float getLensDiameter();
    void setScreenToLensDistance(float screenToLensDistance);
    float getScreenToLensDistance();
    void setEyeToLensDistance(float eyeToLensDistance);
    float getEyeToLensDistance();
    Distortion* getDistortion();
    bool equals(CardboardDeviceParams *other);
private:
    std::string vendor;
    std::string model;
    std::string version;
    float interpupillaryDistance;
    float verticalDistanceToLensCenter;
    float lensDiameter;
    float screenToLensDistance;
    float eyeToLensDistance;
    float visibleViewportSize;
    float fovY;
    Distortion *distortion;
};

#endif
