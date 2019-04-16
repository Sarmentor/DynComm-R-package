
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
#' communities for graphs whose vertices and edges change over time.
#' Edges, which can have new vertices, can be added or deleted, and changes in the 
#' communities are calculated without recalculating communities for the 
#' entire graph.
#' 
#' @docType package
#' @import Rcpp methods
#' @importFrom Rdpack reprompt
#' @useDynLib DynComm
#' 
# @references \url{https://en.en}
# @references
# \insertRef{cordeiro2016dynamic}{DynComm}
#
# \insertRef{R}{bibtex}
#' 
#' 
NULL

########################### API Documentation ###########################

#' @name POSTPROCESSING
#'
#' @aliases POST post postProcessing postprocessing
#' 
#' @title List of available post processing algorithms.
#' 
#' @description 
#' A post processing algorithm is a function that modifies the results 
#' presented to the user, allowing for limited result manipulation, but does not 
#' internally modify the results obtained by the algorithm.
#' 
#' As an example, we are only interested in viewing communities larger than some 
#' value but do not want to actually remove the smaller ones from the graph, 
#' invalidating possible future processing over them. A post processing 
#' algorithm can filter the unwanted values from the results or present a more 
#' compact version of them.
#'
#' @usage POSTPROCESSING
#' 
#' @format A named list with the names of the available algorithms:
#'  \describe{
#'    \item{DENSOPT}{Density optimization.
#'             @references \insertRef{densopt}{DynComm}
#'          }
#'  }
#'  
#' @examples
#' POSTPROCESSING$DENSOPT
#' 
#' @export
#'
POSTPROCESSING <- list(
  # C++ algorithms are listed from 1 to 10000
  NONE=1L #reserved to indicate no post processing
  # ,COUNTLOW=2L #filter out communities with edge count lower than a given value (high pass filter)
  # ,COUNTHIGH=3L #filter out communities with edge count higher than a given value (low pass filter)
  # ,COUNTBETWEEN=4L  #filter out communities with edge count between a given lower and higher value (band-stop or band-rejection filter)
  # ,COUNTTOP=5L  #get the top n communities with higher edge count
  # ,COUNTBOTTOM=6L  #get the bottom n communities with lower edge count
  # ,WEIGHTLOW=7L #filter out communities with total weight lower than a given value (high pass filter)
  # ,WEIGHTHIGH=8L #filter out communities with total weight higher than a given value (low pass filter)
  # ,WEIGHTBETWEEN=9L  #filter out communities with total weight between a given value lower and higher value (band-stop or band-rejection filter)
  # ,WEIGHTTOP=10L  #get the top n communities with higher total weight
  # ,WEIGHTBOTTOM=11L  #get the bottom n communities with lower total weight
  ,DENSOPT=100L
  # Python algorithms are listed from 10001 to 20000
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
DynComm <- function(Algorithm,Criterion,Parameters)
{
  
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  ########## constructor #############
  # chc <- Algorithm  #main algorithm choice
  alg <- DynCommBase(Algorithm,Criterion,Parameters)  #main algorithm
  # qlt <- Quality
  # prm <- Parameters  #main algorithm parameters
  pst <- POSTPROCESSING$NONE  #post processing flag redirects function calls to post processing object. Set to NONE on add/remove edge
  pstid <- 1
  act <- NULL   #list of actions and their parameters to recreate after adding/removing edges. It is delayed until requested.
  prc <- NULL   #pointer to last object in list of post processing objects. They are chained in series. Head is main algorithm
  
  # if(chc>=1 & chc<=10000){
  #   dc <- new(DynCommR,chc,qlt,prm)
  #   # print(dc)
  #   # TO DO: check for errors
  # }
  # else if(chc>=10001 & chc<=20000){
  #   # print("Python algorithm")
  #   dc <- reticulate::import_from_path("DynCommPython","../base/Python")
  #   # calls to python should be equal to calls to c++ but may need separate processing of outputs
  # }
  # else{
  #   print("Unknown algorithm :(")
  # }
  # print(dc)

  recreatePostProcessing = function(){
    assign("prc",NULL,thisEnv)
    for (cnt in act) {
      #get biggest id for current action
      i<-1
      # first <- FALSE
      if(is.null(prc)){#no actions yet
        # first <- TRUE
        assign("prc",alg,thisEnv)
      }
      else{#actions exist
        # cat("prc=",is(prc,"ObjectBase"),"\n")
        # print("prc=")
        # print(is(prc,"ObjectX"))
        # print("\n")
        if(!is(prc,"ObjectX")){
          # print("actions exist")
          # print(cnt)
          # print(i)
          q<-prc$exists(cnt,i)
          # print(q)
          while(q){
            i<-i+1
            # assign("i",i+1,thisEnv)
            # print(cnt)
            # print(i)
            q<-prc$exists(cnt,i)
            # print(q)
          }
        }
      }
      # print("new post process")
      # print(cnt)
      # print(i)
      assign("pst", cnt,thisEnv)
      assign("pstid", i,thisEnv)
      if(cnt==POSTPROCESSING$OBJECT1){
        assign("prc",Object1(prc,i),thisEnv)
        # print("object1")
      }
      else if(cnt==POSTPROCESSING$OBJECT2){
        assign("prc",Object2(prc,i),thisEnv)
        # print("object2")
      }
    }
  }
  
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
    postProcess = function(actions){
      assign("act", actions,thisEnv)
      recreatePostProcessing()
    },
    
    #' 
    #'   \item{select(postProcessing)}{Select between getting the results of the algorithm or one of the post processing steps. See \link{}}
    #'   
    select = function(postProcessing, id=1)
    {
      if(postProcessing==POSTPROCESSING$NONE){
        assign("pst",postProcessing,thisEnv)
      }
      else{
        if(is.null(prc)){
          #recreate post processing chain
          recreatePostProcessing()
          # return(TRUE)
        }
        if(prc$exists(postProcessing, id)){
          # pst <- postProcessing
          # pstid <- id
          assign("pst",postProcessing,thisEnv)
          assign("pstid",id,thisEnv)
          return(TRUE)
        }
        else{
          #there is no such post processing
          return(FALSE)
        }
      }
    },
    
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
      if(chc>=1 & chc<=10000){
        dc$addRemoveEdgesFile(graphAddRemoveFile)
      }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
          dc$quality()
        }
        else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
          dc$communityCount()
        }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
          dc$communities()
        }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
        dc$communityInnerEdgesWeight(community)
      }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
          dc$communityTotalWeight(community)
        }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
            dc$communityEdgeWeight(source,destination)
          }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
            dc$communityVertexCount(community)
          }
      else if(chc>=10001 & chc<=20000){
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
    communityNodeCount=function(community){
      communityVertexCount(community)
    },
    
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \link{community}}
    #'   
    community=function(vertex){
      if(pst==POSTPROCESSING$NONE){
        if(chc>=1 & chc<=10000){
            dc$community(vertex)
          }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
            dc$vertexCount()
          }
      else if(chc>=10001 & chc<=20000){
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
    nodesCount=function(){
      vertexCount()
    },
    
    #' 
    #'   \item{vertexAll()}{Get all vertices in the graph after the last iteration. See \link{vertexAll}}
    #'   
    vertexAll=function(){
      # if(pst==POSTPROCESSING$NONE){
        if(chc>=1 & chc<=10000){
            dc$vertexAll()
          }
      else if(chc>=10001 & chc<=20000){
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
    nodesAll=function(){
      vertexAll()
    },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \link{vertices}}
    #'   
    vertices=function(community){
      if(pst==POSTPROCESSING$NONE){
        if(chc>=1 & chc<=10000){
              dc$vertices(community)
            }
      else if(chc>=10001 & chc<=20000){
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
    nodes=function(community){
      vertices(community)
    },
        
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \link{communityMapping}}
    #'   
    communityMapping = function(){
      if(pst==POSTPROCESSING$NONE){
        if(chc>=1 & chc<=10000){
        dc$communityMapping(TRUE)
      }
      else if(chc>=10001 & chc<=20000){
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
        if(chc>=1 & chc<=10000){
            dc$time()
          }
      else if(chc>=10001 & chc<=20000){
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
  class(me) <- append(class(me),"DynComm")
  return(me)
}


# @name getAlgorithm
# 
# @aliases getAlgorithm
# 
# @title getAlgorithm()
#
# @description This method returns the algorithm with which this object was initialized.
# 
# @details to do
#
# @rdname getAlgorithm
# 
# @docType methods
# 
# @usage getAlgorithm()
# 
# @method DynComm getAlgorithm
# 
# @return ALGORITHM
#
# @examples
# dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
# dc$getAlgorithm()
# 
# NULL

#' @name addRemoveEdgesFile
#' 
#' @aliases edgesFile
#' 
#' @title addRemoveEdgesFile(graphAddRemoveFile)
#'
#' @description 
#' This method reads edges from a file and adds or removes them from the graph.
#' 
#' The file must have only one edge per line, with values separated by a white
#' space (both SPACE and TAB work in any amount and combination).
#' 
#' The first value is the source vertex, the second is the destination vertex, and 
#' the third is the weight.
#' 
#' The weight can be ommited if the edge is to be added using the default weight
#' of 1 (one), or if the parameter to ignore weights was set.
#' 
#' If the weight is exactly zero, the edge is removed from the graph.
#' 
#' If a vertex, mentioned in the source or destination, does not exist it will be 
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
#' @usage
#' addRemoveEdgesFile(graphAddRemoveFile)
#' addRemoveEdgesFile(dyncomm,graphAddRemoveFile)
#' edgesFile(graphAddRemoveFile)
#' edgesFile(dyncomm,graphAddRemoveFile)
#' 
#' @param graphAddRemoveFile The filename that contains the edges to add/remove
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @method DynComm addRemoveEdgesFile
#' 
#' @return FALSE if any kind of error occurred. Otherwise, TRUE
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$addRemoveEdgesFile("graphAddRemoveFile.txt")
#' }
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' addRemoveEdgesFile(dc,"graphAddRemoveFile.txt")
#' }
#' 
# NULL
addRemoveEdgesFile <- function(dyncomm,graphAddRemoveFile){
  dyncomm$addRemoveEdgesFile(graphAddRemoveFile)
}
edgesFile <- function(dyncomm,graphAddRemoveFile){
  dyncomm$addRemoveEdgesFile(graphAddRemoveFile)
}

#' @name quality
#'
# @aliases quality
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
#' @usage 
#' quality()
#' quality(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @method DynComm quality
#'
#' @return a floating point number
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$quality()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' quality(dc)
#'}
#'
# NULL
quality <- function(dyncomm){
  dyncomm$quality()
}

#' @name communityCount
#'
# @aliases communityCount
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
#' @usage 
#' communityCount()
#' communityCount(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @method DynComm communityCount
#'
#' @return an unsigned integer value with the number of communities
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityCount()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityCount(dc)
#'}
#'
# NULL
communityCount <- function(dyncomm){
  dyncomm$communityCount()
}

#' @name communities
#'
# @aliases communities
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
#' @usage 
#' communities()
#' communities(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @method DynComm communities
#'
#' @return a list of all communities
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communities()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communities(dc)
#'}
#'
# NULL
communities <- function(dyncomm){
  dyncomm$communities()
}

#' @name communityInnerEdgesWeight
#'
# @aliases communityInnerEdgesWeight
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
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' communityInnerEdgesWeight(community)
#' communityInnerEdgesWeight(dyncomm,community)
#'
#' @method DynComm communityInnerEdgesWeight
#'
#' @return a floating point number with the weight
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityInnerEdgesWeight(1)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityInnerEdgesWeight(dc,1)
#'}
#'
# NULL
communityInnerEdgesWeight <- function(dyncomm,community){
  dyncomm$communityInnerEdgesWeight(community)
}

#' @name communityTotalWeight
#'
# @aliases communityTotalWeight
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
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' communityTotalWeight(community)
#' communityTotalWeight(dyncomm,community)
#'
#' @method DynComm communityTotalWeight
#'
#' @return a floating point number with the weight
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityTotalWeight(1)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityTotalWeight(dc,1)
#'}
#'
# NULL
communityTotalWeight <- function(dyncomm,community){
  dyncomm$communityTotalWeight(community)
}

#' @name communityEdgeWeight
#'
# @aliases communityEdgeWeight
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
#' @param source The name of the source vertex that is part of the edge
#' 
#' @param destination The name of the destination vertex that is part of the edge
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' communityEdgeWeight(source,destination)
#' communityEdgeWeight(dyncomm,source,destination)
#'
#' @method DynComm communityEdgeWeight
#'
#' @return a floating point number with the weight
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityEdgeWeight(12,42)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityEdgeWeight(dc,12,42)
#'}
#'
# NULL
communityEdgeWeight <- function(dyncomm,source,destination){
  dyncomm$communityEdgeWeight(source,destination)
}

#' @name communityVertexCount
#'
#' @aliases communityNodeCount
#'
#' @title communityVertexCount(community)
#'
#' @description 
#' Get the amount of vertices in the given community after the last iteration.
#'
# @details to do
#'
#' @rdname communityVertexCount
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' communityVertexCount(community)
#' communityVertexCount(dyncomm,community)
#' communityNodeCount(community)
#' communityNodeCount(dyncomm,community)
#'
#' @method DynComm communityVertexCount
#'
#' @return an unsigned integer with the number of vertices in the given community
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityVertexCount(3)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityVertexCount(dc,3)
#'}
#'
# NULL
communityVertexCount <- function(dyncomm,community){
  dyncomm$communityVertexCount()
}
communityNodeCount <- function(dyncomm,community){
  dyncomm$communityVertexCount()
}

#' @name community
#'
# @aliases community
#'
#' @title community(vertex)
#'
#' @description 
#' Get the community of the given vertex after the last iteration.
#'
# @details to do
#'
#' @rdname community
#'
#' @docType methods
#'
#' @param vertex The name of the intended vertex
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' community(vertex)
#' community(dyncomm,vertex)
#'
#' @method DynComm community
#'
#' @return an unsigned integer with the community of the given vertex
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$community(8)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' community(dc,8)
#'}
#'
# NULL
community <- function(dyncomm,vertex){
  dyncomm$community(vertex)
}

#' @name vertexCount
#'
#' @aliases nodesCount
#'
#' @title vertexCount()
#'
#' @description 
#' Get the total number of vertices after the last iteration. It can be useful since
#' vertices can be added, if an edge being added has vertices that do not exist in the 
#' graph, or removed, if they are not part of any edge after removing an edge.
#'
# @details to do
#'
#' @rdname vertexCount
#'
#' @docType methods
#'
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' vertexCount()
#' vertexCount(dyncomm)
#' nodesCount()
#' nodesCount(dyncomm)
#'
#' @method DynComm vertexCount
#'
#' @return an unsigned integer with the number of vertices in the graph
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$vertexCount()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' vertexCount(dc)
#'}
#'
# NULL
vertexCount <- function(dyncomm){
  dyncomm$vertexCount()
}
nodesCount <- function(dyncomm){
  dyncomm$vertexCount()
}

#' @name vertexAll
#'
#' @aliases nodesAll
#'
#' @title vertexAll()
#'
#' @description 
#' Get all vertices in the graph after the last iteration.
#'
# @details to do
#'
#' @rdname vertexAll
#'
#' @docType methods
#'
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' vertexAll()
#' vertexAll(dyncomm)
#' nodesAll()
#' nodesAll(dyncomm)
#'
#' @method DynComm vertexAll
#'
#' @return a list of all vertices in the graph
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$vertexAll()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' vertexAll(dc)
#'}
#'
# NULL
vertexAll <- function(dyncomm){
  dyncomm$vertexAll()
}
nodesAll <- function(dyncomm){
  dyncomm$vertexAll()
}

#' @name vertices
#'
#' @aliases nodes
#'
#' @title vertices(community)
#'
#' @description 
#' Get all vertices belonging to the given community after the last iteration.
#'
# @details to do
#'
#' @rdname vertices
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' vertices(community)
#' vertices(dyncomm,community)
#' nodes(community)
#' nodes(dyncomm,community)
#'
#' @method DynComm vertices
#'
#' @return a list of vertices belonging to the given community
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$vertices(6)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' vertices(dc,6)
#'}
#'
# NULL
vertices <- function(dyncomm,community){
  dyncomm$vertices(community)
}
nodes <- function(dyncomm,community){
  dyncomm$vertices(community)
}

#' @name communityMapping
#'
# @aliases communityMapping
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
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage 
#' communityMapping()
#' communityMapping(dyncomm)
#'
#' @method DynComm communityMapping
#'
#' @return a two column matrix with communities in the first column and the vertices in the second
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityMapping()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityMapping(dc)
#'}
#'
# NULL
communityMapping <- function(dyncomm){
  dyncomm$communityMapping()
}

#' @name time
#'
# @aliases time
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
#' @param dyncomm A DynComm object, if not using the inline version of the function call
#' 
#' @usage
#' time()
#' time(dyncomm)
#' 
#'
#' @method DynComm time
#'
#' @return an unsigned integer with the total processing time
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$time()
#' ## 2.3
#' }
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' time(dc)
#' ## 2.3
#' }
#' 
# NULL
time <- function(dyncomm){
  dyncomm$time()
}

