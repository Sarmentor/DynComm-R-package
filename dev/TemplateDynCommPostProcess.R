########################### Developer Notice ###########################
# Description:
# This file holds the template for post processing algorithms.
#
# Processing of data should occur completely in the constructor. All other
# functions should only implement the retrieval of such data.
#
# Each function contains the description of the type of data received in
# the parameters and has implemented the default return data. The default 
# data should be kept to be returned in case of error
#
# New algorithms should have their name added to the list of algorithms
# (POSTPROCESSING) in the DynCommPostProcess.R file.
#
# For more information about the data that can be recived or returned by a 
# function, read the DynComm documentation.
#
# If the algorithm requires any R library to be imported, do NOT use the
# library R command since it is forbidden inside packages. Instead list it in
# DynComm-package documentation on the "DynComm.R" file
#
#
# Author: poltergeist0
#
# Date: 2019-01-01


########################### Include R sources here ###########################
#source("something.R")


########################### Algorithm Documentation ###########################
#' @name TemplateDynCommPostProcess
#' 
# do not remove the internal keyword. It indicates that the documentation
# should not be visible to end users
#' @keywords internal
#' 
#' @title TemplateDynCommPostProcess
#'
#' @author poltergeist0
#' 
#' @description 
#' Single paragraph with two to three lines maximum.
#' 
#' @details 
#' More details about the algorithm.
#'
#' @rdname TemplateDynCommPostProcess
#' 
#' @docType class
#' 
#' @usage TemplateDynCommPostProcess(dyncomm,Parameters)
#' 
# Do not change the parameters
#' @param dyncomm a DynCom post processing algorithm. See \code{\link{DynCommPostProcess}}
#' 
#' @param Parameters A two column matrix defining the parameters for this 
#' algorithm. See the PARAMETERS section on this page
#'
#' @return \code{TemplateDynCommPostProcess} object
#'
#' @section Performance:
#' This section should indicate if the algorithm requires additional computer
#' resources in two stages: initialization and results. It is important to list
#' these resources for the user to evaluate if this algorithm can be safely
#' run due to the size of their graph. As an example, if this algorithm requires
#' storing a copy of the graph and the main algorithm already occupies more than 
#' half of the computer's available memory, it is not safe to run it.
#' \describe{
#'   \item{Initialization}{
#'   List resources temporarily used in the constructor.
#'   }
#'   \item{Results}{
#'   List resources permanently used to store results and used to provide them
#'   through the functions.
#'   }
#' }
#' As an example:
#' \describe{
#'   \item{Initialization}{
#'   Temporarily stores a copy of the graph to calculate "something" with communities.
#'   }
#'   \item{Results}{
#'   Uses a matrix with three columns and verticesAll() rows with the new community
#'   mapping (vertex<->community) and a number representing "that thing".
#'   }
#' }
#' 
# do not export this object. It is only for internal use of the algorithm
# @export
#'
# List examples of using the algorithm as if it was used on its own
# This will be helpful for including in tests
#' @examples
#' \dontrun{
#' Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
#' dc<-DynCommMain(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
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
#
# List all parameters required by the algorithm
#
#' \describe{
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
TemplateDynCommPostProcess <- function(dyncomm, Parameters=NULL)
{
  ## Get the environment for this instance of the function.
  thisEnv <- environment()
  
  ########## constructor #############
  prv <- dyncomm  #previous object in the chain
  prm <- NULL  #parameters for this post processing algorithm
  
  if(is.null(Parameters)){
    #set default parameters
  }
  else{
    # TODO validate parameters
    assign("prm",Parameters,thisEnv)
  }
  ########## do something in the constructor #############
  ########## implement #############
  # replace the condition of the following if statement for an appropriate one
  if("constructor_failed") return(NULL)
  
  ## Create the list used to represent an object for this class
  me <- list(
    
    ## Define the environment where this list is defined so that I can refer to it later.
    thisEnv = thisEnv,
    
    # The next function is mandatory. It indicates to the parent object which
    # functions of the API are implemented by this object. Comment the if 
    # statements that are not used.
    # Tipically, an algorithm that only processes communities needs only to 
    # implement the functions that deal with communities.
    # Conversely, algorithms dealing with vertices need to implement all
    # functions
    has = function(apiFunction){
      # functions dealing with communities only start here
      if(apiFunction==APIFUNCTIONS$COMMUNITIES){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITIESEDGECOUNT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITY){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYCOUNT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYEDGEWEIGHT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYINNEREDGESWEIGHT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYMAPPINGFILE){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYMAPPINGMATRIX){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYNEIGHBOURS){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYTOTALWEIGHT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITYVERTEXCOUNT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$QUALITY){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$VERTICES){
        return(TRUE)
      }
      # algorithms dealing with vertices need to implement these too
      else if(apiFunction==APIFUNCTIONS$EDGEWEIGHT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$EDGECOUNT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$NEIGHBOURS){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$VERTEXCOUNT){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$VERTICESALL){
        return(TRUE)
      }
      # the next function may or may not need to be implemented
      else if(apiFunction==APIFUNCTIONS$RESULTS){
        return(TRUE)
      }
      # do NOT change or comment the else statement
      else{#function not implemented
        return(FALSE)
      }
    },
    
    #' 
    #'   \item{results(differential)}{Get additional results of the algorithm or the currently selected post processing steps. See \code{\link{results}}}
    #'   
    # differential is boolean
    results = function(differential=TRUE)
    {
      ########## implement #############
      return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("name","value"))))
    },
    
    #' 
    #'   \item{addRemoveEdges(graphAddRemove)}{Add and remove edges read from a file. See \code{\link{addRemoveEdges}}}
    #'
    # graphAddRemoveFile is a string representing a file with path to read from
    addRemoveEdgesFile = function(graphAddRemoveFile){
      ########## implement #############
      return(FALSE)
    },
    
    #' 
    #'   \item{addRemoveEdges(graphAddRemove)}{Add and remove edges read from a matrix. See \code{\link{addRemoveEdges}}}
    #'   
    # graphAddRemoveMatrix is a matrix with two or three columns
    addRemoveEdgesMatrix = function(graphAddRemoveMatrix){
      ########## implement #############
      return(FALSE)
    },
    
    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \code{\link{quality}}}
    #'   
    quality=function(){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \code{\link{communityCount}}}
    #'   
    communityCount=function(){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \code{\link{communities}}}
    #'   
    communities=function(){
      ########## implement #############
      return(list())
    },
    
    #' 
    #'   \item{communitiesEdgeCount()}{Get the number of community to community edges in the graph. See \code{\link{communitiesEdgeCount}}}
    #'   
    communitiesEdgeCount=function(){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{communityNeighbours(community)}{Get the neighbours of the given community after the last iteration. See \code{\link{communityNeighbours}}}
    #'   
    # community is an unsigned integer
    communityNeighbours=function(community){
      ########## implement #############
      return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
    },
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \code{\link{communityInnerEdgesWeight}}}
    #'   
    # community is an unsigned integer
    communityInnerEdgesWeight=function(community){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \code{\link{communityTotalWeight}}}
    #'   
    # community is an unsigned integer
    communityTotalWeight=function(community){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \code{\link{communityEdgeWeight}}}
    #'   
    # source is an unsigned integer
    # destination is an unsigned integer
    communityEdgeWeight=function(source,destination){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \code{\link{communityVertexCount}}}
    #'   
    # community is an unsigned integer
    communityVertexCount=function(community){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \code{\link{community}}}
    #'   
    # vertex is an unsigned integer
    community=function(vertex){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \code{\link{vertexCount}}}
    #'   
    vertexCount=function(){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{verticesAll()}{Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}}
    #'   
    verticesAll=function(){
      ########## implement #############
      return(list())
    },
    
    #' 
    #'   \item{neighbours(vertex)}{Get the neighbours of the given vertex after the last iteration. See \code{\link{neighbours}}}
    #'   
    # vertex is an unsigned integer
    neighbours=function(vertex){
      ########## implement #############
      return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
    },
    
    #' 
    #'   \item{edgeWeight(source,destination)}{Get the weight of the edge that goes from source vertex to destination vertex after the last iteration. See \code{\link{edgeWeight}}}
    #'   
    # source is an unsigned integer
    # destination is an unsigned integer
    edgeWeight=function(source,destination){
      ########## implement #############
      return(NA)
    },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \code{\link{vertices}}}
    #'   
    # community is an unsigned integer
    vertices=function(community){
      ########## implement #############
      return(list())
    },
    
    #' 
    #'   \item{edgeCount()}{Get the number of vertex to vertex edges in the graph. See \code{\link{edgeCount}}}
    #'   
    edgeCount=function(){
      ########## implement #############
      return(NA)
    }
    
    #' 
    #'   \item{communityMapping(differential)}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    # differential is a boolean
    communityMappingMatrix = function(differential=TRUE){
      ########## implement #############
      return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("name","value"))))
    },
    
    #' 
    #'   \item{communityMapping(differential)}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    # differential is a boolean
    # file is a string representing a file with path to read from. Is never NULL
    communityMappingFile = function(differential=TRUE,file=""){
      ########## implement #############
      return(matrix(data=c(FALSE),nrow=1,ncol=1,byrow=TRUE,dimnames = list(c(),c("reply"))))
    }
    
  )
  # close methods section of the documentation
  #' 
  #' }
  #' 
  
  ## Define the value of the list within the current environment.
  assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"TemplateDynCommPostProcess")
  return(me)
}
