#ifndef FZCROP_H
#define FZCROP_H
float fzycrop(NumericVector inparams, NumericVector Tmax, NumericVector Tmin, 
               NumericVector Prcp, String method);
List fzycrop_getList(NumericVector inparams, NumericVector Tmax, NumericVector Tmin, 
               NumericVector Prcp, String method);
#endif
