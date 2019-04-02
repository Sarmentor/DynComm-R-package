
########################### Package Documentation ###########################

#' DynComm: Dynamic Network Communities Detection
#' 
#' This package is used for evolving network analysis regarding community detection.
#' Implements several algorithms that calculate communities for graphs whose nodes and edges change over time.
#' Edges, which can have new nodes, can be added or deleted.
#' Changes in the communities are calculated without recalculating communities for the entire graph.
#' 
#' @docType package
#' @name DynComm-package
NULL

########################### API Documentation ###########################

#' ALGORITHM
#'
#' List of available algorithms.
#' 
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
#' @section Algorithms expected to be supported in the future:
#' \describe{
#'   \item{TILES}{}
#'   \item{eTILES}{}
#' }
#' 
#' @examples
#' ALGORITHM$LOUVAIN
#' ALGORITHM$TILES
#' 
#' @export
#'
ALGORITHM <- list(
  # C++ algorithms are listed from 1 to 10000
  LOUVAIN=1L
  ,SHAKEN=2L
  # Python algorithms are listed from 10001 to 20000
  ,TILES=10001L
  ,ETILES=10002L
  )

#' QUALITY
#'
#' List of available quality measurement functions.
#' 
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
#' @format A named list with the names of the available algorithms:
#'  \describe{
#'   \item{quality}{See available quality below.}
#'  }
#'  
#' @section Currently supported quality measurement functions are:
#' \describe{
#'   \item{MODULARITY}{TO DO}
#'   \item{BALMOD}{TO DO}
#' }
#'
#' @section Quality measurement functions expected to be supported in the future:
#'
#' @examples
#' QUALITY$MODULARITY
#' QUALITY$BALMOD
#' 
#' @export
#'
QUALITY <- list(
  # C++ quality measures are listed from 1 to 10000
  MODULARITY=1L
  ,BALMOD=2L
  # Python quality measures are listed from 10001 to 20000
)

#' DynComm
#'
#' This class provides a single interface for all algorithms in the different languages.
#' It provides methods to get results of processing and to interact with the nodes, edges and communities.
#'
#' @rdname DynComm
#' 
#' @docType class
#' 
#' @section A Custom Section:
#'
#' Text accompanying the custom section.
#'
#' @param x A description of the parameter 'x'. The
#'   description can span multiple lines.
#' @param y A description of the parameter 'y'.
#'
#' @return \code{NULL}
#'
#' @export
#'
#' @examples
# parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE)
# dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
# dc$communityCount()
# dc$communities()
# dc$communityNodeCount(1)
# dc$nodes(1)
# dc$communityMapping(TRUE)
# dc$time()
# dc$addRemoveEdgesFile("test/full/sequences/s0000000000.txt")
#'
#' ## don't run this in calls to 'example(add_numbers)'
#' \dontrun{
#'    add_numbers(2, 3)
#' }
#'
#' ## don't test this during 'R CMD check'
#' \donttest{
#'    add_numbers(4, 5)
#' }

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
    #'
    #' getAlgorithm
    #'
    #' This method returns the algorithm with which this object was initialized.
    #'
    #' @rdname getAlgorithm
    #' 
    #' @docType method
    #' 
    #' @method DynComm getAlgorithm
    #'
    #' @examples
    #' dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
    #' dc$getAlgorithm()
    #'
    #' ## don't run this in calls to 'example(add_numbers)'
    #' \dontrun{
    #'    add_numbers(2, 3)
    #' }
    #'
    #' ## don't test this during 'R CMD check'
    #' \donttest{
    #'    add_numbers(4, 5)
    #' }
    getAlgorithm = function()
    {
      # return(alg)
      ALGORITHM[alg]
    },

    addRemoveEdgesFile = function(graphAddRemoveFile){
      if(alg>=1 & alg<=10000){
        dc$addRemoveEdgesFile(graphAddRemoveFile)
      }
      else{
        print("Unknown :(")
      }
    },
    
    quality=function(){
      if(alg>=1 & alg<=10000){
        dc$quality()
      }
      else{
        print("Unknown :(")
      }
    },
    
      communityCount=function(){
        if(alg>=1 & alg<=10000){
          dc$communityCount()
        }
        else{
          print("Unknown :(")
        }
      },
    
      communities=function(){
        if(alg>=1 & alg<=10000){
          dc$communities()
        }
        else{
          print("Unknown :(")
        }
      },
    
    
    communityInnerEdgesWeight=function(community){
      if(alg>=1 & alg<=10000){
        dc$communityInnerEdgesWeight(community)
      }
      else{
        print("Unknown :(")
      }
    },
    
      communityTotalWeight=function(community){
        if(alg>=1 & alg<=10000){
          dc$communityTotalWeight(community)
        }
        else{
          print("Unknown :(")
        }
      },
      
        
        communityEdgeWeight=function(source,destination){
          if(alg>=1 & alg<=10000){
            dc$communityEdgeWeight(source,destination)
          }
          else{
            print("Unknown :(")
          }
        },
        
        communityNodeCount=function(community){
          if(alg>=1 & alg<=10000){
            dc$communityNodeCount(community)
          }
          else{
            print("Unknown :(")
          }
        },
        
        community=function(node){
          if(alg>=1 & alg<=10000){
            dc$community(node)
          }
          else{
            print("Unknown :(")
          }
        },
        
        nodesCount=function(){
          if(alg>=1 & alg<=10000){
            dc$nodesCount()
          }
          else{
            print("Unknown :(")
          }
        },
        
        nodesAll=function(){
          if(alg>=1 & alg<=10000){
            dc$nodesAll()
          }
          else{
            print("Unknown :(")
          }
        },
        
          nodes=function(community){
            if(alg>=1 & alg<=10000){
              dc$nodes(community)
            }
            else{
              print("Unknown :(")
            }
          },
        
    communityMapping = function(){
      if(alg>=1 & alg<=10000){
        dc$communityMapping(TRUE)
      }
      else{
        print("Unknown :(")
      }
    },
    
        time=function(){
          if(alg>=1 & alg<=10000){
            dc$time()
          }
          else{
            print("Unknown :(")
          }
        }
      
    
  )
  
  ## Define the value of the list within the current environment.
  # assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"DynComm")
  return(me)
}
