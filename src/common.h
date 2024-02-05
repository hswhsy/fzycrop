#ifndef COMMON_H
#define COMMON_H

#include<vector>
#include<cmath>
#include<cctype>
#include<string>
#include<iostream>

const float no_data = -99;
const float pi= 3.14159265359;
const float Gsc = 0.082;

#define FZY_OR 1
#define FZY_AND 2
#define FZY_CROP_MONTHLY 30
#define FZY_CROP_WEEKLY 7
#define DAY_OF_END_YEAR 360
#define SUMMARY_MEDIAN 101
#define SUMMARY_MAX 102

float THBox(float tmax, float tmin, float Tmin, float Topmin, float Topmax, float Tmax);
float Bell(float x, float* params);
float WeightedGaussian(float x, float* params);
float Trapezoid(float x, float Pmin, float POPmin, float POPmax, float Pmax);
void ChillDayModel(float tmin, float tmax, float Tc, float& Cd, float& Ca);

#endif
