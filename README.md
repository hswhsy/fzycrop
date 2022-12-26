# fzycrop
Calculate suitability with fuzzy logic   
Kim, H., S. W. Hyun, G. Hoogenboom, C. H. Porter and K. S. Kim, 2018: Fuzzy Union to Assess Climate Suitability of Annual Ryegrass (Lolium multiflorum), Alfalfa (Medicago sativa) and Sorghum (Sorghum bicolor). Scientific Reports 8(1), 10220.

Windows    
-------------
#(in R) install devtools and Rcpp and other dependencies   
if(!require(Rcpp)) install.packages("Rcpp")   

#(in R) install fzycrop    
remotes::install_github("hswhsy/fzycrop")

Linux (in terminal)  
-------------
#get package and install    
git clone https://github.com/hswhsy/fzycrop.git    
R CMD INSTALL fzycrop

Linux (in R)  
-------------
#install fzycrop    
remotes::install_github("hswhsy/fzycrop")

After install package (in R)
------------
library(fzycrop)    
help(fzycrop)
