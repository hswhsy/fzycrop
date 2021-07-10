#include "common.h"
#include "CropData.h"
#include "fzcrop.h"

using namespace std;
using namespace Rcpp;

int _mode = FZY_OR;
int _resolution = FZY_CROP_MONTHLY;

// [[Rcpp::export]]
List ecocropK(NumericVector inparams,
              NumericVector Tmax,
              NumericVector Tmin,
              NumericVector Prcp,
              LogicalVector rainfed = true,
              String method = "median")
{
    //init
    int size = Tmax.size();
    List out = List::create(_["Suitability"], _["GrowingPeriod"]);
    NumericVector suit(size);
    NumericVector growingPeriod(size);

    CropData crop;
    crop.init(size);
    crop.setRainfed(rainfed);
    crop.setParams(inparams);
    crop.setMode(_mode);
    crop.setResolution(_resolution);
    crop.setSeasonSummary(method.get_cstring());

    //run fzycrop
    for(int i=0;i<size;i++){
        crop.setMonth(i);
        for(int j=0;j<size;j++){
            int idx = (i+j)%size;
            crop.setTmin(j,Tmin[idx]);
            crop.setTmax(j,Tmax[idx]);
            crop.setPrcp(j,Prcp[idx]);
        }
        crop.Suitrun();
    }
    crop.calcMaxSuit();

    //output
    for(int i=0; i<size; i++){
        suit[i] = crop.getSuitability(i);
        growingPeriod[i] = crop.getGrowingPeriod(i);
    }
    out[0] = suit;
    out[1] = growingPeriod;

    return out;
}
