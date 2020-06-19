########################### Developer Notice ###########################
# Description:
# This file holds all the DynComm main algorithms. It also holds the lists of 
# available algorithms (ALGORITHM) and criterion (CRITERION).
#
# Internally, this object, dispatches calls to objects that do the actual work.
#
# New algorithms should have their name added to the list of algorithms
# (ALGORITHM).
#
# New criterion should have their name added to the list of criterion
# (CRITERION).
#
# New main algorithms source code must be added to their corresponding files as 
# stated in the developer documentation.
#
# More developer information can be found in the project source page on GitHub at
# https://github.com/softskillsgroup/DynComm-R-package
#
#
# Author: poltergeist0
# Date: 2019-01-01

#include main algorithms implemented in R
source("R/DynCommMainR.R")

#include algorithms documentation
source("R/ALGORITHM.R")

#include criterion documentation
source("R/CRITERION.R")

########################### API Documentation ###########################

#' @name ALGORITHM
#'
#' @aliases algorithm Algorithm
#' 
#' @title List of available algorithms.
#' 
#' @author poltergeist0
#' 
#' @description 
#' An algorithm mainly defines how vertices and/or communities are processed,
#' when criterion is applyed (quality measurements occur) and what happens 
#' to the communities depending on the value of the quality obtained.
#'
#' @usage ALGORITHM
#' 
########## document new algorithms here #############
#' @format A named list with the names of the available algorithms:
#'  \describe{
#'    \item{LOUVAIN}{
#'      is a greedy optimization method to extract communities from large networks 
#'      by optimizing the density of edges inside communities to edges outside 
#'      communities. \cr
#'      See \code{\link{ALGORITHM_LOUVAIN}}\cr
#'      @references \insertRef{cordeiro2016dynamic}{DynComm}
#'    }
#'  }
#'  
#' @seealso \code{\link{DynComm}}
#' 
#' @examples
#' ALGORITHM$LOUVAIN
# ALGORITHM$TILES
#' 
# @export DynComm::ALGORITHM
#' 
#' @export
#'
########## list new algorithms here #############
ALGORITHM <- list(
  #### C++ algorithms are listed from 1 to 10000
  LOUVAIN=1L
  # ,SHAKEN=2L
  #### Python algorithms are listed from 10001 to 20000
  # ,TILES=10001L
  # ,ETILES=10002L
  #### R algorithms are listed from 20001 to 30000
  )

#' @name CRITERION
#'
#' @aliases criterion Criterion
#' 
#' @title List of available CRITERION (quality measurement functions).
#' 
#' @author poltergeist0
#' 
#' @description 
#' A criterion is used to indicate the proximity of the current grouping 
#' of vertices (communities) to the optimum one. 
#' 
#' @details 
#' Theoretically, the bigger the value returned by the criterion, the closer the
#' current grouping is to the best possible grouping.
#' 
#' Each CRITERION internally defines two functions. One is used to 
#' evaluate if moving a vertex from one group (community) to another 
#' possibly yields a better overall result. The other is used to measure 
#' the actual overall quality of the entire grouping (current community 
#' mapping).
#' 
#' Not all criterion might be available for all algorithms. See each algorithms'
#' help to find which criterion is supported
#'
#' @usage CRITERION
#' 
########## document new criterion here #############
#' @format A named list with the names of the available CRITERION:
#' \describe{
#'  \item{MODULARITY}{
#'    Newman-Girvan \cr
#'    See \code{\link{CRITERION_MODULARITY}}
#'  }
#   \item{BALMOD}{Balanced Modularity}
#'}
#'  
#' @seealso \code{\link{DynComm}}
#' 
#' @examples
#' CRITERION$MODULARITY
# CRITERION$BALMOD
#' 
#' @export
#'
########## list new criterions here #############
CRITERION <- list(
  # C++ criterion are listed from 1 to 10000
  MODULARITY=1L
  # ,BALMOD=2L
  # Python criterion are listed from 10001 to 20000
)

