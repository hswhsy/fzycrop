#ifndef FZCROP_H
#define FZCROP_H
float fzycrop(NumericVector inparams, NumericVector Tmax, NumericVector Tmin, 
               NumericVector Prcp, LogicalVector rainfed);
List fzycrop_getList(NumericVector inparams, NumericVector Tmax, NumericVector Tmin, 
               NumericVector Prcp, LogicalVector rainfed);
#endif
