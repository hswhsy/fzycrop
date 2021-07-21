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
    float** suitEachSeason;
    float maxSuit;

    //settings
    int mode;
    int resolution;
    int size;
    int durpr;
    int npr;
    int seasonSummary;
    int nSeason;
    float irrig;

    protected:
    void GetMedian(float*, int);
    float fzsuit(int);
    float calcsuit(suitvars*);

    public:
    void setRainfed(bool);
    void setParams(NumericVector);
    void setMode(int);
    void setResolution(int);
    void setMonth(int);
    void setTmin(int, float);
    void setTmax(int, float);
    void setPrcp(int, float);
    void setSeasonSummary(const char*);

    float getSuitability(int);
    float getGrowingPeriod(int);
    float getMaxSuit();
    float getSuitEach(int, int);
    int getNSeason();
    int getPeriodEach(int);

    void init(int);
    void Suitrun();
    void calcMaxSuit();
    void calcNSeason();
};
#endif
