########################### Developer Notice ###########################
# Description:
# This file holds the DynComm post processing algorithms.
#
# Internally, this object, dispatches calls to objects that do the actual work.
#
# New post processing algorithms should have their name added to the list 
# POSTPROCESSING and its calls in the if clause of the algorithms function.
#
# Author: poltergeist0
# Date: 2019-01-01

########################### Include R sources here ###########################
source ("R/DensityOPT2.R")

########################### API Documentation ###########################

#' @name POSTPROCESSING
#'
#' @aliases POST post postProcessing postprocessing
#' 
#' @title List of available post processing algorithms.
#' 
#' @author poltergeist0
#' 
#' @description 
#' A post processing algorithm is a function that modifies the results 
#' presented to the user, allowing for limited result manipulation, but does not 
#' internally modify the results obtained by the algorithm.
#' 
#' @details 
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
#'             @references \insertRef{Sarmento2019Apr}{DynComm}
#'          }
#'  }
#'  
#' @examples
#' POSTPROCESSING$DENSOPT
#' 
#' @export
#'
########## list new algorithms here #############
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

#' @name APIFUNCTIONS
#'
#' @keywords internal
#' 
# @aliases 
#' 
#' @title List of API functions.
#' 
#' @author poltergeist0
#' 
#' @description 
#' This is a list of all functions of the API.
#' 
#' @details 
#' Post processing algorithms must register, which functions of the API they 
#' implement, in a has() function. If the post processing algorithm implements 
#' a certain API function, the has() function must return TRUE when questioned 
#' about that API function. Otherwise, it must return FALSE.
#'
#' @usage APIFUNCTIONS
#' 
#' @format A named list with the names of all API functions:
#'  \describe{
#'    \item{COMMUNITIES}{Get all communities after the last iteration. See \code{\link{communities}}}
#'    \item{COMMUNITY}{Get the community of the given vertex after the last iteration. See \code{\link{community}}}
#'    \item{COMMUNITYCOUNT}{Get the number of communities after the last iteration. See \code{\link{communityCount}}}
#'    \item{COMMUNITYEDGEWEIGHT}{Get the weight of the edge that goes from source to destination after the last iteration. See \code{\link{communityEdgeWeight}}}
#'    \item{COMMUNITYINNEREDGESWEIGHT}{Get the sum of weights of the inner edges of the given community after the last iteration. See \code{\link{communityInnerEdgesWeight}}}
#'    \item{COMMUNITYMAPPINGMATRIX}{Get a matrix with the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
#'    \item{COMMUNITYMAPPINGFILE}{Write to a file the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
#'    \item{COMMUNITYNEIGHBOURS}{Get the neighbours of the given community after the last iteration. See \code{\link{communityNeighbours}}}
#'    \item{COMMUNITYTOTALWEIGHT}{Get the sum of weights of all edges of the given community after the last iteration. See \code{\link{communityTotalWeight}}}
#'    \item{COMMUNITYVERTEXCOUNT}{Get the amount of vertices in the given community after the last iteration. See \code{\link{communityVertexCount}}}
#'    \item{EDGEWEIGHT}{Get the weight of the edge that goes from source vertex to destination vertex after the last iteration. See \code{\link{edgeWeight}}}
#'    \item{NEIGHBOURS}{Get the neighbours of the given vertex after the last iteration. See \code{\link{neighbours}}}
#'    \item{QUALITY}{Get the quality measurement of the graph after the last iteration. See \code{\link{quality}}}
#'    \item{RESULTS}{Get additional results of the algorithm or the currently selected post processing steps. See \code{\link{results}}}
#'    \item{TIME}{Get the cumulative time spent on processing after the last iteration. See \code{\link{time}}}
#'    \item{VERTEXCOUNT}{Get the total number of vertices after the last iteration. See \code{\link{vertexCount}}}
#'    \item{VERTICESALL}{Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}}
#'    \item{VERTICES}{Get all vertices belonging to the given community after the last iteration. See \code{\link{vertices}}}
#'  }
#'  
# @examples
# APIFUNCTIONS$
#' 
# @export
#'
APIFUNCTIONS <- list(
  COMMUNITIES=1L
  ,COMMUNITY=2L
  ,COMMUNITYCOUNT=3L
  ,COMMUNITYEDGEWEIGHT=4L
  ,COMMUNITYINNEREDGESWEIGHT=5L
  ,COMMUNITYMAPPINGFILE=6L
  ,COMMUNITYMAPPINGMATRIX=7L
  ,COMMUNITYNEIGHBOURS=8L
  ,COMMUNITYTOTALWEIGHT=9L
  ,COMMUNITYVERTEXCOUNT=10L
  ,EDGEWEIGHT=11L
  ,NEIGHBOURS=12L
  ,QUALITY=13L
  ,RESULTS=14L
  ,TIME=15L
  ,VERTEXCOUNT=16L
  ,VERTICESALL=17L
  ,VERTICES=18L
)

