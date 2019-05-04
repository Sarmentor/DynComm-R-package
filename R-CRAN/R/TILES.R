# library(reticulate)
# 
# # packages.py <- c("networkx","time","numpy","tqdm","six")
# 
# # py_install(packages.py, envname = "virtualenv", method = "auto", conda = "auto")
# 
# 
# # The implementation of TILES algorithm.
# #
# # @param init.graph input initial edge list graph
# # @param streamfile .csv file with stream input edges
# # @param ttl edge time to live (days)
# # @param obs observation window (days)
# # @param path Path where generate the results and find the edge file
# # @param start starting date
# # @param end ending date
# # @return dynamic community detection represented in a XXXXXX type of object
# # @seealso \code{\link{nchar}} which this function wraps
# # @export
# # @examples
# # str_length(letters)
# tiles <- function(streamfile.edge.removal, init.graph=NULL, ttl=Inf, obs=7, path="", start=NULL, end=NULL){
# 
#   #initial networkx graph
#   nx <-reticulate::import("networkx")
#   py.initgraph <- nx$Graph(init.graph)
#   
#   #"""
#   #
#   #  Constructor
#   #  :param g: networkx graph
#   #  :param ttl: edge time to live (days)
#   #  :param obs: observation window (days)
#   #  :param path: Path where generate the results and find the edge file
#   #  :param start: starting date
#   #  :param end: ending date
#   #"""
# 
#   # TILES$__init__(self, filename=streamfile, g=py.initgraph, ttl=float('inf'), obs=7, path="", start=start, end=end)
#   # 
#   # return(TILES$execute(self))
#   im <- reticulate::import_from_path("TILES","../base/Python/TILES")
#   e<-im$TILES(filename=streamfile.edge.removal, g=py.initgraph, obs=7, path="", start=start, end=end)
#   
#   return(e)
#   
# 
# }
