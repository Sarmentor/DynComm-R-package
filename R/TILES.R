library(devtools)
library(rPython)

setwd("../")
python.load(file = paste(getwd(),'/base/Python/TILES/TILES.py',sep=""), get.exception = TRUE)

