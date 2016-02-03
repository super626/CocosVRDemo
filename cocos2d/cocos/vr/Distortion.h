/**
 Copyright (c) 2014 Peter Tribe.
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Original file from Peter Tribe
 
 This file was modified to fit the cocos2d-x project by super626
 */


#ifndef __CardboardVR_iOS__Distortion__
#define __CardboardVR_iOS__Distortion__

#include <string>
#include "Sensors/Internal/Structs.h"

class Distortion
{
public:
    Distortion();
    Distortion(Distortion *other);
    void setCoefficients(DistortionCoeffients coefficients);
    DistortionCoeffients getCoefficients();
    float distortionFactor(float radius);
    float distort(float radius);
    float distortInverse(float radius);
    bool equals(Distortion *other);
    std::string toString();
private:
    DistortionCoeffients coefficients;
};

#endif
