library(devtools)
library(rPython)
library(reticulate)

packages.py <- c("networkx","time","numpy","tqdm","six")

py_install(packages.py, envname = "r-reticulate", method = "auto", conda = "auto")

setwd("../")
python.load(file = paste(getwd(),'/base/Python/TILES/TILES.py',sep=""), get.exception = TRUE)