#' @name DynCommPostProcess
#' 
#' @keywords internal
#' 
#' @aliases dyncommpostprocess
#' 
#' @title DynCommPostProcess(postProcessing, id, previous, Parameters)
#'
#' @author poltergeist0
#' 
#' @description 
#' Provides a single interface for all post processing algorithms in the 
#' different languages.
#' 
#' @details 
#' Includes methods to get results of processing and to interact with the 
#' vertices, edges and communities.
#'
#' @rdname DynCommPostProcess
#' 
#' @docType class
#' 
#' @usage 
#' DynCommPostProcess(postProcessing, id)
#' DynCommPostProcess(postProcessing, id, previous,Parameters)
#'
#' @param Algorithm One of the available ALGORITHM See \code{\link{ALGORITHM}}
#' 
#' @param Criterion One of the available CRITERION. See \code{\link{CRITERION}}
#' 
#' @param Parameters A two column matrix defining additional parameters. See
#'   the PARAMETERS section on this page
#'
#' @return \code{DynCommPostProcess} object
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
#' }
#' 
#' @section Methods:
#' \describe{
#' 
# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
DynCommPostProcess <- function(postProcessing=POSTPROCESSING$NONE, id=1, previous, Parameters=NULL)
{
  
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  prm <- Parameters  #main algorithm parameters
  pst <- POSTPROCESSING$NONE  #post processing flag redirects function calls to post processing object. Set to NONE on add/remove edge
  pstid <- id
  prv <- previous

  ########## add new algorithms here #############
  algorithms = function(){
    # print(pst)
    # print(postProcessing)
    if(postProcessing==POSTPROCESSING$DENSOPT){
      assign("pst",POSTPROCESSING$DENSOPT,thisEnv)
      return(DensOpt(prv,prm))
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
    
    #' 
    #'   \item{exists(postProcessing,ID)}{Check if a post processing object exists with given name and ID. See \code{\link{exists}}}
    #'   
    exists = function(postProcessing=POSTPROCESSING$NONE,ID=1){
      if(pst==postProcessing && id==ID){
        #this object
        return(TRUE)
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          return(FALSE)  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommMain")){
            #do not call exists on main algorithm because function does not exist
            return(FALSE)
          }
          else{
            return(prv$exists(postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{results(differential)}{Get additional results of the algorithm or the currently selected post processing steps. See \code{\link{results}}}
    #'   
    results = function(differential=TRUE,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$RESULTS)){
        #this object
        return(alg$results(differential))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(matrix(data=c(NA,NA),ncol=2,byrow = TRUE,dimnames = c("name","value")))
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$results(differential))
          }
          else{ #is another post processing algorithm
            return(prv$results(differential,postProcessing,ID))
          }
        }
      }
    },

    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \code{\link{quality}}}
    #'   
    quality=function(postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$QUALITY)){
        #this object
        return(alg$quality())
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$quality())
          }
          else{ #is another post processing algorithm
            return(prv$quality(postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \code{\link{communityCount}}}
    #'   
    communityCount=function(postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYCOUNT)){
        #this object
        return(alg$communityCount())
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityCount())
          }
          else{ #is another post processing algorithm
            return(prv$communityCount(postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \code{\link{communities}}}
    #'   
    communities=function(postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITIES)){
        #this object
        return(alg$communities())
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communities())
          }
          else{ #is another post processing algorithm
            return(prv$communities(postProcessing,ID))
          }
        }
      }
    },
    
    
    #' 
    #'   \item{communityNeighbours(community)}{Get the neighbours of the given community after the last iteration. See \code{\link{communityNeighbours}}}
    #'   
    communityNeighbours=function(community,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYNEIGHBOURS)){
        #this object
        return(alg$communityNeighbours(community))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(matrix(data=c(NA,NA),ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityNeighbours(community))
          }
          else{ #is another post processing algorithm
            return(prv$communityNeighbours(community,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \code{\link{communityInnerEdgesWeight}}}
    #'   
    communityInnerEdgesWeight=function(community,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYINNEREDGESWEIGHT)){
        #this object
        return(alg$communityInnerEdgesWeight(community))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityInnerEdgesWeight(community))
          }
          else{ #is another post processing algorithm
            return(prv$communityInnerEdgesWeight(community,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \code{\link{communityTotalWeight}}}
    #'   
    communityTotalWeight=function(community,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYTOTALWEIGHT)){
        #this object
        return(alg$communityTotalWeight(community))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityTotalWeight(community))
          }
          else{ #is another post processing algorithm
            return(prv$communityTotalWeight(community,postProcessing,ID))
          }
        }
      }
    },
      
        
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \code{\link{communityEdgeWeight}}}
    #'   
    communityEdgeWeight=function(source,destination,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYEDGEWEIGHT)){
        #this object
        return(alg$communityEdgeWeight(source,destination))
      }
      else{ #it is not me (its the one armed man :P ) <- copy/paste rules
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityEdgeWeight(source,destination))
          }
          else{ #is another post processing algorithm
            return(prv$communityEdgeWeight(source,destination,postProcessing,ID))
          }
        }
      }
    },
        
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \code{\link{communityVertexCount}}}
    #'   
    communityVertexCount=function(community,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYVERTEXCOUNT)){
        #this object
        return(alg$communityVertexCount(community))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityVertexCount(community))
          }
          else{ #is another post processing algorithm
            return(prv$communityVertexCount(community,postProcessing,ID))
          }
        }
      }
    },
        
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \code{\link{community}}}
    #'   
    community=function(vertex,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITY)){
        #this object
        return(alg$community(vertex))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$community(vertex))
          }
          else{ #is another post processing algorithm
            return(prv$community(vertex,postProcessing,ID))
          }
        }
      }
    },
        
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \code{\link{vertexCount}}}
    #'   
    vertexCount=function(postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$VERTEXCOUNT)){
        #this object
        return(alg$vertexCount())
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$vertexCount())
          }
          else{ #is another post processing algorithm
            return(prv$vertexCount(postProcessing,ID))
          }
        }
      }
    },

    #' 
    #'   \item{verticesAll()}{Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}}
    #'   
    verticesAll=function(postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$VERTICESALL)){
        #this object
        return(alg$verticesAll())
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(list())
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$verticesAll())
          }
          else{ #is another post processing algorithm
            return(prv$verticesAll(postProcessing,ID))
          }
        }
      }
    },
        
    #' 
    #'   \item{neighbours(vertex)}{Get the neighbours of the given vertex after the last iteration. See \code{\link{neighbours}}}
    #'   
    neighbours=function(vertex,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$NEIGHBOURS)){
        #this object
        return(alg$neighbours(community))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(matrix(data=c(NA,NA),ncol=2,byrow=TRUE,dimnames = list(c(),c("neighbour","weight"))))
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$neighbours(community))
          }
          else{ #is another post processing algorithm
            return(prv$neighbours(community,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{edgeWeight(source,destination)}{Get the weight of the edge that goes from source vertex to destination vertex after the last iteration. See \code{\link{edgeWeight}}}
    #'   
    edgeWeight=function(source,destination,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$EDGEWEIGHT)){
        #this object
        return(alg$edgeWeight(source,destination))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$edgeWeight(source,destination))
          }
          else{ #is another post processing algorithm
            return(prv$edgeWeight(source,destination,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \code{\link{vertices}}}
    #'   
    vertices=function(community,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$VERTICES)){
        #this object
        return(alg$vertices(community))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(list())
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$vertices(community))
          }
          else{ #is another post processing algorithm
            return(prv$vertices(community,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingFile = function(differential=TRUE,file="",postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYMAPPINGFILE)){
        #this object
        return(alg$communityMappingFile(differential,file))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(matrix(data=c(NA,NA),ncol=2,byrow = TRUE,dimnames = c("name","value")))
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityMappingFile(differential,file))
          }
          else{ #is another post processing algorithm
            return(prv$communityMappingFile(differential,file,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \code{\link{communityMapping}}}
    #'   
    communityMappingMatrix = function(differential=TRUE,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$COMMUNITYMAPPINGMATRIX)){
        #this object
        return(alg$communityMappingMatrix(differential))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(matrix(data=c(NA,NA),ncol=2,byrow = TRUE,dimnames = c("name","value")))
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$communityMappingMatrix(differential))
          }
          else{ #is another post processing algorithm
            return(prv$communityMappingMatrix(differential,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{time()}{Get the cumulative time spent on processing after the last iteration. See \code{\link{time}}}
    #'   
    time=function(differential=FALSE,postProcessing=POSTPROCESSING$NONE,ID=1){
      if((postProcessing==POSTPROCESSING$NONE || (pst==postProcessing && id==ID)) && alg$has(APIFUNCTIONS$TIME)){
        #this object
        return(alg$time(differential))
      }
      else{ #it is not me (its the one armed man :P )
        #return from the previous object
        if(is.null(prv)){
          #should never get here. There is always a previous
          return(NA)
        }
        else{# there is a previous
          if(is(prv,"DynCommMain")){ #is main algorithm
            #do not pass type and id
            return(prv$time(differential))
          }
          else{ #is another post processing algorithm
            return(prv$time(differential,postProcessing,ID))
          }
        }
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
  class(me) <- append(class(me),"DynCommPostProcess")
  if(is.null(alg)) return(NULL)
  return(me)
}
