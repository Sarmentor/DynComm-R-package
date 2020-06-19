########################### Developer Notice ###########################
# Description:
# This file holds the DynComm post processing algorithms implemented in R.
#
# Internally, this object, dispatches calls to objects that do the actual work.
#
# A template for post processing algorithms is provided as a file named 
# "TemplateDynCommPostProcess.R" and can be found in the dev folder in the
# project source page on GitHub.
#
# More developer information can be found in the project source page on GitHub at
# https://github.com/softskillsgroup/DynComm-R-package
#
#
# Author: poltergeist0
# Date: 2019-01-01

########################### Include R sources here ###########################
source ("R/postProcessDensOpt.R")

########################### API Documentation ###########################
#' @name DynCommPostProcessR
#' 
#' @keywords internal
#' 
#' @title DynCommPostProcessR(postProcessing, previous, Parameters)
#'
#' @author poltergeist0
#' 
#' @description 
#' Provides a single interface for all post processing algorithms implemented in R.
#' 
#' @details 
#' Includes methods to get results of processing and to interact with the 
#' vertices, edges and communities.
#'
#' @rdname DynCommPostProcessR
#' 
# @docType class
#' 
#' @usage DynCommPostProcessR(postProcessing, previous, Parameters)
#' 
#' @param Algorithm One of the available ALGORITHM See \code{\link{ALGORITHM}}
#' 
#' @param Criterion One of the available CRITERION. See \code{\link{CRITERION}}
#' 
#' @param Parameters A two column matrix defining additional parameters. See
#'   the PARAMETERS section on this page
#'
#' @return \code{DynCommPostProcessR} object
#'
#' @seealso \code{\link{DynComm}}
#' 
# @export
#'
#' @examples
#' \dontrun{
#' Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdgesFile("initial_graph.txt")
#' dc$communityCount()
#' dc$communities()
#' dc$communityNodeCount(1)
#' dc$vertices(1)
#' dc$communityMapping(TRUE)
#' dc$time()
#' dc$addRemoveEdgesFile("s0000000000.txt")
#' }
#'
#' @section PARAMETERS:
#' A two column matrix defining additional parameters to be passed to the
#' selected ALGORITHM and CRITERION.
#' The first column names the parameter and the second defines its value.
#' \describe{
#'   \item{
#'   -c
#'   }{
#'   Owsinski-Zadrozny quality function parameter. Values [0.0:1.0]. Default: 0.5
#'   }
#'   \item{
#'   -k
#'   }{
#'   Shi-Malik quality function kappa_min value. Value > 0 . Default 1
#'   }
#'   \item{
#'   -w
#'   }{
#'   Treat graph as weighted. In other words, do not ignore weights for edges 
#'   that define them when inserting edges in the graph.
#'   A weight of exactly zero removes the edge instead of inserting so its
#'   weight is never ignored.
#'   Without this parameter defined or for edges that do not have a weight defined, 
#'   edges are assigned the default value of 1 (one).
#'   As an example, reading from a file may define weights (a third column) for
#'   some edges (defined in rows, one per row) and not for others. With this
#'   parameter defined, the edges that have weights that are not exactly zero,
#'   have their weight replaced by the default value.
#'   }
#'   \item{
#'   -e
#'   }{
#'   Stops when, on a cycle of the algorithm, the quality is increased by less 
#'   than the value given in this parameter.
#'   }
#'   \item{
#'   cv
#'   }{
#'   Community-Vertex.
#'   Boolean parameter that indicates if sending community mapping to a file
#'   prints the community first, if true, or the vertex first, if false. See
#'   \code{\link{communityMapping}} for details.
#'   Default TRUE
#'   }
#' }
#' 
#' @section Methods:
#' \describe{
#' 
# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
DynCommPostProcessR <- function(postProcessing=POSTPROCESSING$NONE, previous, Parameters=NULL)
{
  
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()

  ########## add new algorithms here #############
  algorithms = function(){
    if(postProcessing==POSTPROCESSING$DENSOPT){
      return(postProcessDensOpt(previous,Parameters))
    }
    return(NULL)
  }
  
  ########## constructor #############
  alg <- algorithms()  #algorithm selection

  ## Create the list used to represent an
  ## object for this class
  me <- list(
    
    ## Define the environment where this list is defined so
    ## that I can refer to it later.
    thisEnv = thisEnv,
    
    has = function(apiFunction){
      return(alg$has(apiFunction))
    },
    
    #' 
    #'   \item{results(differential)}{Get additional results of the algorithm or the currently selected post processing steps. See \code{\link{results}}}
    #'   
    results = function(differential=TRUE){
        return(alg$results(differential))
    },

    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \code{\link{quality}}}
    #'   
    quality=function(){
        return(alg$quality())
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \code{\link{communityCount}}}
    #'   
    communityCount=function(){
        return(alg$communityCount())
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \code{\link{communities}}}
    #'   
    communities=function(){
        return(alg$communities())
    },
    
    #' 
    #'   \item{communitiesEdgeCount()}{Get the number of community to community edges in the graph. See \code{\link{communitiesEdgeCount}}}
    #'   
    communitiesEdgeCount=function(){
        return(alg$communitiesEdgeCount())
    },
    
    #' 
    #'   \item{communityNeighbours(community)}{Get the neighbours of the given community after the last iteration. See \code{\link{communityNeighbours}}}
    #'   
    communityNeighbours=function(community){
        return(alg$communityNeighbours(community))
    },
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \code{\link{communityInnerEdgesWeight}}}
    #'   
    communityInnerEdgesWeight=function(community){
        return(alg$communityInnerEdgesWeight(community))
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \code{\link{communityTotalWeight}}}
    #'   
    communityTotalWeight=function(community){
        return(alg$communityTotalWeight(community))
    },
      
        
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \code{\link{communityEdgeWeight}}}
    #'   
    communityEdgeWeight=function(source,destination){
        return(alg$communityEdgeWeight(source,destination))
    },
        
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \code{\link{communityVertexCount}}}
    #'   
    communityVertexCount=function(community){
        return(alg$communityVertexCount(community))
    },
        
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \code{\link{community}}}
    #'   
    community=function(vertex){
        return(alg$community(vertex))
    },
        
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \code{\link{vertexCount}}}
    #'   
    vertexCount=function(){
        return(alg$vertexCount())
    },

    #' 
    #'   \item{verticesAll()}{Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}}
    #'   
    verticesAll=function(){
        return(alg$verticesAll())
    },
        
    #' 
    #'   \item{neighbours(vertex)}{Get the neighbours of the given vertex after the last iteration. See \code{\link{neighbours}}}
    #'   
    neighbours=function(vertex){
        return(alg$neighbours(community))
    },
    
    #' 
    #'   \item{edgeWeight(source,destination)}{Get the weight of the edge that goes from source vertex to destination vertex after the last iteration. See \code{\link{edgeWeight}}}
    #'   
    edgeWeight=function(source,destination){
        return(alg$edgeWeight(source,destination))
    },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \code{\link{vertices}}}
    #'   
    vertices=function(community){
        return(alg$vertices(community))
    },
    
    #' 
    #'   \item{edgeCount()}{Get the number of vertex to vertex edges in the graph. See \code{\link{edgeCount}}}
    #'   
    edgeCount=function(){
        return(alg$edgeCount())
    },
    
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingFile = function(differential=TRUE,file="communityMapping.txt"){
        return(alg$communityMappingFile(differential,file))
    },
    
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingMatrix = function(differential=TRUE){
        return(alg$communityMappingMatrix(differential))
    }

  )
  # close methods section of the documentation
  #' 
  #' }
  #' 
  
  
  ## Define the value of the list within the current environment.
  assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"DynCommPostProcessR")
  
  if(is.null(alg)) return(NULL)
  return(me)
}
