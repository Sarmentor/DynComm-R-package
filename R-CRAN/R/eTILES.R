# library(reticulate)
# 
# # packages.py <- c("networkx","numpy","tqdm","six")
# 
# # reticulate::py_config() ################ This gives me python version 2.7.15rc1 and I don't think the next command likes the rc. Do not know how to solve this
# # reticulate::py_install(packages.py, method = "virtualenv", conda = "auto") ########### Does not work, supposedly, due to above error. Gives Error : invalid version specification ‘2.7.15rc1’
# 
# # The implementation of eTILES algorithm.
# #
# # @param init.graph input initial edge list graph
# # @param streamfile.edge.removal .csv file with stream input edges to remove from initial graph
# # @param obs observation window (days)
# # @param path Path where generate the results and find the edge file
# # @param start starting date
# # @param end ending date
# # @return dynamic community detection represented in a XXXXXX type of object
# # @seealso \code{\link{nchar}} which this function wraps
# # @export
# # @examples
# # str_length(letters)
# etiles <- function(streamfile.edge.removal , init.graph=NULL, obs=7, path="", start=NULL, end=NULL){
# 
#   #initial networkx graph
#   nx <-reticulate::import("networkx")
#   py.initgraph <- nx$Graph(init.graph)
# 
#   #"""
#   #  Constructor
#   #  :param g: networkx graph
#   #  :param obs: observation window (days)
#   #  :param path: Path where generate the results and find the edge file
#   #  :param start: starting date
#   #  :param end: ending date
#   #"""
# 
#   ####################################################################
#   # Can not call eTILES like in the commented source line below. There
#   # is no instantiation of an object.
#   # eTILES is a python class (object) not an instance.
#   # The __init__ function, according to the documentation is called
#   # automatically when instantiating an object.
#   ####################################################################
#   # eTILES$__init__(self, filename=streamfile.edge.removal, g=py.initgraph, obs=7, path="", start=start, end=end)
#   
#   im <- reticulate::import_from_path("eTILES","../base/Python/TILES")
#   e<-im$eTILES(filename=streamfile.edge.removal, g=py.initgraph, obs=7, path="", start=start, end=end)
# 
#   return(e)
# 
# }
# 
# 
