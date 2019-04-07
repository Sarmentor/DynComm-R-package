
########################### Package Documentation ###########################

#' @name DynComm-package
#' 
#' @aliases dyncomm-package Dyncomm-package
#' 
#' @title DynComm: Dynamic Network Communities Detection
#' 
#' @description 
#' Bundle of algorithms used for evolving network analysis regarding 
#' community detection.
#' Implements several algorithms, using a common API, that calculate 
#' communities for graphs whose nodes and edges change over time.
#' Edges, which can have new nodes, can be added or deleted, and changes in the 
#' communities are calculated without recalculating communities for the 
#' entire graph.
#' 
#' @docType package
#' @import Rcpp methods
#' @useDynLib DynComm
NULL

########################### API Documentation ###########################

#' @name ALGORITHM
#'
#' @aliases algorithm Algorithm
#' 
#' @title List of available algorithms.
#' 
#' @description 
#' An algorithm mainly defines how nodes and/or communities are processed,
#' when quality measurements occur and what happens to the communities
#' depending on the value of the quality obtained.
#'
#' @usage ALGORITHM$algorithm
#' 
#' @format A named list with the names of the available algorithms:
#'  \describe{
#'   \item{algorithm}{See available algorithms below.}
#'  }
#'  
#' @section Currently supported algorithms:
#' \describe{
#'   \item{Louvain}{is a greedy optimization method to extract communities 
#'             from large networks by optimizing the density of edges 
#'             inside communities to edges outside communities.}
#' }
#'
# @section Algorithms expected to be supported in the future:
# \describe{
#   \item{TILES}{}
#   \item{eTILES}{}
# }
#' 
#' @examples
#' ALGORITHM$LOUVAIN
# ALGORITHM$TILES
#' 
#' @export
#'
ALGORITHM <- list(
  # C++ algorithms are listed from 1 to 10000
  LOUVAIN=1L
  # ,SHAKEN=2L
  # Python algorithms are listed from 10001 to 20000
  # ,TILES=10001L
  # ,ETILES=10002L
  )

#' @name QUALITY
#'
#' @aliases quality criterion CRITERION
#' 
#' @title List of available quality measurement functions (criterion).
#' 
#' @description 
#' A quality measurement function mainly returns a value that indicates 
#' the proximity of the current grouping of nodes (communities) to the
#' optimum one. Theoretically, the bigger the value, the closer the 
#' current grouping is to the best possible grouping.
#' 
#' Each QUALITY internally defines two functions. One is used to 
#' evaluate if moving a node from one group (community) to another 
#' possibly yields a better overall result. The other is used to measure 
#' the actual overall quality of the entire grouping (current community 
#' mapping).
#'
#' @usage QUALITY$quality
#' 
#' @format A named list with the names of the available QUALITY:
#'  \describe{
#'   \item{quality}{See available quality below.}
#'  }
#'  
#' @section Currently supported QUALITY are:
#' \describe{
#'   \item{MODULARITY}{Newman-Girvan}
#   \item{BALMOD}{Balanced Modularity}
#' }
#'
# @section Quality measurement functions expected to be supported in the future:
#'
#' @examples
#' QUALITY$MODULARITY
# QUALITY$BALMOD
#' 
#' @export
#'
QUALITY <- list(
  # C++ quality measures are listed from 1 to 10000
  MODULARITY=1L
  # ,BALMOD=2L
  # Python quality measures are listed from 10001 to 20000
)

#' @name DynComm
#' 
#' @aliases Dyncomm dyncomm
#' 
#' @title DynComm
#'
#' @description 
#' This class provides a single interface for all algorithms in the different 
#' languages.
#' It provides methods to get results of processing and to interact with the 
#' nodes, edges and communities.
#'
# @details to do
#' 
#' @rdname DynComm
#' 
#' @docType class
#' 
#' @usage DynComm(algorithm, quality, parameters)
#' 
#' @param algorithm One of the available algorithms. See \code{\link{ALGORITHM}}
#' 
#' @param quality One of the available quality measurement functions. See \code{\link{QUALITY}}
#' 
#' @param parameters A two column matrix defining additional parameters. See
#'   the Parameters section on this page
#'
#' @return \code{DynComm} object
#'
#' @export
#'
#' @examples
# parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$addRemoveEdgesFile("initial_graph.txt")
#' dc$communityCount()
#' dc$communities()
#' dc$communityNodeCount(1)
#' dc$nodes(1)
#' dc$communityMapping(TRUE)
#' dc$time()
#' dc$addRemoveEdgesFile("s0000000000.txt")
#'
#' @section Parameters:
#' A two column matrix defining additional parameters to be passed to the
#' selected algorithm and quality measurement function.
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
#' }
#' 
#' @section Methods:
#' \describe{

# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
DynComm <- function(Algorithm,Quality,parameters)
{
  
  ## Get the environment for this
  ## instance of the function.
  # thisEnv <- environment()
  
  alg <- Algorithm
  qlt <- Quality
  prm <- parameters
  if(alg>=1 & alg<=10000){
    dc <- new(DynCommR,alg,qlt,prm)
    # print(dc)
    # TO DO: check for errors
  }
  else{
    print("Unknown algorithm :(")
  }
  # print(dc)

  ## Create the list used to represent an
  ## object for this class
  me <- list(
    
    ## Define the environment where this list is defined so
    ## that I can refer to it later.
    # thisEnv = thisEnv,
    
    ## Define the accessors for the data fields.
    # getEnv = function()
    # {
    #   return(get("thisEnv",thisEnv))
    # },

    #' 
    #'   \item{getAlgorithm()}{Get the algorithm being used. See \link{DynComm$getAlgorithm}}
    #'   
    getAlgorithm = function()
    {
      # return(alg)
      ALGORITHM[alg]
    },

    #' 
    #'   \item{addRemoveEdgesFile(graphAddRemoveFile)}{Add and remove edges read from a file. See \link{DynComm$addRemoveEdgesFile}}
    #'   
    addRemoveEdgesFile = function(graphAddRemoveFile){
      if(alg>=1 & alg<=10000){
        dc$addRemoveEdgesFile(graphAddRemoveFile)
      }
      else{
        print("Unknown :(")
      }
    },
    
    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \link{DynComm$quality}}
    #'   
    quality=function(){
      if(alg>=1 & alg<=10000){
        dc$quality()
      }
      else{
        print("Unknown :(")
      }
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \link{DynComm$communityCount}}
    #'   
    communityCount=function(){
        if(alg>=1 & alg<=10000){
          dc$communityCount()
        }
        else{
          print("Unknown :(")
        }
      },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \link{DynComm$communities}}
    #'   
    communities=function(){
        if(alg>=1 & alg<=10000){
          dc$communities()
        }
        else{
          print("Unknown :(")
        }
      },
    
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \link{DynComm$communityInnerEdgesWeight}}
    #'   
    communityInnerEdgesWeight=function(community){
      if(alg>=1 & alg<=10000){
        dc$communityInnerEdgesWeight(community)
      }
      else{
        print("Unknown :(")
      }
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \link{DynComm$communityTotalWeight}}
    #'   
    communityTotalWeight=function(community){
        if(alg>=1 & alg<=10000){
          dc$communityTotalWeight(community)
        }
        else{
          print("Unknown :(")
        }
      },
      
        
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \link{DynComm$communityEdgeWeight}}
    #'   
    communityEdgeWeight=function(source,destination){
          if(alg>=1 & alg<=10000){
            dc$communityEdgeWeight(source,destination)
          }
          else{
            print("Unknown :(")
          }
        },
        
    #' 
    #'   \item{communityNodeCount(community)}{Get the amount of nodes in the given community after the last iteration. See \link{DynComm$communityNodeCount}}
    #'   
    communityNodeCount=function(community){
          if(alg>=1 & alg<=10000){
            dc$communityNodeCount(community)
          }
          else{
            print("Unknown :(")
          }
        },
        
    #' 
    #'   \item{community(node)}{Get the community of the given node after the last iteration. See \link{DynComm$community}}
    #'   
    community=function(node){
          if(alg>=1 & alg<=10000){
            dc$community(node)
          }
          else{
            print("Unknown :(")
          }
        },
        
    #' 
    #'   \item{nodesCount()}{Get the total number of nodes after the last iteration. See \link{DynComm$nodesCount}}
    #'   
    nodesCount=function(){
          if(alg>=1 & alg<=10000){
            dc$nodesCount()
          }
          else{
            print("Unknown :(")
          }
        },
        
    #' 
    #'   \item{nodesAll()}{Get all nodes in the graph after the last iteration. See \link{DynComm$nodesAll}}
    #'   
    nodesAll=function(){
          if(alg>=1 & alg<=10000){
            dc$nodesAll()
          }
          else{
            print("Unknown :(")
          }
        },
        
    #' 
    #'   \item{nodes(community)}{Get all nodes belonging to the given community after the last iteration. See \link{DynComm$nodes}}
    #'   
    nodes=function(community){
            if(alg>=1 & alg<=10000){
              dc$nodes(community)
            }
            else{
              print("Unknown :(")
            }
          },
        
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \link{DynComm$communityMapping}}
    #'   
    communityMapping = function(){
      if(alg>=1 & alg<=10000){
        dc$communityMapping(TRUE)
      }
      else{
        print("Unknown :(")
      }
    },
    
    #' 
    #'   \item{time()}{Get the cumulative time spent on processing after the last iteration. See \link{DynComm$time}}
    #'   
    time=function(){
          if(alg>=1 & alg<=10000){
            dc$time()
          }
          else{
            print("Unknown :(")
          }
        }
      
    
  )
  #' 
  #' }
  #' 
  
  
  ## Define the value of the list within the current environment.
  # assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"DynComm")
  return(me)
}


