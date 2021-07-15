\name{fzycrop}
\alias{fzycrop}
\docType{package}
\title{
FZYCROP model
}
\description{
Simple suitability model for plants using fuzzy logic (2020-02-24)
}
\usage{
fzycrop(inparams, Tmax, Tmin, Prcp, rainfed, method)	
}
\arguments{
  \item{inparams}{Vector of parameters of a plant (Gmin, Gmax, Tkill, Tmin, Topmin, Topmax, Tmax, Rmin, Ropmin, Ropmax, Rmax)}
  \item{Tmax}{Vector of monthly maximum temperature (degrees C)}
  \item{Tmin}{Vector or monthly minimum temperature (degrees C)}
  \item{Prcp}{Vector of monthly precipitation (mm)}
  \item{rainfed}{Logical. Rainfed (TRUE) or irrigated (FALSE)}
  \item{method}{String. Method for summarize seasonal suitability. "max" or "median"}
}
\references{
  Kim, H., Hyun, S. W., Hoogenboom, G., Porter, C. H., & Kim, K. S. (2018). Fuzzy Union to Assess Climate Suitability of Annual Ryegrass (Lolium multiflorum), Alfalfa (Medicago sativa) and Sorghum (Sorghum bicolor). Scientific reports, 8(1), 1-15.
}
\examples{
#input vectors
params<-c(40,90,0,8,12,22,34,500,800,1600,2700)
tmax<-c(1.5,4.7,10.4,17.8,23,27.1,28.6,29.6,25.8,19.8,11.6,4.3)
tmin<-c(-5.9,-3.4,1.6,7.8,13.2,18.2,21.9,22.4,17.2,10.3,3.2,-3.2)
prcp<-c(20.8,25,47.2,64.5,105.9,133.2,394.7,364.2,169.3,51.8,52.5,21.5)

#run ecocropK
fzycrop(params, tmax, tmin, prcp, TRUE, "median")
}
