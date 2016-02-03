//
//  Distortion.h
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

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
