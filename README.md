# fzycrop
Calculate suitability with fuzzy logic   
Kim, H., S. W. Hyun, G. Hoogenboom, C. H. Porter and K. S. Kim, 2018: Fuzzy Union to Assess Climate Suitability of Annual Ryegrass (Lolium multiflorum), Alfalfa (Medicago sativa) and Sorghum (Sorghum bicolor). Scientific Reports 8(1), 10220.

Linux (in R)  
-------------
#install devtools    
if(!require(devtools)) install.packages("devtools")    

#install fzycrop    
library(devtools)   
install_github("hswhsy/fzycrop")

Windows    
-------------
#download and install Rtools
#https://cran.r-project.org/bin/windows/Rtools/   

#in R    
#install devtools and Rcpp and other dependencies   
if(!require(devtools)) install.packages("devtools")   
if(!require(Rcpp)) install.packages("Rcpp")   

#install fzycrop    
library(devtools)   
install_github("hswhsy/fzycrop")

After install package
------------
library(fzycrop)    
help(fzycrop)
