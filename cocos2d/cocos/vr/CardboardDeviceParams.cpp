//
//  CardboardDeviceParams.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#include "CardboardDeviceParams.h"

CardboardDeviceParams::CardboardDeviceParams()
{
    this->vendor = "com.google";
    this->model = "cardboard";
    this->version = "1.0";
    
    this->interpupillaryDistance = 0.06F;
    this->verticalDistanceToLensCenter = 0.035F;
    this->lensDiameter = 0.025F;
    this->screenToLensDistance = 0.037F;
    this->eyeToLensDistance = 0.011F;
    
    this->visibleViewportSize = 0.06F;
    this->fovY = 65.0F;
    
    this->distortion = new Distortion();
}

CardboardDeviceParams::CardboardDeviceParams(CardboardDeviceParams* params)
{
    this->vendor = params->vendor;
    this->model = params->model;
    this->version = params->version;
    
    this->interpupillaryDistance = params->interpupillaryDistance;
    this->verticalDistanceToLensCenter = params->verticalDistanceToLensCenter;
    this->lensDiameter = params->lensDiameter;
    this->screenToLensDistance = params->screenToLensDistance;
    this->eyeToLensDistance = params->eyeToLensDistance;
    
    this->visibleViewportSize = params->visibleViewportSize;
    this->fovY = params->fovY;
    
    this->distortion = new Distortion(params->distortion);
}

CardboardDeviceParams::~CardboardDeviceParams()
{
    delete this->distortion;
}

void CardboardDeviceParams::setVendor(const std::string& vendor_)
{
    this->vendor = vendor_;
}

const std::string& CardboardDeviceParams::getVendor() const
{
    return this->vendor;
}

void CardboardDeviceParams::setModel(const std::string& model_)
{
    this->model = model_;
}

const std::string& CardboardDeviceParams::getModel() const
{
    return this->model;
}

void CardboardDeviceParams::setVersion(const std::string& version_)
{
    this->version = version_;
}

const std::string& CardboardDeviceParams::getVersion() const
{
    return this->version;
}

void CardboardDeviceParams::setInterpupillaryDistance(float interpupillaryDistance_)
{
    this->interpupillaryDistance = interpupillaryDistance_;
}

float CardboardDeviceParams::getInterpupillaryDistance()
{
    return this->interpupillaryDistance;
}

void CardboardDeviceParams::setVerticalDistanceToLensCenter(float verticalDistanceToLensCenter_)
{
    this->verticalDistanceToLensCenter = verticalDistanceToLensCenter_;
}

float CardboardDeviceParams::getVerticalDistanceToLensCenter()
{
    return this->verticalDistanceToLensCenter;
}

void CardboardDeviceParams::setVisibleViewportSize(float visibleViewportSize_)
{
    this->visibleViewportSize = visibleViewportSize_;
}

float CardboardDeviceParams::getVisibleViewportSize()
{
    return this->visibleViewportSize;
}

void CardboardDeviceParams::setFovY(float fovY_)
{
    this->fovY = fovY_;
}

float CardboardDeviceParams::getFovY()
{
    return this->fovY;
}

void CardboardDeviceParams::setLensDiameter(float lensDiameter_)
{
    this->lensDiameter = lensDiameter_;
}

float CardboardDeviceParams::getLensDiameter()
{
    return this->lensDiameter;
}

void CardboardDeviceParams::setScreenToLensDistance(float screenToLensDistance_)
{
    this->screenToLensDistance = screenToLensDistance_;
}

float CardboardDeviceParams::getScreenToLensDistance()
{
    return this->screenToLensDistance;
}

void CardboardDeviceParams::setEyeToLensDistance(float eyeToLensDistance_)
{
    this->eyeToLensDistance = eyeToLensDistance_;
}

float CardboardDeviceParams::getEyeToLensDistance()
{
    return this->eyeToLensDistance;
}

Distortion* CardboardDeviceParams::getDistortion()
{
    return this->distortion;
}

bool CardboardDeviceParams::equals(CardboardDeviceParams *other)
{
    if (other == nullptr)
    {
        return false;
    }
    if (other == this)
    {
        return true;
    }
    return (this->getVendor() == other->getVendor()) && (this->getModel() == other->getModel()) && (this->getVersion() == other->getVersion()) && (this->getInterpupillaryDistance() == other->getInterpupillaryDistance()) && (this->getVerticalDistanceToLensCenter() == other->getVerticalDistanceToLensCenter()) && (this->getLensDiameter() == other->getLensDiameter()) && (this->getScreenToLensDistance() == other->getScreenToLensDistance()) && (this->getEyeToLensDistance() == other->getEyeToLensDistance()) && (this->getVisibleViewportSize() == other->getVisibleViewportSize()) && (this->getFovY() == other->getFovY()) && (this->getDistortion()->equals(other->getDistortion()));
}