# ecocropK
Improved with fuzzy logic   
Kim, H., S. W. Hyun, G. Hoogenboom, C. H. Porter and K. S. Kim, 2018: Fuzzy Union to Assess Climate Suitability of Annual Ryegrass (Lolium multiflorum), Alfalfa (Medicago sativa) and Sorghum (Sorghum bicolor). Scientific Reports 8(1), 10220.

Linux (in terminal)  
-------------
R CMD INSTALL ecocropK   

Windows (in R)   
-------------
#install Rtools and other dependencies   
#https://cran.r-project.org/bin/windows/Rtools/   
#install.packages("devtools")   
#install.packages("Rcpp")   
   
library(devtools)   
install_github("hswhsy/ecocropK")
