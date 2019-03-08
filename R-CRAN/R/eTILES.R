library(devtools)
library(reticulate)

packages.py <- c("networkx","numpy","tqdm","six")

py_install(packages.py, method = "virtualenv", method = "auto", conda = "auto")

setwd("../")
source_python(paste(getwd(),'/base/Python/TILES/eTILES.py',sep=""))

#' The implementation of eTILES algorithm.
#'
#' @param init.graph input initial edge list graph
#' @param streamfile.edge.removal .csv file with stream input edges to remove from initial graph
#' @param obs observation window (days)
#' @param path Path where generate the results and find the edge file
#' @param start starting date
#' @param end ending date
#' @return dynamic community detection represented in a XXXXXX type of object
#' @seealso \code{\link{nchar}} which this function wraps
#' @export
#' @examples
#' str_length(letters)
eTILES <- function(streamfile.edge.removal , init.graph=NULL, obs=7, path="", start=NULL, end=NULL){
  
  #initial networkx graph
  import("networkx")
  py.initgraph <- nx.Graph(init.graph)
  
  """
    Constructor
    :param g: networkx graph
    :param obs: observation window (days)
    :param path: Path where generate the results and find the edge file
    :param start: starting date
    :param end: ending date
  
  """
  
  eTILES$__init__(self, filename=streamfile.edge.removal, g=py.initgraph, obs=7, path="", start=start, end=end)
  
  return(eTILES$execute(self))
  
}


