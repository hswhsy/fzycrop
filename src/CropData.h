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
        float Tc, Cr, TkillR;
    } parameter;
    parameter params;

    //wxdata
    typedef struct WXDATA {
        float tmax;
        float tmin;
        float prcp;
    } wxdata;
    wxdata* climdata;
    float tminMin;

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
    int* dormancyPeriod;

    //settings
    int mode;
    int resolution;
    int size;
    int durpr;
    int npr;
    int seasonSummary;
    int nSeason;
    float irrig;
    bool vernal;

    protected:
    void GetMedian(float*, int);
    float fzsuit(int);
    float fzsuit_vernal(int);
    float calcsuit(suitvars*);

    public:
    void setIrrig(float);
    void setVernal(bool);
    void setParams(NumericVector);
    void setMode(int);
    void setResolution(int);
    void setMonth(int);
    void setTmin(int, float);
    void setTminMin();
    void setTmax(int, float);
    void setPrcp(int, float);
    void setSeasonSummary(const char*);

    float getSuitability(int);
    float getMaxSuit();
    float getSuitEach(int, int);
    int getGrowingPeriod(int);
    int getDormancyPeriod(int);
    int getNSeason();
    int getPeriodEach(int);
    int getSeasonSummary();
    bool getVernal();

    void init(int);
    void Suitrun();
    void calcMaxSuit(int);
    void calcNSeason();
};
#endif
