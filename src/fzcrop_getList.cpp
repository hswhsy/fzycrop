#include "common.h"
#include "CropData.h"
#include "fzcrop.h"

using namespace std;
using namespace Rcpp;

// [[Rcpp::export]]
List fzycrop_getList(NumericVector inparams,
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

    int nSeason = crop.getNSeason();

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
    List out;// = List::create(_["Suitability"], _["GrowingPeriod"]);
    for(int s=0; s<nSeason; s++){
        NumericVector suitEach(size);
        for(int i=0; i<size; i++){
            suitEach[i] = crop.getSuitEach(i,s);
        }
        name << "Suit" << crop.getPeriodEach(s) << "Mon";
        out.push_back(suitEach,name.str().c_str());
        name.str("");
        name.clear();
    }

    NumericVector suit(size);
    for(int i=0; i<size; i++){
        suit[i] = crop.getSuitability(i);
    }
    out.push_back(suit,"Suitability");

    if(method=="max"){
        NumericVector growingPeriod(size);
        for(int i=0; i<size; i++){
            growingPeriod[i] = crop.getGrowingPeriod(i);
            if(suit[i] == 0){
                growingPeriod[i] = NumericVector::get_na();
            }
        }
        out.push_back(growingPeriod, "GrowingPeriod");
    }

    return out;
}