#' @name DynComm$getAlgorithm
#' 
#' @aliases getAlgorithm
#' 
#' @title getAlgorithm()
#'
#' @description This method returns the algorithm with which this object was initialized.
#' 
# @details to do
#'
#' @rdname getAlgorithm
#' 
#' @docType methods
#' 
#' @usage getAlgorithm()
#' 
#' @method DynComm getAlgorithm
#' 
#' @return ALGORITHM
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$getAlgorithm()
#' 
NULL

#' @name DynComm$addRemoveEdgesFile
#' 
#' @aliases addRemoveEdgesFile
#' 
#' @title addRemoveEdgesFile(graphAddRemoveFile)
#'
#' @description 
#' This method reads edges from a file and adds or removes them from the graph.
#' 
#' The file must have only one edge per line, with values separated by a white
#' space (both SPACE and TAB work in any amount and combination).
#' 
#' The first value is the source node, the second is the destination node, and 
#' the third is the weight.
#' 
#' The weight can be ommited if the edge is to be added using the default weight
#' of 1 (one), or if the parameter to ignore weights was set.
#' 
#' If the weight is exactly zero, the edge is removed from the graph.
#' 
#' If a node, mentioned in the source or destination, does not exist it will be 
#' added to the graph.
#' 
#' The method detects automatically if the weight is present on a row by row 
#' basis so some rows may have weights defined and others not.
#' 
# @details to do
#'
#' @rdname addRemoveEdgesFile
#' 
#' @docType methods
#' 
#' @usage addRemoveEdgesFile(graphAddRemoveFile)
#' 
#' @method DynComm addRemoveEdgesFile
#' 
#' @return FALSE if any kind of error occurred. Otherwise, TRUE
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$addRemoveEdgesFile("graphAddRemoveFile.txt")
#' 
NULL

#' @name DynComm$quality
#'
#' @aliases quality
#'
#' @title quality()
#'
#' @description 
#' Get the quality measurement of the graph after the last iteration of the 
#' algorithm.
#'
# @details to do
#'
#' @rdname quality
#'
#' @docType methods
#'
#' @usage quality()
#'
#' @method DynComm quality
#'
#' @return a floating point number
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$quality()
#'
NULL

#' @name DynComm$communityCount
#'
#' @aliases communityCount
#'
#' @title communityCount()
#'
#' @description Get the number of communities after the last iteration of the algorithm.
#'
# @details to do
#'
#' @rdname communityCount
#'
#' @docType methods
#'
#' @usage communityCount()
#'
#' @method DynComm communityCount
#'
#' @return an unsigned integer value with the number of communities
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$communityCount()
#'
NULL

#' @name DynComm$communities
#'
#' @aliases communities
#'
#' @title communities()
#'
#' @description 
#' This method returns all communities after the last iteration.
#'
# @details to do
#'
#' @rdname communities
#'
#' @docType methods
#'
#' @usage communities()
#'
#' @method DynComm communities
#'
#' @return a list of all communities
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$communities()
#'
NULL

#' @name DynComm$communityInnerEdgesWeight
#'
#' @aliases communityInnerEdgesWeight
#'
#' @title communityInnerEdgesWeight(community)
#'
#' @description 
#' Get the sum of weights of the inner edges of the given community after the 
#' last iteration.
#'
# @details to do
#'
#' @rdname communityInnerEdgesWeight
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @usage communityInnerEdgesWeight(community)
#'
#' @method DynComm communityInnerEdgesWeight
#'
#' @return a floating point number with the weight
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$communityInnerEdgesWeight(1)
#'
NULL

