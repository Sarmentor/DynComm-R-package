library(devtools)
library(rPython)

setwd("../")
python.load(file = paste(getwd(),'/base/Python/RDyn/RDyn_v2.py',sep=""), get.exception = TRUE)

