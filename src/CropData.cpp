#include "CropData.h"
using namespace std;

void CropData::init(int value){
    suitability = new float[value];
    growingPeriod = new int[value];
    climdata = new wxdata[value];

    if(params.Gmin < durpr)
        params.Gmin = durpr;
    calcNSeason();

    suitEachSeason = new float*[value];
    for(int i = 0; i < value; i++){
        suitEachSeason[i] = new float[nSeason];
    }

    return;
}

void CropData::setMonth(int value){
    stMonth = value;
    return;
}

void CropData::setRainfed(bool rainfed)
{
    irrig = 1;
    if(rainfed==true){
        irrig = 0;
    }
    return;
}

void CropData::setParams(NumericVector inparams)
{
    params.Gmin = (int) inparams[0];
    params.Gmax = (int) inparams[1];
    params.Tkill = inparams[2];
    params.Tmin = inparams[3];
    params.Topmin = inparams[4];
    params.Topmax = inparams[5];
    params.Tmax = inparams[6];
    params.Rmin = inparams[7];
    params.Ropmin = inparams[8];
    params.Ropmax = inparams[9];
    params.Rmax = inparams[10];
    return;
}

void CropData::setMode(int value){
    mode = value;
    return;
}

void CropData::setResolution(int value){
    resolution = value;
    if (resolution == FZY_CROP_MONTHLY)
    {
        npr = 12;
        durpr = 30;
    }
    else
    {
        npr = 52;
        durpr = 7;
    }
    return;
}

void CropData::setTmin(int idx, float value){
    climdata[idx].tmin = value;
    return;
}

void CropData::setTmax(int idx, float value){
    climdata[idx].tmax = value;
    return;
}

void CropData::setPrcp(int idx, float value){
    climdata[idx].prcp = value;
    return;
}

void CropData::setSeasonSummary(const char* _method){
    string method = _method;
    for(int i=0; i<method.length(); i++){
        method[i] = tolower(method[i]);
    }
    if(method == "max"){
        seasonSummary = SUMMARY_MAX;
    }else if(method == "median"){
        seasonSummary = SUMMARY_MEDIAN;
    }else{
        seasonSummary = SUMMARY_MEDIAN;
        cout << "Use median as default. Current: " << method << "\n";
    }
    return;
}

float CropData::getSuitability(int idx){
    return suitability[idx];
}

float CropData::getGrowingPeriod(int idx){
    return growingPeriod[idx];
}

float CropData::getMaxSuit(){
    return maxSuit;
}

float CropData::getSuitEach(int startMonth, int period){
    return suitEachSeason[startMonth][period];
}

int CropData::getNSeason(){
    return nSeason;
}

int CropData::getPeriodEach(int season){
    return params.Gmin/durpr + season;
}

void CropData::calcMaxSuit(int size){
    maxSuit = 0.;
    for(int i=0;i<size;i++){
        if(suitability[i] > maxSuit) maxSuit = suitability[i];
    }
    return;
}

void CropData::calcNSeason(){
    nSeason = params.Gmax/durpr - params.Gmin/durpr + 1;
    return;
}

void CropData::Suitrun()
{
    int i, Season;
    float suitseason;

    for (int i = 0; i < nSeason; i++)
        suitEachSeason[stMonth][i] = 0;

    Season = params.Gmin;
    for(i = 0; i < nSeason; i++){
        suitseason = fzsuit(Season);
        suitEachSeason[stMonth][i] = suitseason;
        Season += durpr;
    }

    GetMedian(suitEachSeason[stMonth], nSeason);

    return;
}

