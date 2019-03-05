library(devtools)
library(rPython)
library(reticulate)

packages.py <- c("networkx","numpy","tqdm","six")

py_install(packages.py, method = "virtualenv", conda = "auto")

setwd("../")
python.load(file = paste(getwd(),'/base/Python/TILES/eTILES.py',sep=""), get.exception = TRUE)

