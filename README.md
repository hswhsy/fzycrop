# fzycrop
Calculate suitability with fuzzy logic   
Kim, H., S. W. Hyun, G. Hoogenboom, C. H. Porter and K. S. Kim, 2018: Fuzzy Union to Assess Climate Suitability of Annual Ryegrass (Lolium multiflorum), Alfalfa (Medicago sativa) and Sorghum (Sorghum bicolor). Scientific Reports 8(1), 10220.

Windows    
-------------
#Download and install Rtools from    
#https://cran.r-project.org/bin/windows/Rtools/   

#(in R) install remotes and fzycrop    
if(!require(remotes)) install.packages("remotes")   
remotes::install_github("hswhsy/fzycrop")

Linux (in R)  
-------------
#install fzycrop    
if(!require(remotes)) install.packages("remotes")   
remotes::install_github("hswhsy/fzycrop")

Linux (in terminal)  
-------------
#get package and install    
git clone https://github.com/hswhsy/fzycrop.git    
R CMD INSTALL fzycrop

After install package (in R)
------------
library(fzycrop)    
help(fzycrop)
