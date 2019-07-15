########################### Developer Notice ###########################
# Description:
# This file holds all DynComm main algorithms implemented in R. It contains the 
# API for R.
#
# Internally, this object, dispatches calls to objects that do the actual work.
#
# More developer information can be found in the project source page on GitHub at
# https://github.com/softskillsgroup/DynComm-R-package
#
#
# Author: poltergeist0
# Date: 2019-01-01

########################### Include R sources here ###########################
#source("something.R")

########################### Main Algorithm Documentation ###########################
#' @name DynCommMainR
#' 
#' @keywords internal
#' 
#' @title DynCommMainR
#'
#' @author poltergeist0
#' 
#' @description 
#' Provides a single interface for all main algorithms written in R.
#' 
#' @details 
#' Includes methods to get results of processing and to interact with the 
#' vertices, edges and communities.
#'
#' @rdname DynCommMainR
#' 
# @docType class
#' 
#' @usage DynCommMainR(Algorithm,Criterion,Parameters)
#' 
#' @param Algorithm One of the available ALGORITHM See \code{\link{ALGORITHM}}
#' 
#' @param Criterion One of the available CRITERION. See \code{\link{CRITERION}}
#' 
#' @param Parameters A two column matrix defining additional parameters. See
#'   the PARAMETERS section on this page
#'
#' @return \code{DynCommMainR} object
#'
#' @seealso \code{\link{DynComm}}
#' 
# @export
#'
#' @examples
#' \dontrun{
#' Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
#' dc<-DynCommMainR(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
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
DynCommMainR <- function(Algorithm,Criterion,Parameters)
{
  
  ## Get the environment for this instance of the function.
  thisEnv <- environment()
  
  ########## constructor #############
  alg <- Algorithm
  qlt <- Criterion
  prm <- Parameters
  if(is.null(Parameters)){
    #set default parameters
  }
  else{
    # TODO validate parameters
    assign("prm",Parameters,thisEnv)
  }
  
  ########## add new algorithms here #############
  # if(alg==){
    # dc <- new(DynCommRcpp,alg,qlt,prm)
    # print(dc)
    # TO DO: check for errors
  # }
  # else if(alg>=10001 & alg<=20000){
  #   # print("Python algorithm")
  #   dc <- reticulate::import_from_path("DynCommPython","../src/base/Python")
  #   # calls to python should be equal to calls to c++ but may need separate processing of outputs
  # }
  # else{
    dc<-NULL
    print("Unknown algorithm :(")
  # }
  
  ## Create the list used to represent an
  ## object for this class
  me <- list(
    
    ## Define the environment where this list is defined so
    ## that I can refer to it later.
    thisEnv = thisEnv,
    
    #' 
    #'   \item{results(differential)}{Get additional results of the algorithm or the currently selected post processing steps. See \code{\link{results}}}
    #'   
    results = function(differential=TRUE)
    {
      # if(alg>=1 & alg<=10000){
      #   return(dc$results(differential))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$results(differential))
      # }
      # else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("name","value"))))
      # }
    },
    
    #' 
    #'   \item{addRemoveEdges(graphAddRemove)}{Add and remove edges read from a file. See \code{\link{addRemoveEdges}}}
    #'   
    addRemoveEdgesFile = function(graphAddRemoveFile){
      # if(alg>=1 & alg<=10000){
      #   return(dc$addRemoveEdgesFile(graphAddRemoveFile))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$addRemoveEdgesFile(graphAddRemoveFile))
      # }
      # else{
        return(FALSE)
      # }
    },
    
    #' 
    #'   \item{addRemoveEdges(graphAddRemove)}{Add and remove edges read from a matrix. See \code{\link{addRemoveEdges}}}
    #'   
    addRemoveEdgesMatrix = function(graphAddRemoveMatrix){
      # if(alg>=1 & alg<=10000){
      #   return(dc$addRemoveEdgesMatrix(graphAddRemoveMatrix))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$addRemoveEdgesMatrix(graphAddRemoveMatrix))
      # }
      # else{
        return(FALSE)
      # }
    },
    
    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \code{\link{quality}}}
    #'   
    quality=function(){
      # if(alg>=1 & alg<=10000){
      #   return(dc$quality())
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$quality())
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \code{\link{communityCount}}}
    #'   
    communityCount=function(){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityCount())
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityCount())
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \code{\link{communities}}}
    #'   
    communities=function(){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communities())
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communities())
      # }
      # else{
        return(list())
      # }
    },
    
    #' 
    #'   \item{communitiesEdgeCount()}{Get the number of community to community edges in the graph. See \code{\link{communitiesEdgeCount}}}
    #'   
    communitiesEdgeCount=function() {
      # if(alg>=1 & alg<=10000){
      #   return(dc$communitiesEdgeCount())
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communitiesEdgeCount())
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{communityNeighbours(community)}{Get the neighbours of the given community after the last iteration. See \code{\link{communityNeighbours}}}
    #'   
    communityNeighbours=function(community){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityNeighbours(community))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityNeighbours(community))
      # }
      # else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
      # }
    },
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \code{\link{communityInnerEdgesWeight}}}
    #'   
    communityInnerEdgesWeight=function(community){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityInnerEdgesWeight(community))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityInnerEdgesWeight(community))
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \code{\link{communityTotalWeight}}}
    #'   
    communityTotalWeight=function(community){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityTotalWeight(community))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityTotalWeight(community))
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \code{\link{communityEdgeWeight}}}
    #'   
    communityEdgeWeight=function(source,destination){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityEdgeWeight(source,destination))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityEdgeWeight(source,destination))
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \code{\link{communityVertexCount}}}
    #'   
    communityVertexCount=function(community){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityVertexCount(community))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityVertexCount(community))
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \code{\link{community}}}
    #'   
    community=function(vertex){
      # if(alg>=1 & alg<=10000){
      #   return(dc$community(vertex))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$community(vertex))
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \code{\link{vertexCount}}}
    #'   
    vertexCount=function(){
      # if(alg>=1 & alg<=10000){
      #   return(dc$vertexCount())
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$vertexCount())
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{verticesAll()}{Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}}
    #'   
    verticesAll=function(){
      # if(alg>=1 & alg<=10000){
      #   return(dc$verticesAll())
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$verticesAll())
      # }
      # else{
        return(list())
      # }
    },
    
    #' 
    #'   \item{neighbours(vertex)}{Get the neighbours of the given vertex after the last iteration. See \code{\link{neighbours}}}
    #'   
    neighbours=function(vertex){
      # if(alg>=1 & alg<=10000){
      #   return(dc$neighbours(vertex))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$neighbours(vertex))
      # }
      # else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
      # }
    },
    
    #' 
    #'   \item{edgeWeight(source,destination)}{Get the weight of the edge that goes from source vertex to destination vertex after the last iteration. See \code{\link{edgeWeight}}}
    #'   
    edgeWeight=function(source,destination){
      # if(alg>=1 & alg<=10000){
      #   return(dc$edgeWeight(source,destination))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$edgeWeight(source,destination))
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \code{\link{vertices}}}
    #'   
    vertices=function(community){
      # if(alg>=1 & alg<=10000){
      #   return(dc$vertices(community))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$vertices(community))
      # }
      # else{
        return(list())
      # }
    },
    
    #' 
    #'   \item{edgeCount()}{Get the number of vertex to vertex edges in the graph. See \code{\link{edgeCount}}}
    #'   
    edgeCount=function() {
      # if(alg>=1 & alg<=10000){
      #   return(dc$edgeCount())
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$edgeCount())
      # }
      # else{
        return(NA)
      # }
    },
    
    #' 
    #'   \item{communityMapping(differential)}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingMatrix = function(differential=TRUE){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityMappingMatrix(differential))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityMappingMatrix(differential))
      # }
      # else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("vertex","community"))))
      # }
    },
    
    #' 
    #'   \item{communityMapping(differential)}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingFile = function(differential=TRUE,file=""){
      # if(alg>=1 & alg<=10000){
      #   return(dc$communityMappingFile(prm[which(prm=="cv"),2], differential,file))
      # }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityMappingFile(prm[which(prm=="cv"),2],differential,file))
      # }
      # else{
        return(matrix(nrow=0,ncol=1,byrow=TRUE,dimnames = list(c(),c("reply"))))
      # }
    },
    
    #' 
    #'   \item{time()}{Get the cumulative time spent on processing after the last iteration. See \code{\link{time}}}
    #'   
    time=function(differential=FALSE){
      # if(alg==){
      #   return(dc$time(differential))
      # }
      # else if(alg==){
      #   return(dc$time(differential))
      # }
      # else{
        return(NA)
      # }
    }
    
  )
  # close methods section of the documentation
  #' 
  #' }
  #' 

  ## Define the value of the list within the current environment.
  assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"DynCommMainR")
  return(me)
}
