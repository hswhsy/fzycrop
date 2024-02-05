#include "common.h"

void ChillDayModel(float tmin, float tmax, float Tc, float& Cd, float& Ca)
{
    float tavg = (tmax + tmin) / 2.;
    if (0 <= tmin) {
        if ( Tc <= tmin) { //0 < Tc < Tmin < Tmax
            Cd = 0.;
            Ca = tavg - Tc;
        }
        else {
            if (Tc <= tmax) { // 0 < Tmin < Tc < Tmax
                Cd = -((tavg - tmin) - (tmax - Tc) / 2.);
                Ca = (tmax - Tc) / 2.;
            }
            else{ // 0 < Tmin < Tmax < Tc
                Cd = -(tavg - tmin);
                Ca = 0.;
            }
        }
    }
    else {
        if (tmax <= Tc) { // Tmin < 0 < Tmax < Tc
            Cd = -(tmax / (tmax - tmin)) * (tmax / 2.);
            Ca = 0.;
        }
        else { // Tmin < 0 < Tc < Tmax
            Cd = -((tmax / (tmax - tmin)) * (tmax / 2.) - (tmax - Tc) / 2.);
            Ca = (tmax - Tc) /2.;
        }
    }
}

float Bell(float x, float* params) {
    return 1 / (1 + pow(fabs((x - params[2]) / params[0]), (2 * params[1])));
}

float WeightedGaussian(float x, float* params) {
    return exp(pow((-(x - params[2]) / params[1]), 2) / 2) * params[0];
}

float Trapezoid(float x, float Pmin, float POPmin, float POPmax, float Pmax) {
    float val;
    if (POPmin == Pmin) {
        POPmin = POPmin + 0.001;
    }
    if (POPmax == Pmax) {
        Pmax = Pmax + 0.001;
    }

    if (x < Pmin)
        val = 0;
    else if (x < POPmin)
        val = (x - Pmin) / (POPmin - Pmin);
    else if (x < POPmax)
        val = 1;
    else if (x < Pmax)
        val = (Pmax - x) / (Pmax - POPmax);
    else
        val = 0;

    return val;
}

float THBox(float tmax, float tmin, float param1, float param2, float param3, float param4) {
    float validtime;
    float tavg, tanx;
    float max1diff, max2diff;
    float avg1diff, avg2diff;
    //float area = 0.;
    tavg = (tmax + tmin) / 2.;
    max1diff = fmax(tmax - param3, 0);
    max2diff = fmax(tmax - fmax(tmin, param2), 0);
    avg1diff = tmin - param3;
    avg2diff = tmax - param2;

    tanx = (tmax - tmin) / 12.;
    if (avg1diff > 0 || avg2diff < 0)
    {
        validtime = 0;
        //area = 0;
    }
    else
    {
        if (tmax == tmin)
        {
            //area = 0;
            if (tavg > param2 && tavg < param3)
            {
                validtime = 1;
                //area = 1;
            }
            else
            {
                validtime = 0;
            }
        }
        else
        {
            validtime = (max2diff - max1diff) * 2. / tanx / 24.;
            //area = validtime * (max2diff-max1diff)/2./24;
        }
    }

    return fmin(1, validtime);
}

