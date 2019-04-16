
########################### API Documentation ###########################

#' @name ALGORITHM
#'
#' @aliases algorithm Algorithm
#' 
#' @title List of available algorithms.
#' 
#' @description 
#' An algorithm mainly defines how vertices and/or communities are processed,
#' when criterion is applyed (quality measurements occur) and what happens 
#' to the communities depending on the value of the quality obtained.
#'
#' @usage ALGORITHM
#' 
#' @format A named list with the names of the available algorithms:
#'  \describe{
#'    \item{LOUVAIN}{is a greedy optimization method to extract communities 
#'             from large networks by optimizing the density of edges 
#'             inside communities to edges outside communities.
#'             @references \insertRef{cordeiro2016dynamic}{DynComm}
#'               
#'          }
#'  }
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

#' @name CRITERION
#'
#' @aliases criterion Criterion
#' 
#' @title List of available CRITERION (quality measurement functions).
#' 
#' @description 
#' A criterion is used to indicate the proximity of the current grouping 
#' of vertices (communities) to the optimum one. Theoretically, the bigger 
#' the value, the closer the current grouping is to the best possible 
#' grouping.
#' 
#' Each CRITERION internally defines two functions. One is used to 
#' evaluate if moving a vertex from one group (community) to another 
#' possibly yields a better overall result. The other is used to measure 
#' the actual overall quality of the entire grouping (current community 
#' mapping).
#'
#' @usage CRITERION
#' 
#' @format A named list with the names of the available CRITERION:
#' \describe{
#'   \item{MODULARITY}{Newman-Girvan}
#   \item{BALMOD}{Balanced Modularity}
#'}
#'  
#' @examples
#' CRITERION$MODULARITY
# CRITERION$BALMOD
#' 
#' @export
#'
CRITERION <- list(
  # C++ criterion are listed from 1 to 10000
  MODULARITY=1L
  # ,BALMOD=2L
  # Python criterion are listed from 10001 to 20000
)

#' @name DynComm
#' 
#' @aliases Dyncomm dyncomm
#' 
#' @title DynComm
#'
#' @description 
#' Provides a single interface for all algorithms in the different 
#' languages.
#' Includes methods to get results of processing and to interact with the 
#' vertices, edges and communities.
#'
#' @rdname DynComm
#' 
#' @docType class
#' 
#' @usage DynComm(Algorithm,Criterion,Parameters)
#' 
#' @param Algorithm One of the available ALGORITHM See \code{\link{ALGORITHM}}
#' 
#' @param Criterion One of the available CRITERION. See \code{\link{CRITERION}}
#' 
#' @param Parameters A two column matrix defining additional parameters. See
#'   the PARAMETERS section on this page
#'
#' @return \code{DynComm} object
#'
#' @export
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
#' }
#' 
#' @section Methods:
#' \describe{

# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
DynCommBase <- function(Algorithm,Criterion,Parameters)
{
  
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  alg <- Algorithm
  qlt <- Quality
  prm <- parameters
  pst <- POSTPROCESSING$NONE  #post processing flag redirects function calls to post processing object. Set to NONE on add/remove edge
  pstid <- 1
  act <- NULL   #list of actions and their parameters to recreate after adding/removing edges. It is delayed until requested.
  prc <- NULL   #pointer to last object in list of post processing objects. They are chained in series
  
  ########## constructor #############
  if(alg>=1 & alg<=10000){
    dc <- new(DynCommR,alg,qlt,prm)
    # print(dc)
    # TO DO: check for errors
  }
  else if(alg>=10001 & alg<=20000){
    # print("Python algorithm")
    dc <- reticulate::import_from_path("DynCommPython","../base/Python")
    # calls to python should be equal to calls to c++ but may need separate processing of outputs
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
    thisEnv = thisEnv,
    
    ## Define the accessors for the data fields.
    # getEnv = function()
    # {
    #   return(get("thisEnv",thisEnv))
    # },

    #' 
    #'   \item{postProcess(actions)}{Set a list of post processing steps. See \link{}}
    #'   
    # postProcess = function(actions){
    #   act <- actions
    #   to do: recreate chain and prc. Set pst to last link in chain (prc)
    # }
    
    #' 
    #'   \item{select(postProcessing)}{Select between getting the results of the algorithm or one of the post processing steps. See \link{}}
    #'   
    # select = function(postProcessing, id=1)
    # {
    #   if(is.null(prc)){
    #     #recreate post processing chain
    #     to do
    #   }
    #   if(prc$exists(postProcessing, id)){
    #     # pst <- postProcessing
    #     # pstid <- id
    #     assign("pst",postProcessing,thisEnv)
    #     assign("pstid",id,thisEnv)
    #     return(TRUE)
    #   }
    #   else{
    #     #there is no such post processing
    #     return(FALSE)
    #   }
    # },
    
    #' 
    #'   \item{results()}{Get additional results of the algorithm or the currently selected post processing steps. See \link{}}
    #'   
    results = function()
    {
      if(pst==POSTPROCESSING$NONE){#get from algorithm
        to do
        
      }
      else{#get from post processing
        to do
        
      }
    },

    #' 
    #'   \item{addRemoveEdgesFile(graphAddRemoveFile)}{Add and remove edges read from a file. See \link{addRemoveEdgesFile}}
    #'   
    addRemoveEdgesFile = function(graphAddRemoveFile){
      # pst <- POSTPROCESSING$NONE
      # prc <- NULL
      assign("pst",POSTPROCESSING$NONE,thisEnv)
      assign("prc",NULL,thisEnv)
      if(alg>=1 & alg<=10000){
        dc$addRemoveEdgesFile(graphAddRemoveFile)
      }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$addRemoveEdgesFile(graphAddRemoveFile)
      }
      else{
        print("Unknown :(")
      }
    },

    #' 
    #'   \item{edgesFile(graphAddRemoveFile)}{Alias for addRemoveEdgesFile(). See \link{addRemoveEdgesFile}}
    #'   
    edgesFile = function(graphAddRemoveFile){
      addRemoveEdgesFile(graphAddRemoveFile)
    },
    
    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \link{quality}}
    #'   
    quality=function(){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
          dc$quality()
        }
        else if(alg>=10001 & alg<=20000){
          # print("Python algorithm")
          dc$quality()
        }
        else{
          print("Unknown :(")
        }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$quality(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \link{communityCount}}
    #'   
    communityCount=function(){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
          dc$communityCount()
        }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$communityCount()
      }
      else{
          print("Unknown :(")
        }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$communityCount(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \link{communities}}
    #'   
    communities=function(){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
          dc$communities()
        }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$communities()
      }
      else{
          print("Unknown :(")
        }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$communities(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
    
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \link{communityInnerEdgesWeight}}
    #'   
    communityInnerEdgesWeight=function(community){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
        dc$communityInnerEdgesWeight(community)
      }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$communityInnerEdgesWeight(community)
      }
      else{
        print("Unknown :(")
      }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$communityInnerEdgesWeight(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \link{communityTotalWeight}}
    #'   
    communityTotalWeight=function(community){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
          dc$communityTotalWeight(community)
        }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$communityTotalWeight(community)
      }
      else{
          print("Unknown :(")
        }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$communityTotalWeight(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
      
        
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \link{communityEdgeWeight}}
    #'   
    communityEdgeWeight=function(source,destination){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
            dc$communityEdgeWeight(source,destination)
          }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$communityEdgeWeight(source,destination)
      }
      else{
            print("Unknown :(")
          }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$communityEdgeWeight(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
        
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \link{communityVertexCount}}
    #'   
    communityVertexCount=function(community){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
            dc$communityVertexCount(community)
          }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$communityVertexCount(community)
      }
      else{
            print("Unknown :(")
          }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$communityVertexCount(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
        
    #' 
    #'   \item{communityNodeCount(community)}{Alias for communityVertexCount(). See \link{communityVertexCount}}
    #'   
    # communityNodeCount=function(community){
    #   communityVertexCount(community)
    # },
    
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \link{community}}
    #'   
    community=function(vertex){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
            dc$community(vertex)
          }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$community(vertex)
      }
      else{
            print("Unknown :(")
          }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$community(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
        
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \link{vertexCount}}
    #'   
    vertexCount=function(){
      # if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
            dc$vertexCount()
          }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$vertexCount()
      }
      else{
            print("Unknown :(")
          }
      # }
      # else{
      #   if(pst==POSTPROCESSING$DENSOPT){
      #     # print("Python algorithm")
      #     prc$vertexCount(pst,pstid)
      #   }
      #   else{
      #     print("Unknown :(")
      #   }
      # }
    },
    #' 
    #'   \item{nodesCount()}{Alias for vertexCount(). See \link{vertexCount}}
    #'   
    # nodesCount=function(){
    #   vertexCount()
    # },
    
    #' 
    #'   \item{vertexAll()}{Get all vertices in the graph after the last iteration. See \link{vertexAll}}
    #'   
    vertexAll=function(){
      # if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
            dc$vertexAll()
          }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$vertexAll()
      }
      else{
            print("Unknown :(")
          }
      # }
      # else{
      #   if(pst==POSTPROCESSING$DENSOPT){
      #     # print("Python algorithm")
      #     prc$vertexAll(pst,pstid)
      #   }
      #   else{
      #     print("Unknown :(")
      #   }
      # }
    },
        
    #' 
    #'   \item{nodesAll()}{Alias for vertexAll(). See \link{vertexAll}}
    #'   
    # nodesAll=function(){
    #   vertexAll()
    # },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \link{vertices}}
    #'   
    vertices=function(community){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
              dc$vertices(community)
            }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$vertices(community)
      }
      else{
              print("Unknown :(")
            }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$vertices(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
    
    #' 
    #'   \item{nodes(community)}{Alias for vertices(community). See \link{vertices}}
    #'   
    # nodes=function(community){
    #   vertices(community)
    # },
        
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \link{communityMapping}}
    #'   
    communityMapping = function(){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
        dc$communityMapping(TRUE)
      }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$communityMapping(TRUE)
      }
      else{
        print("Unknown :(")
      }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$communityMapping(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
      }
    },
    
    #' 
    #'   \item{time()}{Get the cumulative time spent on processing after the last iteration. See \link{time}}
    #'   
    time=function(){
      if(pst==POSTPROCESSING$NONE){
        if(alg>=1 & alg<=10000){
            dc$time()
          }
      else if(alg>=10001 & alg<=20000){
        # print("Python algorithm")
        dc$time()
      }
      else{
            print("Unknown :(")
          }
      }
      else{
        # if(pst==POSTPROCESSING$DENSOPT){
          # print("Python algorithm")
          prc$time(pst,pstid)
        # }
        # else{
        #   print("Unknown :(")
        # }
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
  class(me) <- append(class(me),"DynCommBase")
  return(me)
}
