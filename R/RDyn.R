library(devtools)
library(reticulate)
library(rPython)

packages.py <- c("networkx","time","numpy","tqdm","six")

py_install(packages.py, envname = "r-reticulate", method = "auto", conda = "auto")

setwd("../")
python.load(file = paste(getwd(),'/base/Python/RDyn/RDyn_v2.py',sep=""), get.exception = TRUE)