########################### Main Algorithm Documentation ###########################
#' @name DynCommMain
#' 
#' @keywords internal
#' 
# @aliases Dyncommmain dyncommmain
#' 
#' @title DynCommMain
#'
#' @author poltergeist0
#' 
#' @description 
#' Provides a single interface for all main algorithms in the different 
#' languages.
#' 
#' @details 
#' Includes methods to get results of processing and to interact with the 
#' vertices, edges and communities.
#'
#' @rdname DynCommMain
#' 
# @docType class
#' 
#' @usage DynCommMain(Algorithm,Criterion,Parameters)
#' 
#' @param Algorithm One of the available ALGORITHM See \code{\link{ALGORITHM}}
#' 
#' @param Criterion One of the available CRITERION. See \code{\link{CRITERION}}
#' 
#' @param Parameters A two column matrix defining additional parameters. See
#'   the PARAMETERS section on this page
#'
#' @return \code{DynCommMain} object
#'
#' @seealso \code{\link{DynComm}}
#' 
# @export
#'
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
DynCommMain <- function(Algorithm,Criterion,Parameters)
{
  
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  ########## constructor #############
  alg <- Algorithm
  qlt <- Criterion
  prm <- Parameters

  if(alg>=1 & alg<=10000){
    dc <- new(DynCommRcpp,alg,qlt,prm)
    # print(dc)
    # TO DO: check for errors
  }
  # else if(alg>=10001 & alg<=20000){
  #   # print("Python algorithm")
  #   dc <- reticulate::import_from_path("DynCommPython","../src/base/Python")
  #   # calls to python should be equal to calls to c++ but may need separate processing of outputs
  # }
  else if(alg>=20001 & alg<=30000){
    # print("R algorithm")
    dc <- DynCommMainR(alg,qlt,prm)
  }
  else{
    dc<-NULL
    print("Unknown algorithm :(")
  }

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
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$results(differential))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$results(differential))
      # }
      else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("name","value"))))
      }
    },

    #' 
    #'   \item{addRemoveEdges(graphAddRemove)}{Add and remove edges read from a file. See \code{\link{addRemoveEdges}}}
    #'   
    addRemoveEdgesFile = function(graphAddRemoveFile){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$addRemoveEdgesFile(graphAddRemoveFile))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$addRemoveEdgesFile(graphAddRemoveFile))
      # }
      else{
        return(FALSE)
      }
    },

    #' 
    #'   \item{addRemoveEdges(graphAddRemove)}{Add and remove edges read from a matrix. See \code{\link{addRemoveEdges}}}
    #'   
    addRemoveEdgesMatrix = function(graphAddRemoveMatrix){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$addRemoveEdgesMatrix(graphAddRemoveMatrix))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$addRemoveEdgesMatrix(graphAddRemoveMatrix))
      # }
      else{
        return(FALSE)
      }
    },
    
    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \code{\link{quality}}}
    #'   
    quality=function(){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$quality())
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$quality())
      # }
      else{
        return(NA)
      }
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \code{\link{communityCount}}}
    #'   
    communityCount=function(){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityCount())
        }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityCount())
      # }
      else{
          return(NA)
        }
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \code{\link{communities}}}
    #'   
    communities=function(){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communities())
        }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communities())
      # }
      else{
        return(list())
        }
    },
    
    #' 
    #'   \item{communitiesEdgeCount()}{Get the number of community to community edges in the graph. See \code{\link{communitiesEdgeCount}}}
    #'   
    communitiesEdgeCount=function() {
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communitiesEdgeCount())
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communitiesEdgeCount())
      # }
      else{
        return(NA)
      }
    },
    
    #' 
    #'   \item{communityNeighbours(community)}{Get the neighbours of the given community after the last iteration. See \code{\link{communityNeighbours}}}
    #'   
    communityNeighbours=function(community){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityNeighbours(community))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityNeighbours(community))
      # }
      else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
      }
    },
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \code{\link{communityInnerEdgesWeight}}}
    #'   
    communityInnerEdgesWeight=function(community){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityInnerEdgesWeight(community))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityInnerEdgesWeight(community))
      # }
      else{
        return(NA)
      }
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \code{\link{communityTotalWeight}}}
    #'   
    communityTotalWeight=function(community){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityTotalWeight(community))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityTotalWeight(community))
      # }
      else{
        return(NA)
      }
    },

    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \code{\link{communityEdgeWeight}}}
    #'   
    communityEdgeWeight=function(source,destination){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityEdgeWeight(source,destination))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityEdgeWeight(source,destination))
      # }
      else{
        return(NA)
      }
    },
        
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \code{\link{communityVertexCount}}}
    #'   
    communityVertexCount=function(community){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityVertexCount(community))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityVertexCount(community))
      # }
      else{
        return(NA)
      }
    },
        
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \code{\link{community}}}
    #'   
    community=function(vertex){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$community(vertex))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$community(vertex))
      # }
      else{
        return(NA)
      }
    },
        
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \code{\link{vertexCount}}}
    #'   
    vertexCount=function(){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$vertexCount())
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$vertexCount())
      # }
      else{
        return(NA)
      }
    },

    #' 
    #'   \item{verticesAll()}{Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}}
    #'   
    verticesAll=function(){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$verticesAll())
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$verticesAll())
      # }
      else{
        return(list())
      }
    },
        
    #' 
    #'   \item{neighbours(vertex)}{Get the neighbours of the given vertex after the last iteration. See \code{\link{neighbours}}}
    #'   
    neighbours=function(vertex){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$neighbours(vertex))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$neighbours(vertex))
      # }
      else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
      }
    },
    
    #' 
    #'   \item{edgeWeight(source,destination)}{Get the weight of the edge that goes from source vertex to destination vertex after the last iteration. See \code{\link{edgeWeight}}}
    #'   
    edgeWeight=function(source,destination){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$edgeWeight(source,destination))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$edgeWeight(source,destination))
      # }
      else{
        return(NA)
      }
    },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \code{\link{vertices}}}
    #'   
    vertices=function(community){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$vertices(community))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$vertices(community))
      # }
      else{
        return(list())
      }
    },
    
    #' 
    #'   \item{edgeCount()}{Get the number of vertex to vertex edges in the graph. See \code{\link{edgeCount}}}
    #'   
    edgeCount=function() {
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$edgeCount())
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$edgeCount())
      # }
      else{
        return(NA)
      }
    },
    
    #' 
    #'   \item{communityMapping(differential)}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingMatrix = function(differential=TRUE){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityMappingMatrix(differential))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityMappingMatrix(differential))
      # }
      else{
        return(matrix(nrow=0,ncol=2,byrow=TRUE,dimnames = list(c(),c("vertex","community"))))
      }
    },
    
    #' 
    #'   \item{communityMapping(differential)}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingFile = function(differential=TRUE,file=""){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$communityMappingFile(prm[which(prm=="cv"),2], differential,file))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$communityMappingFile(prm[which(prm=="cv"),2],differential,file))
      # }
      else{
        return(matrix(nrow=0,ncol=1,byrow=TRUE,dimnames = list(c(),c("reply"))))
      }
    },
    
    #' 
    #'   \item{time()}{Get the cumulative time spent on processing after the last iteration. See \code{\link{time}}}
    #'   
    time=function(differential=FALSE){
      if((alg>=1 & alg<=10000) | (alg>=20001 & alg<=30000)){ # R and C++ calls are identical
        return(dc$time(differential))
      }
      # else if(alg>=10001 & alg<=20000){
      #   # print("Python algorithm")
      #   return(dc$time(differential))
      # }
      else{
        return(NA)
      }
    }
    
  )
  # close methods section of the documentation
  #' 
  #' }
  #' 

  ## Define the value of the list within the current environment.
  assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"DynCommMain")
  return(me)
}
