#ifndef CROP_DATA_H
#define CROP_DATA_H

#include <Rcpp.h>
#include "common.h"

using namespace Rcpp;

class CropData
{
    private:
    //parameters
    typedef struct PARAMETER{
        int Gmin, Gmax;
        float Tkill, Tmin, Topmin, Topmax, Tmax;
        float Rmin, Ropmin, Ropmax, Rmax;
    } parameter;
    parameter params;

    //wxdata
    typedef struct WXDATA {
        float tmax;
        float tmin;
        float prcp;
    } wxdata;
    wxdata* climdata;

    //suitabilies
    typedef struct SuitVars {
        float TI;
        float RN;
        float GR;
    } suitvars;
    int stMonth;
    int* growingPeriod;
    float* suitability;
    float maxSuit;

    //settings
    int mode;
    int resolution;
    int size;
    int durpr;
    int npr;
    int seasonSummary;
    float irrig;

    protected:
    void GetMedian(float* daArray, int iSize);
    float fzsuit(int season);
    float calcsuit(suitvars* fvar);

    public:
    void setRainfed(bool rainfed);
    void setParams(NumericVector inparams);
    void setMode(int value);
    void setResolution(int value);
    void setMonth(int value);
    void setTmin(int idx, float value);
    void setTmax(int idx, float value);
    void setPrcp(int idx, float value);
    void setSeasonSummary(const char* method);

    float getSuitability(int idx);
    float getGrowingPeriod(int idx);
    float getMaxSuit();

    void init(int value);
    void Suitrun();
    void calcMaxSuit();
};
#endif
