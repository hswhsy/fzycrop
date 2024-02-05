#include "common.h"
#include "CropData.h"
#include "fzcrop.h"

using namespace std;
using namespace Rcpp;

// [[Rcpp::export]]
float fzycrop(NumericVector inparams,
              NumericVector Tmax,
              NumericVector Tmin,
              NumericVector Prcp,
              NumericVector irrig = 0,
              LogicalVector vernal = false,
              String method = "median")
{
    //init
    stringstream name;
    int size = Tmax.size();
    int _mode = FZY_OR;
    int _resolution = FZY_CROP_MONTHLY;

    CropData crop;
    crop.setIrrig(irrig[0]);
    crop.setVernal(vernal[0]);
    crop.setSeasonSummary(method.get_cstring());
    crop.setResolution(_resolution);
    crop.setParams(inparams);
    crop.setMode(_mode);
    crop.init(size); //need calcNSeason

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
    crop.calcMaxSuit(size);

    //output
    float out = 0.;
    for(int i=0; i<size; i++){
        if(out < crop.getSuitability(i)){
            out = crop.getSuitability(i);
        }
    }
    return out;
}
