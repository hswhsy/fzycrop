#include "CropData.h"
using namespace std;

void CropData::init(int value){
    suitability = new float[value];
    growingPeriod = new int[value];
    climdata = new wxdata[value];
    dormancyPeriod = new int[value];

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

void CropData::setIrrig(float irrig_input)
{
    irrig = irrig_input;
    return;
}

void CropData::setVernal(bool vernal_input)
{
    vernal = vernal_input;
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
    if(inparams.size() >11){
        params.Tc = inparams[11];
        params.Cr = inparams[12];
        params.TkillR = inparams[13];
    }else{
        params.Tc = -99;
        params.Cr = -99;
        params.TkillR = -99;
    }
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

void CropData::setTminMin(){
    tminMin = climdata[0].tmin;
    for(int i = 0; i < 12; i++){
        if(tminMin > climdata[i].tmin)
            tminMin = climdata[i].tmin;
    }
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
    if(vernal==TRUE){
        seasonSummary = SUMMARY_MAX;
        cout << "Use method : max for vernal\n";
    }else{
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
            cout << "Use method : median as default. Current: " << method << "\n";
        }
    }
    return;
}

float CropData::getSuitability(int idx){
    return suitability[idx];
}

float CropData::getMaxSuit(){
    return maxSuit;
}

float CropData::getSuitEach(int startMonth, int period){
    return suitEachSeason[startMonth][period];
}

int CropData::getGrowingPeriod(int idx){
    return growingPeriod[idx];
}

int CropData::getDormancyPeriod(int idx){
    return dormancyPeriod[idx];
}

int CropData::getNSeason(){
    return nSeason;
}

int CropData::getPeriodEach(int season){
    return params.Gmin/durpr + season;
}

int CropData::getSeasonSummary(){
    return seasonSummary;
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
        if(tminMin < params.TkillR){
            suitseason = 0;
        }else{
            if(vernal == TRUE){
                suitseason = fzsuit_vernal(Season);
            }else{
                suitseason = fzsuit(Season);
            }
        }
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
        fvar[k].RN = fvar[k].RN + irrig - fvar[k].RN*irrig;
        if (tmin < params.Tmin) // || tmax > params.Topmax)
        {
            fvar[k].RN = 0;
        }

        // high temp and no irrigation 
         if (tmax > params.Topmax && irrig < 0.01)
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

        if (fvar[k].RN == 0)
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

float CropData::fzsuit_vernal(int season)
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

    //chill day model
    bool rest;
    bool quiescence;
    bool flowering;
    int vernal_pr;
    int rest_pr;
    int quiescence_pr;
    int flowering_pr;
    float cdd, Cd, Ca;
    float Tc, Cr, Ca_fl;
    float vernal_temp;

    Tc = params.Tc;
    Cr = -params.Cr;
    Ca_fl = -2 * Cr;

    mpr = season / durpr;

    ////////////////////////////////////dynamic alloc///////////////////////////////////////////////////
    //use -> suitvars fvar [mpr]?
    suitvars* fvar = new suitvars[12];
    for (k = 0; k < 12; k++) {
        fvar[k].TI = 0;
        fvar[k].RN = 0;
        fvar[k].GR = 0;
    }

    sum = 0;
    coldmth = 0;
    heatmth = 0;
    drymth = 0;
    stress = 0;
    stressed = false;

    //vernalization init
    cdd = 0.;
    vernal_pr = 0;
    rest_pr = 99;
    quiescence_pr = 99;
    flowering_pr = 99;
    rest = true;
    quiescence = false;
    flowering = false;
    for (k = 0; k < 12; k++) {
        tmin = climdata[k].tmin;
        tmax = climdata[k].tmax;
        if (tmax < tmin) {
            tmax = climdata[k].tmin;
            tmin = climdata[k].tmax;
        }
        ChillDayModel(tmin, tmax, Tc, Cd, Ca);
        vernal_pr++;

        if(k==0 && Cd == 0){
            vernal_pr = 99.;
            break;
        }

        if(rest) {
            rest_pr = vernal_pr;
            cdd += Cd * durpr;
            if(cdd <= Cr) {
                vernal_temp = 1 - (cdd-Cr)/(Cd*durpr);
                cdd = Cr;
                cdd += Ca*durpr*(1-vernal_temp);
                rest = false;
                quiescence = true;
            }
            if(cdd>=0 && quiescence){
                quiescence_pr = vernal_pr;
                quiescence = false;
                flowering = true;
            }
        }else if (quiescence){
            quiescence_pr = vernal_pr;
            cdd += Ca * durpr;
            if (cdd >= 0) {
                vernal_temp = cdd / (Ca*durpr);
                quiescence_pr -= round(vernal_temp);
                quiescence = false;
                flowering = true;
            }
            if (cdd >= Ca_fl + Cr) {
                flowering_pr = vernal_pr;
                flowering = false;
                break;
            }
        }else if (flowering) {
            flowering_pr = vernal_pr;
            cdd += Ca * durpr;
            if (cdd >= Ca_fl + Cr) {
                vernal_temp = (cdd - (Ca_fl + Cr)) / (Ca*durpr);
                flowering_pr -= round(vernal_temp);
                flowering = false;
                break;
            }
        }
    }
    //calc vernalization end

    dormancyPeriod[stMonth] = quiescence_pr;
    int end;
    end = quiescence_pr + mpr;
    if(end > 12) end = 0;

    for (k = quiescence_pr; k < end; k++) {
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
        fvar[k].RN = fvar[k].RN + irrig - fvar[k].RN*irrig;
        if (tmin < params.Tmin) // || tmax > params.Topmax)
        {
            fvar[k].RN *= 1/(params.Tmin-params.Tkill) * (tmin - params.Tkill);
            if (tmin < params.Tkill) fvar[k].RN = 0;
//            fvar[k].RN = 0;
        }

        // high temp and no irrigation 
         if (tmax > params.Topmax && irrig < 0.01)
        {
            fvar[k].RN *= 1 - 1/(params.Tmax-params.Topmax) * (tmax - params.Topmax);
            if (tmax > params.Tmax) fvar[k].RN = 0;
//            fvar[k].RN = 0;
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

        if (fvar[k].RN == 0)
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

    if(flowering_pr < end){
      suit = sum / (end - quiescence_pr);
    }else{
      suit = 0.;
    }

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
    int m = 0;
    for (int i = 0; i < iSize; ++i)
    {
#ifdef ZERO_FILTER
        if (daArray[i] == 0.)
        {
            if (m == 0)
            {
                m++;
                dpSorted.push_back(0);
                dpMonth.push_back(params.Gmin / durpr + i);
            }
        }
        else
#endif
        {
            dpSorted.push_back(daArray[i]);
            dpMonth.push_back(params.Gmin / durpr + i);
        }
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