#' @name DynComm$communityTotalWeight
#'
#' @aliases communityTotalWeight
#'
#' @title communityTotalWeight(community)
#'
#' @description 
#' Get the sum of weights of all edges of the given community after the last iteration.
#'
# @details to do
#'
#' @rdname communityTotalWeight
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @usage communityTotalWeight(community)
#'
#' @method DynComm communityTotalWeight
#'
#' @return a floating point number with the weight
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$communityTotalWeight(1)
#'
NULL

#' @name DynComm$communityEdgeWeight
#'
#' @aliases communityEdgeWeight
#'
#' @title communityEdgeWeight(source,destination)
#'
#' @description 
#' Get the weight of the edge that goes from source to destination after the last iteration.
#'
# @details to do
#'
#' @rdname communityEdgeWeight
#'
#' @docType methods
#'
#' @param source The name of the source node that is part of the edge
#' 
#' @param destination The name of the destination node that is part of the edge
#' 
#' @usage communityEdgeWeight(source,destination)
#'
#' @method DynComm communityEdgeWeight
#'
#' @return a floating point number with the weight
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$communityEdgeWeight(12,42)
#'
NULL

#' @name DynComm$communityNodeCount
#'
#' @aliases communityNodeCount
#'
#' @title communityNodeCount(community)
#'
#' @description 
#' Get the amount of nodes in the given community after the last iteration.
#'
# @details to do
#'
#' @rdname communityNodeCount
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @usage communityNodeCount(community)
#'
#' @method DynComm communityNodeCount
#'
#' @return an unsigned integer with the number nodes in the given community
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$communityNodeCount(3)
#'
NULL

#' @name DynComm$community
#'
#' @aliases community
#'
#' @title community(node)
#'
#' @description 
#' Get the community of the given node after the last iteration.
#'
# @details to do
#'
#' @rdname community
#'
#' @docType methods
#'
#' @param node The name of the intended node
#' 
#' @usage community(node)
#'
#' @method DynComm community
#'
#' @return an unsigned integer with the community of the given node
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$community(8)
#'
NULL

#' @name DynComm$nodesCount
#'
#' @aliases nodesCount
#'
#' @title nodesCount()
#'
#' @description 
#' Get the total number of nodes after the last iteration. It can be useful since
#' nodes can be added, if an edge being added has nodes that do not exist in the 
#' graph, or removed, if they are not part of any edge after removing an edge.
#'
# @details to do
#'
#' @rdname nodesCount
#'
#' @docType methods
#'
#' @usage nodesCount()
#'
#' @method DynComm nodesCount
#'
#' @return an unsigned integer with the number of nodes in the graph
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$nodesCount()
#'
NULL

#' @name DynComm$nodesAll
#'
#' @aliases nodesAll
#'
#' @title nodesAll()
#'
#' @description 
#' Get all nodes in the graph after the last iteration.
#'
# @details to do
#'
#' @rdname nodesAll
#'
#' @docType methods
#'
#' @usage nodesAll()
#'
#' @method DynComm nodesAll
#'
#' @return a list of all nodes in the graph
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$nodesAll()
#'
NULL

#' @name DynComm$nodes
#'
#' @aliases nodes
#'
#' @title nodes(community)
#'
#' @description 
#' Get all nodes belonging to the given community after the last iteration.
#'
# @details to do
#'
#' @rdname nodes
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @usage nodes(community)
#'
#' @method DynComm nodes
#'
#' @return a list of nodes belonging to the given community
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$nodes(6)
#'
NULL

#' @name DynComm$communityMapping
#'
#' @aliases communityMapping
#'
#' @title communityMapping()
#'
#' @description 
#' Get the community mapping for all communities after the last iteration.
#'
# @details to do
#'
#' @rdname communityMapping
#'
#' @docType methods
#'
#' @usage communityMapping()
#'
#' @method DynComm communityMapping
#'
#' @return a two column matrix with communities in the first column and the nodes in the second
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$communityMapping()
#'
NULL

#' @name DynComm$time
#'
#' @aliases time
#'
#' @title time()
#'
#' @description 
#' Get the cumulative time spent on processing after the last iteration.
#'
# @details to do
#'
#' @rdname time
#'
#' @docType methods
#'
#' @usage time()
#'
#' @method DynComm time
#'
#' @return an unsigned integer with the total processing time
#'
#' @examples
#' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
#' dc$time()
#' ## 2.3
NULL