float CropData::fzsuit(int season)
{
    int k;
    float sum = 0;
    float suit;
    float tmin;
    float tmax;
    float rain;
    float stress;
    int coldmth, heatmth, drymth;
    bool stressed = false;
    int mpr; // num of calculation unit

    mpr = season / durpr;
    // mpr = Season/30 for monthly; Seaosn/ 7 for wkly
    // npr = 12 for monthly ; 52 for wkly

    ////////////////////////////////////dynamic alloc///////////////////////////////////////////////////
    //use -> suitvars fvar [mpr]?
    suitvars* fvar = new suitvars[mpr];
    for (k = 0; k < mpr; k++) {
        fvar[k].TI = 0;
        fvar[k].RN = 0;
        fvar[k].GR = 0;
    }

    sum = 0;
    coldmth = 0;
    heatmth = 0;
    drymth = 0;
    // planting date identification
    k = 0;
    tmin = climdata[k].tmin;
    tmax = climdata[k].tmax;

    suit = THBox(tmax, tmin,
            params.Tmin,
            params.Tmin,
            params.Topmax,
            params.Tmax);
    if (suit == 0)
    {
        delete [] fvar;
        fvar = NULL;
        return suit;
    }

    for (k = 0; k < mpr; k++) {
        tmin = climdata[k].tmin;
        tmax = climdata[k].tmax;
        rain = climdata[k].prcp;

        if (tmax < tmin) {
            tmax = climdata[k].tmin;
            tmin = climdata[k].tmax;
        }

        fvar[k].TI = THBox(tmax, tmin,
            params.Tmin,
            params.Topmin,
            params.Topmax,
            params.Tmax);

        fvar[k].RN = Trapezoid(rain,
            params.Rmin / npr,
            params.Ropmin / npr,
            params.Ropmax / npr,
            params.Rmax / npr);

        // rain OR irrig
        // fvar[k].RN = fvar[k].RN + irrig - fvar[k].RN*irrig;
        if (tmin < params.Tmin) // || tmax > params.Topmax)
        {
            fvar[k].RN = 0;
        }

        // high temp and no irrigation 
        // if (tmax > params.Topmax && irrig < 0.01)
        if (tmax > params.Topmax && irrig == 0)
        {
            fvar[k].RN = 0;
        }


        stressed = false;
        if (drymth > 0)
        {
            if (fvar[k].RN > 0)
            {
                drymth = 0;
            }
            else  // consecutive dry month
            {
                fvar[k].TI /= (float) (drymth);
            }
        }

        if (irrig==0 && fvar[k].RN == 0)
        {
            drymth++;
        }
#ifdef NO_DROUGHT_STRESS
        // disable the effect of dry month
        drymth = 0;
#endif

        fvar[k].GR = calcsuit(&fvar[k]);
        sum += fvar[k].GR;

        // OR stress to be added
        if (!stressed && tmax >= params.Topmax)
        {
            stress = 1/(params.Tmax-params.Topmax) * (tmax - params.Topmax);
            stress *= (1-irrig);

            if (stress > sum)
                stress = sum;

            sum -= stress;
            if (tmax > params.Tmax)
                heatmth++;
        }
#ifdef TMIN_HEAT_STRESS
        if (!stressed && tmin >= params.Topmin)
        {
            stress = 1/(params.Topmax-params.Topmin) * (tmin - params.Topmin);
            stress *= (1-irrig);

            if (stress > sum)
                stress = sum;

            sum -= stress;
        }
#endif
        if (!stressed && tmin <= params.Tmin)
        {
            stress = 1/(params.Tmin- params.Tkill) * (params.Tmin - tmin);

            if (stress > sum)
                stress = sum;

            sum -= stress;
            if (tmin < params.Tkill)
                coldmth++;
        }
    }

    suit = sum / mpr;

    if ((coldmth + heatmth) > 0)
    {
        suit = 0;
    }

    ////////////////////////////////////dynamic alloc///////////////////////////////////////////////////
    delete [] fvar;
    fvar = NULL;
    return suit;
}

void CropData::GetMedian(float* daArray, int iSize) {
    // Allocate an array of the same size and sort it.
    // calc growing period for median suitability
    ////////////////////////////////////dynamic alloc///////////////////////////////////////////////////
    vector<float> dpSorted;
    vector<int> dpMonth;
    float dMedian = 0.;
    int dMonth = 0;
    for (int i = 0; i < iSize; ++i)
    {
        dpSorted.push_back(daArray[i]);
        dpMonth.push_back(params.Gmin / durpr + i);
    }
    iSize = dpSorted.size();
    for (int i = iSize - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (dpSorted[j] > dpSorted[j + 1]) {
                float dTemp = dpSorted[j];
                dpSorted[j] = dpSorted[j + 1];
                dpSorted[j + 1] = dTemp;
                int dTemp2 = dpMonth[j];
                dpMonth[j] = dpMonth[j+1];
                dpMonth[j + 1] = dTemp2;
            }
        }
    }

    if(seasonSummary == SUMMARY_MAX){
        dMedian = dpSorted[iSize-1];
        dMonth = dpMonth[iSize-1];
    } else if (seasonSummary == SUMMARY_MEDIAN){
        // Middle or average of middle values in the sorted array.
        dMonth = dpMonth[iSize/2];
        if ((iSize % 2) == 0) {
            dMedian = (dpSorted[iSize / 2] + dpSorted[(iSize / 2) - 1]) / 2.0;
        }
        else {
            dMedian = dpSorted[iSize / 2];
        }
    } else {
        dMedian = -99.;
        dMonth = -99;
    }

    suitability[stMonth] = dMedian;
    growingPeriod[stMonth] = dMonth;

    dpSorted.clear();
    return;
}

float CropData::calcsuit(suitvars* fvar)
{
    float GR = -99.;

    switch(mode)
    {
    case FZY_OR:
        GR = fvar->TI + fvar->RN - fvar->TI * fvar->RN;
        break;
    case FZY_AND:
        GR = fvar->TI * fvar->RN;
        break;
    }
    return GR;
}

