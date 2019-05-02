# library(reticulate)
# 
# # packages.py <- c("networkx","time","numpy","tqdm","six")
# 
# # py_install(packages.py, envname = "virtualenv", method = "auto", conda = "auto")
# 
# # The implementatio of TILES algorithm.
# #
# # @param size input initial size of graph
# # @param iterations number of iterations
# # @param avg_deg node average degree
# # @param sigma XXXX
# # @param lambdad XXXX
# # @param alpha XXXXX
# # @param paction XXXXX
# # @param prenewal XXXXX
# # @param quality_threshold XXXX
# # @param new_node XXXXX
# # @param del_node XXXXX
# # @param max_evts XXXXX
# # @return dynamically generated network in a stream of edges
# # @seealso \code{\link{nchar}} which this function wraps
# # @export
# # @examples
# # str_length(letters)
# rdyn <- function(size=1000, iterations=100, avg_deg=15, sigma=.6,
#                  lambdad=1, alpha=2.5, paction=1, prenewal=.8,
#                  quality_threshold=.2, new_node=.0, del_node=.0, max_evts=1){
# 
#   # RDyn$__init__(self, size=size, iterations=iterations, avg_deg=avg_deg, sigma=sigma,
#   #               lambdad=lambdad, alpha=alpha, paction=paction, prenewal=prenewal,
#   #               quality_threshold=quality_threshold, new_node=new_node, del_node=del_node, max_evts=max_evts):
# 
# 
#   # return(RDyn$execute(self, simplified=TRUE))
# 
#   im <- reticulate::import_from_path("RDyn","../base/Python/RDyn")
#   e<-im$RDyn(size=size, iterations=iterations, avg_deg=avg_deg, sigma=sigma,
#                            lambdad=lambdad, alpha=alpha, paction=paction, prenewal=prenewal,
#                            quality_threshold=quality_threshold, new_node=new_node, del_node=del_node, max_evts=max_evts)
#   
#   return(e)
#   
# }
# 
