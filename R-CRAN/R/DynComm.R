########################### Developer Notice ###########################
# Description:
# This file holds the DynComm user interface and its documentation.
#
# Internally, it dispatches calls to objects that implement the API and do the 
# actual work.
#
# There should never be any reason to change it unless the API or the user 
# interface changes.
#
# Main algorithms are handled by the DynCommMain object. Post processing
# algorithms are handled by the DynCommPostProcess object. Changes should be in
# those objects. They can be found in the files with the same name as the object.
#
# Author: poltergeist0
# Date: 2019-01-01


########################### Package Documentation ###########################
#' @name DynComm-package
#' 
#' @aliases dyncomm-package Dyncomm-package
#' 
#' @title DynComm: Dynamic Network Communities Detection
#' 
#' @author poltergeist0
#' 
#' @description 
#' Bundle of algorithms used for evolving network analysis regarding 
#' community detection.
#' 
#' @details 
#' Implements several algorithms, using a common API, that calculate 
#' communities for graphs whose vertices and edges change over time.
#' Edges, which can have new vertices, can be added or deleted, and changes in 
#' the communities are calculated without recalculating communities for the 
#' entire graph.
#' 
#' @docType package
#' @import Rcpp methods
#' @importFrom Rdpack reprompt
#' @useDynLib DynComm
#' 
#' @section Referenced Work:
#' This package uses the following work as reference material for the 
#' implementation of the algorithms.
#' @references
#' \insertRef{cordeiro2016dynamic}{DynComm}
#' \insertRef{Rossetti:2017:TOA:3127967.3128003}{DynComm}
#' \insertRef{RG17}{DynComm}
#' \insertRef{Sarmento2019Apr}{DynComm}
#' 
#' @seealso \code{\link{DynComm}}
#' 
#' 
NULL

source('R/DynCommMain.R')
source('R/DynCommPostProcess.R')

########################### API Documentation ###########################
#' @name DynComm
#' 
#' @aliases Dyncomm dyncomm
#' 
#' @title DynComm
#'
#' @author poltergeist0
#' 
#' @description 
#' Provides a single interface for all algorithms in the different 
#' languages.
#' 
#' @details 
#' Includes methods to get results of processing and to interact with the 
#' vertices, edges and communities.
#' Provided methods to return information on the graph are divided into two 
#' layers. A lower level layer that interacts with vertices and how they 
#' connect. And a higher level layer that interacts with communities and how 
#' they connect.
#' Besides the main algorithm, also accepts post processing algorithms that are 
#' used mainly to filter the results. Post processing algorithms can use 
#' additional computational resources so check the Performance section of the
#' help page of each algorithm you intend to use.
#'
#' @rdname DynComm
#' 
#' @docType class
#' 
#' @usage DynComm(Algorithm,Criterion,Parameters)
#' 
#' @param Algorithm One of the available ALGORITHM. Default ALGORITHM$LOUVAIN. 
#'   See \code{\link{ALGORITHM}}
#' 
#' @param Criterion One of the available CRITERION. Default CRITERION$MODULARITY.
#'   See \code{\link{CRITERION}}
#' 
#' @param Parameters A two column matrix defining additional parameters. Default NULL.
#'  See the PARAMETERS section on this page
#'
#' @return \code{DynComm} object
#'
#' @seealso 
#' \code{\link{DynComm-package}}
#'
#' @export
#'
#' @examples
#' Parameters<-matrix(c("-e","0.1","-w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#' matrix(
#' c(10,20,10,30,20,30,30,60,40,60,40,50,50,70,60,70)
#' ,ncol=2,byrow=TRUE)
#' )
#' ## or
#' ## dc$addRemoveEdgesFile("initial_graph.txt")
#' dc$communityCount()
#' ## you can use the non inline version of the functions
#' communities(dc)
#' dc$communityNodeCount(1)
#' dc$vertices(1)
#' dc$communityMapping(TRUE)
#' dc$quality()
#' dc$time()
#' ## lets add post processing :)
#' dc$postProcess(
#' list(
#' list(POSTPROCESSING$DENSOPT)
#' )
#' )
#' ## the results of the last step of post processing are selected automatically
#' ## densopt post processing algorithm may change the community mapping so...
#' ## check it
#' dc$communityMapping(TRUE)
#' ## densopt post processing algorithm may change quality so check it
#' dc$quality()
#' ## time is now the total time of the main algorithm plus the time of every...
#' ## post processing algorithm up to the one selected
#' dc$time()
#' ## get back to main algorithm results to check they haven't changed
#' dc$select(POSTPROCESSING$NONE)
#' dc$communityMapping(TRUE)
#' dc$quality()
#' dc$time()
#' ## add and remove edges. Notice that there is one more column to give...
#' ## weights of zero on the edges to remove. In this case, all other weights...
#' ## are ignored because the graph is set to ignore weights (parameter w is...
#' ## false).
#' dc$addRemoveEdges(
#' matrix(
#' c(30,60,0,40,60,0.23,10,80,2342,80,90,3.1415)
#' ,ncol=3,byrow=TRUE)
#' )
#' ## since the post processing was not reset, it will be automatically...
#' ## calculated and results switched to the last step. In this case, to the...
#' ## densopt algorithm
#' dc$communityMapping(TRUE)
#' dc$quality()
#' dc$time()
#' ## get back to main algorithm results to check them
#' dc$select(POSTPROCESSING$NONE)
#' dc$communityMapping(TRUE)
#' dc$quality()
#' dc$time()
#' ## lets reset/remove post processing
#' dc$postProcess()
#' 
#'
#' @section PARAMETERS:
#' A two column matrix defining additional parameters to be passed to the
#' selected ALGORITHM and CRITERION.
#' The first column names the parameter and the second defines its value.
#' \describe{
#'   \item{
#'   -c
#'   }{
#'   Owsinski-Zadrozny quality function parameter. 
#'   Values [0.0:1.0]. Default: 0.5
#'   }
#'   \item{
#'   -k
#'   }{
#'   Shi-Malik quality function kappa_min value. 
#'   Value > 0 . Default 1
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
#'   Values TRUE,FALSE. Default FALSE
#'   }
#'   \item{
#'   -e
#'   }{
#'   Stops when, on a cycle of the algorithm, the quality is increased by less 
#'   than the value given in this parameter.
#'   Value > 0 . Default 0.01
#'   }
#' }
#' 
#' @section Methods:
#' \describe{

# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
DynComm <- function(Algorithm=ALGORITHM$LOUVAIN,Criterion=CRITERION$MODULARITY,Parameters=NULL)
{
  
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  ########## constructor #############
  alg <- DynCommMain(Algorithm,Criterion,Parameters)  #main algorithm
  pst <- POSTPROCESSING$NONE  #post processing flag redirects function calls to post processing object. Set to NONE on add/remove edge
  pstid <- 1
  act <- NULL   #list of actions and their parameters to recreate after adding/removing edges. It is delayed until requested.
  prc <- NULL   #pointer to last object in list of post processing objects. They are chained in series. Head is main algorithm
  
  ## internal function that recreates the post processing chain
  recreatePostProcessing = function(){
    # invalidate previous post processing. Results are outdated
    assign("pst",POSTPROCESSING$NONE,thisEnv)
    assign("prc",NULL,thisEnv)
    b<-FALSE
    # validate no actions
    if(is.null(act) || length(act)==0){
      #setting to NULL always succeeds
      assign("act", NULL,thisEnv)
      return(TRUE)
    }
    else{#act not NULL and has values
      # validate actions list does not contain POSTPROCESSING$NONE
      for (cnt in act) {
        if((!is.null(cnt)) && is.list(cnt) && length(cnt)>0){
            if(cnt[1]!=POSTPROCESSING$NONE){
              b<-TRUE
            }
            else{
              return(FALSE)
            }
        }
        else{
          return(FALSE)
        }
      }
    }
    if(b){
      for (cnt in act) {
        i<-1
        if(is.null(prc)){#no actions yet
          # assign main algorithm to queue
          assign("prc",alg,thisEnv)
        }
        else{#actions exist
          # if there is more than the main algorithm, get biggest id for the current action
          if(!is(prc,"DynCommBase")){
            q<-prc$exists(cnt,i)
            # increment id while a post processing object of the given type exists
            while(q){
              i<-i+1
              q<-prc$exists(cnt,i)
            }
          }
        }
        # select the latest post processing algorithm and id as default for posterior user operations
        assign("pst", cnt,thisEnv)
        assign("pstid", i,thisEnv)
        # create post processing object and assign it to the end of the queue
        tmp <- DynCommPostProcess(pst,pstid,prc,NULL) #TODO pass parameters to post processing
        if(is.null(tmp)){
          #TODO improve error message
          print("Invalid post processing")
          print(pst)
          assign("pst",POSTPROCESSING$NONE,thisEnv)
          assign("prc",NULL,thisEnv)
          return(FALSE)
        }
        assign("prc",tmp,thisEnv)
      }
      return(TRUE)
    }
    return(FALSE)
  }
  
  ## Create the list used to represent an
  ## object for this class
  me <- list(
    
    ## Define the environment where this list is defined so
    ## that I can refer to it later.
    thisEnv = thisEnv,
    
    #' 
    #'   \item{postProcess(actions)}{
    #'   Set a list of post processing steps. See \code{\link{postProcess}}
    #'   }
    #'   
    postProcess = function(actions=NULL){
      if((!is.null(actions)) && (!is.list(actions))){
        # if not NULL and not a list assign default
        assign("pst",POSTPROCESSING$NONE,thisEnv)
        assign("prc",NULL,thisEnv)
        assign("act", NULL,thisEnv)
        return(FALSE)
      }
      else{#is list or NULL
        assign("act", actions,thisEnv)
        return(recreatePostProcessing())
      }
    },
    
    #' 
    #'   \item{select(postProcessing,id)}{
    #'   Select between getting the results of the algorithm or one of the post 
    #'   processing steps. See \code{\link{select}}
    #'   }
    #'   
    select = function(postProcessing=POSTPROCESSING$NONE, id=1)
    {
      if(postProcessing==POSTPROCESSING$NONE){
        assign("pst",postProcessing,thisEnv)
        return(TRUE)
      }
      else{
        if(is.null(prc)){
          #recreate post processing chain
          b<-recreatePostProcessing()
          if(!b){#failed to recreate post processing}
            return(FALSE)
          }
        }
        if(prc$exists(postProcessing, id)){
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
    #'   \item{results(differential)}{
    #'   Get additional results of the algorithm or the currently selected post 
    #'   processing steps. See \code{\link{results}}
    #'   }
    #'   
    results = function(differential=TRUE){
      if(pst==POSTPROCESSING$NONE){#get from algorithm
        return(alg$results(differential))
      }
      else{#get from post processing
        return(prc$results(differential,pst,pstid))
      }
    },

    #' 
    #'   \item{addRemoveEdges(graphAddRemove)}{
    #'   Add and remove edges read from a matrix or file. See \code{\link{addRemoveEdges}}
    #'   }
    #'   
    addRemoveEdges = function(graphAddRemove=""){
      # invalidate previous post processing queue. Results will be outdated
      assign("pst",POSTPROCESSING$NONE,thisEnv)
      assign("prc",NULL,thisEnv)
      # update graph
      b<-FALSE
      if(is.matrix(graphAddRemove) && ncol(graphAddRemove)>1 && ncol(graphAddRemove)<4){#test for matrix
        b<-alg$addRemoveEdgesMatrix(graphAddRemove)
      }
      else if(is.character(graphAddRemove) && length(graphAddRemove)==1 && nchar(graphAddRemove)>0){#file was given
        b<-alg$addRemoveEdgesFile(graphAddRemove)
      }
      else{#neither file nor matrix
        print("Invalid input")
        return(FALSE)
      }
      if(b){
        #attempt to recreate post processing
        b<-recreatePostProcessing()
      }
      return(b)
    },

    #' 
    #'   \item{addRemove(graphAddRemove)}{Alias for addRemoveEdges(). See \code{\link{addRemoveEdges}}}
    #'   
    addRemove = function(graphAddRemove=""){
      return(addRemoveEdges(graphAddRemove))
    },
    
    #' 
    #'   \item{add(graphAddRemove)}{Alias for addRemoveEdges(). See \code{\link{addRemoveEdges}}}
    #'   
    add = function(graphAddRemove=""){
      return(addRemoveEdges(graphAddRemove))
    },
    
    #' 
    #'   \item{quality()}{
    #'   Get the quality measurement of the graph after the last iteration. 
    #'   See \code{\link{quality}}
    #'   }
    #'   
    quality=function(){
      if(pst==POSTPROCESSING$NONE){
        return(alg$quality())
      }
      else{
        return(prc$quality(pst,pstid))
      }
    },
    
    #' 
    #'   \item{communityCount()}{
    #'   Get the number of communities after the last iteration. 
    #'   See \code{\link{communityCount}}
    #'   }
    #'   
    communityCount=function(){
      if(pst==POSTPROCESSING$NONE){
        return(alg$communityCount())
      }
      else{
        return(prc$communityCount(pst,pstid))
      }
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \code{\link{communities}}}
    #'   
    communities=function(){
      if(pst==POSTPROCESSING$NONE){
        return(alg$communities())
      }
      else{
        return(prc$communities(pst,pstid))
      }
    },
    
    #' 
    #'   \item{communityNeighbours(community)}{
    #'   Get the neighbours of the given community after the last iteration. 
    #'   See \code{\link{communityNeighbours}}
    #'   }
    #'   
    communityNeighbours=function(community=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$communityNeighbours(community))
      }
      else{
        return(prc$communityNeighbours(community,pst,pstid))
      }
    },
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{
    #'   Get the sum of weights of the inner edges of the given community after 
    #'   the last iteration. See \code{\link{communityInnerEdgesWeight}}
    #'   }
    #'   
    communityInnerEdgesWeight=function(community=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$communityInnerEdgesWeight(community))
      }
      else{
        return(prc$communityInnerEdgesWeight(community,pst,pstid))
      }
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{
    #'   Get the sum of weights of all edges of the given community after the 
    #'   last iteration. See \code{\link{communityTotalWeight}}
    #'   }
    #'   
    communityTotalWeight=function(community=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$communityTotalWeight(community))
      }
      else{
        return(prc$communityTotalWeight(community,pst,pstid))
      }
    },
      
        
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{
    #'   Get the weight of the edge that goes from source community to destination 
    #'   community after the last iteration. See \code{\link{communityEdgeWeight}}
    #'   }
    #'   
    communityEdgeWeight=function(source=1,destination=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$communityEdgeWeight(source,destination))
      }
      else{
        return(prc$communityEdgeWeight(source,destination,pst,pstid))
      }
    },
        
    #' 
    #'   \item{communityVertexCount(community)}{
    #'   Get the amount of vertices in the given community after the last 
    #'   iteration. See \code{\link{communityVertexCount}}
    #'   }
    #'   
    communityVertexCount=function(community=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$communityVertexCount(community))
      }
      else{
        return(prc$communityVertexCount(community,pst,pstid))
      }
    },
        
    #' 
    #'   \item{communityNodeCount(community)}{Alias for communityVertexCount(). See \code{\link{communityVertexCount}}}
    #'   
    communityNodeCount=function(community=1){
      return(communityVertexCount(community))
    },
    
    #' 
    #'   \item{community(vertex)}{
    #'   Get the community of the given vertex after the last iteration. 
    #'   See \code{\link{community}}
    #'   }
    #'   
    community=function(vertex=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$community(vertex))
      }
      else{
        return(prc$community(vertex,pst,pstid))
      }
    },
        
    #' 
    #'   \item{vertexCount()}{
    #'   Get the total number of vertices after the last iteration. See \code{\link{vertexCount}}
    #'   }
    #'   
    vertexCount=function(){
      if(pst==POSTPROCESSING$NONE){
        return(alg$vertexCount())
      }
      else{
        return(prc$vertexCount(pst,pstid))
      }
    },

    #' 
    #'   \item{nodesCount()}{Alias for vertexCount(). See \code{\link{vertexCount}}}
    #'   
    nodesCount=function(){
      return(vertexCount())
    },
    
    #' 
    #'   \item{verticesAll()}{
    #'   Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}
    #'   }
    #'   
    verticesAll=function(){
      if(pst==POSTPROCESSING$NONE){
        return(alg$verticesAll())
      }
      else{
        return(prc$verticesAll(pst,pstid))
      }
    },
        
    #' 
    #'   \item{nodesAll()}{Alias for verticesAll(). See \code{\link{verticesAll}}}
    #'   
    nodesAll=function(){
      return(verticesAll())
    },
    
    #' 
    #'   \item{neighbours(vertex)}{
    #'   Get the neighbours of the given vertex after the last iteration. See \code{\link{neighbours}}
    #'   }
    #'   
    neighbours=function(vertex=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$neighbours(vertex))
      }
      else{
        return(prc$neighbours(vertex,pst,pstid))
      }
    },
    
    #' 
    #'   \item{edgeWeight(source,destination)}{
    #'   Get the weight of the edge that goes from source vertex to destination 
    #'   vertex after the last iteration. See \code{\link{edgeWeight}}
    #'   }
    #'   
    edgeWeight=function(source=1,destination=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$edgeWeight(source,destination))
      }
      else{
        return(prc$edgeWeight(source,destination,pst,pstid))
      }
    },
    
    #' 
    #'   \item{edge(source,destination)}{Alias for edgeWeight(). See \code{\link{edgeWeight}}}
    #'   
    edge=function(source=1,destination=1){
      return(edgeWeight(source,destination))
    },
    
    #' 
    #'   \item{vertices(community)}{
    #'   Get all vertices belonging to the given community after the last iteration. 
    #'   See \code{\link{vertices}}
    #'   }
    #'   
    vertices=function(community=1){
      if(pst==POSTPROCESSING$NONE){
        return(alg$vertices(community))
      }
      else{
        return(prc$vertices(community,pst,pstid))
      }
    },
    
    #' 
    #'   \item{nodes(community)}{Alias for vertices(community). See \code{\link{vertices}}}
    #'   
    nodes=function(community=1){
      return(vertices(community))
    },
        
    #' 
    #'   \item{communityMapping(differential, file)}{
    #'   Get the community mapping for all communities after the last iteration.
    #'   See \code{\link{communityMapping}}
    #'   }
    #'   
    communityMapping = function(differential=TRUE, file=""){
      if(pst==POSTPROCESSING$NONE){
        if(is.character(file) && length(file)==1 && nchar(file)>0){#file was given
          return(alg$communityMappingFile(differential,file))
        }
        else{
          return(alg$communityMappingMatrix(differential))
        }
      }
      else{
        if(is.character(file) && length(file)==1 && nchar(file)>0){#file was given
          return(prc$communityMappingFile(differential,file,pst,pstid))
        }
        else{
          return(prc$communityMappingMatrix(differential,pst,pstid))
        }
      }
    },
    
    #' 
    #'   \item{time(differential)}{
    #'   Get the cumulative time spent on processing after the last iteration. 
    #'   See \code{\link{time}}
    #'   }
    #'   
    time=function(differential=FALSE){
      if(pst==POSTPROCESSING$NONE){
        return(alg$time(differential))
      }
      else{
        return(prc$time(differential,pst,pstid))
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


#' @name postProcess
#' 
#' @aliases postprocess
#' 
#' @title postProcess(actions)
#'
#' @author poltergeist0
#' 
#' @description 
#' This method receives a list of actions to perform in post processing in the 
#' same order they are listed from left to right.
#' 
#' @details 
#' Several actions of the same type are allowed. They receive an internal ID
#' number that starts at one and increments by one unit with each action of the 
#' same type. Later, this ID can be used to select the intended action and get 
#' results from it.
#' 
#' Post processing can be reset (removed) be setting actions to NULL (default 
#' value)
#' 
#' The format of the actions is a list of action. Each action is a list of the
#' action name (see \code{\link{POSTPROCESSING}}) and parameters. The parameters
#' is a matrix of two columns, the first having the name of the parameter and, 
#' the second, the value of the parameter. The parameters is optional, and may 
#' be missing, in which case default values are used, if required at all.
#' 
#' The parameters accepted by each post processing algorithm can be found on the
#' help page of each respective algorithm.
#' 
#' This slighty awkward syntax is due to R not supporting matrix of matrices.
#' 
#' @rdname postProcess
#' 
#' @docType methods
#' 
#' @usage
# postProcess(actions)
#' postProcess(dyncomm,actions)
#' 
#' @param actions A list of post processing actions/steps
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm postProcess
#' 
#' @return FALSE if any kind of error occurred. Otherwise, TRUE
#'
#' @seealso 
#' \code{\link{DynComm}} 
#' , \code{\link{select}} 
#' , \code{\link{POSTPROCESSING}}
#' 
#' @examples
#' \dontrun{
#' dc<-DynComm()
#' dc$postProcess(
#'   list(
#'     list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE))
#'     ,list(POSTPROCESSING$DENSOPT)
#'     ,list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))
#'   )
#' )
#' # first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
#' # POSTPROCESSING$DENSOPT uses default parameters
#' dc$select(POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
#' dc$select(POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
#' dc$select(POSTPROCESSING$NONE)  #selects the main algorithm results
#' dc$select(POSTPROCESSING$DENSOPT)  #selects the results of densopt
#' dc$postProcess(NULL)  #remove post processing
#' ## or just
#' ## dc$postProcess()
#' }
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' postProcess(dc,
#'   list(
#'     list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE))
#'     ,list(POSTPROCESSING$DENSOPT)
#'     ,list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))
#'   )
#' )
#' # first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
#' # POSTPROCESSING$DENSOPT uses default parameters
#' select(dc,POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
#' select(dc,POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
#' select(dc,POSTPROCESSING$NONE)  #selects the main algorithm results
#' select(dc,POSTPROCESSING$DENSOPT)  #selects the results of densopt
#' postProcess(dc,NULL)  #remove post processing
#' ## or just
#' ## postProcess(dc)
#' }
#' 
# NULL
postProcess <- function(dyncomm,actions=NULL){
  return(dyncomm$postProcess(actions))
}

#' @name select
#' 
#' @title select(postProcessing, id)
#'
#' @author poltergeist0
#' 
#' @description 
#' This method allows for the selection of which result should be shown. Any of 
#' the post processing algorithms and the main algorithm can be choosen.
#' 
#' @details 
#' The ID parameter is used to distinguish between several post processing 
#' algorithms of the same type. It is not required for neither the main 
#' algorithm nor any post processing algorithm type that only appears one time.
#' 
#' The main algorithm can be selected with POSTPROCESSING$NONE (default value) 
#' and the ID is ignored. See \code{\link{POSTPROCESSING}} for other available 
#' algorithms.
#' 
#' If there are no actions defined for post processing, this function fails.
#' 
#' @rdname select
#' 
#' @docType methods
#' 
#' @usage
#' select(dyncomm,postProcessing, id)
#' 
#' @param postProcessing The name of the post processing algorithm. Default 
#'   POSTPROCESSING$NONE. See \code{\link{POSTPROCESSING}}
#' 
#' @param id The ID of the post processing algorithm. Default value is 1
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm select
#' 
#' @return FALSE if the algorithm does not exist in the chain. Otherwise, TRUE
#'
#' @seealso \code{\link{DynComm}} , \code{\link{select}}
#' 
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$postProcess(list(list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE)),list(POSTPROCESSING$DENSOPT),list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))))
#' # first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
#' # POSTPROCESSING$DENSOPT uses default parameters
#' dc$select(POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
#' dc$select(POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
#' dc$select(POSTPROCESSING$NONE)  #selects the main algorithm results
#' }
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' postProcess(dc,list(list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE)),list(POSTPROCESSING$DENSOPT),list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))))
#' # first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
#' # POSTPROCESSING$DENSOPT uses default parameters
#' select(dc,POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
#' select(dc,POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
#' select(dc,POSTPROCESSING$NONE)  #selects the main algorithm results
#' }
#' 
# NULL
select <- function(dyncomm,postProcessing=POSTPROCESSING$NONE, id=1){
  return(dyncomm$select(postProcessing=POSTPROCESSING$NONE, id=1))
}

#' @name results
#' 
#' @title results(differential)
#'
#' @author poltergeist0
#' 
#' @description 
#' This method returns additional results from the selected post processing
#' algorithm or the main algorithm. See \code{\link{select}} to know how to 
#' select an algorithm.
#' 
#' @details 
#' Additional results are any results other than those returned by other 
#' existing functions like \code{\link{quality}}, \code{\link{time}} and 
#' \code{\link{communityMapping}}.
#' 
#' @rdname results
#' 
#' @docType methods
#' 
#' @usage
#' results(differential)
#' results(dyncomm,differential)
#' 
#' @param differential If TRUE, only values that have changed in the latest run 
#'   will be returned
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm results
#' 
#' @return a two column matrix where, the first column is the name of the 
#' result and, the second column is its value.
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$results()
#' }
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' results(dc)
#' }
#' 
# NULL
results <- function(dyncomm,differential=TRUE){
  return(dyncomm$results(differential))
}

#' @name addRemoveEdges
#' 
#' @aliases addRemove add
#' 
#' @title addRemoveEdges(graphAddRemove)
#'
#' @author poltergeist0
#' 
#' @description 
#' This method reads edges from either a matrix or a file and adds or removes 
#' them from the graph.
#' 
#' @details 
#' The matrix must have at least two columns with the source and destination
#' vertices. Optionally, it can have a third column with the weight of the edge.
#' 
#' The file must have only one edge per line, with values separated by a white
#' space (both SPACE and TAB work in any amount and combination).
#' 
#' The first value is the source vertex, the second is the destination vertex, 
#' and the third is the weight.
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
#' @rdname addRemoveEdges
#' 
#' @docType methods
#' 
#' @usage
# addRemoveEdges(graphAddRemove)
#' addRemoveEdges(dyncomm,graphAddRemove)
# addRemove(graphAddRemove)
#' addRemove(dyncomm,graphAddRemove)
# add(graphAddRemove)
#' add(dyncomm,graphAddRemove)
#' 
#' @param graphAddRemove Either the matrix or the filename that contains the 
#'   edges to add/remove
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm addRemoveEdges
#' 
#' @return FALSE if any kind of error occurred. Otherwise, TRUE
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$addRemoveEdges("graphAddRemoveFile.txt")
#' }
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' addRemoveEdges(dc,"graphAddRemoveFile.txt")
#' }
#' 
addRemoveEdges <- function(dyncomm,graphAddRemove){
  return(dyncomm$addRemoveEdges(graphAddRemove))
}
addRemove <- function(dyncomm,graphAddRemove){
  return(dyncomm$addRemoveEdges(graphAddRemove))
}
add <- function(dyncomm,graphAddRemove){
  return(dyncomm$addRemoveEdges(graphAddRemove))
}

#' @name quality
#'
#' @title quality()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the quality measurement of the graph after the last iteration of the 
#' algorithm.
#'
#' @rdname quality
#'
#' @docType methods
#'
#' @usage 
#' quality()
#' quality(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
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
  return(dyncomm$quality())
}

#' @name communityCount
#'
#' @aliases communityCount
#'
#' @title communityCount()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the number of communities after the last iteration of the algorithm.
#'
#' @rdname communityCount
#'
#' @docType methods
#'
#' @usage 
#' communityCount()
#' communityCount(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
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
communityCount <- function(dyncomm){
  return(dyncomm$communityCount())
}

#' @name communities
#'
#' @title communities()
#'
#' @author poltergeist0
#' 
#' @description 
#' This method returns all communities after the last iteration.
#'
#' @rdname communities
#'
#' @docType methods
#'
#' @usage 
#' communities()
#' communities(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
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
communities <- function(dyncomm){
  return(dyncomm$communities())
}

#' @name communityNeighbours
#'
#' @title communityNeighbours(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all neighbours (community connected through direct edges) of the given 
#' community in the graph after the last iteration.
#'
#' @rdname communityNeighbours
#'
#' @docType methods
#'
#' @param community The community to get neighbours from
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
#' 
#' @usage 
#' communityNeighbours(community)
#' communityNeighbours(dyncomm,community)
#'
#' @method DynComm communityNeighbours
#'
#' @return a matrix of all communities in the graph that are neighbours of the 
#' given community and their edge weight
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityNeighbours(community)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityNeighbours(dc,community)
#'}
#'
communityNeighbours <- function(dyncomm,community){
  return(dyncomm$communityNeighbours(community))
}

#' @name communityInnerEdgesWeight
#'
#' @title communityInnerEdgesWeight(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the sum of weights of the inner edges of the given community after the 
#' last iteration.
#'
#' @rdname communityInnerEdgesWeight
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
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
communityInnerEdgesWeight <- function(dyncomm,community){
  return(dyncomm$communityInnerEdgesWeight(community))
}

#' @name communityTotalWeight
#'
#' @title communityTotalWeight(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the sum of weights of all edges of the given community after the last 
#' iteration.
#'
#' @rdname communityTotalWeight
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
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
communityTotalWeight <- function(dyncomm,community){
  return(dyncomm$communityTotalWeight(community))
}

#' @name communityEdgeWeight
#'
#' @title communityEdgeWeight(source,destination)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the weight of the edge that goes from source community to destination 
#' community after the last iteration.
#'
#' @rdname communityEdgeWeight
#'
#' @docType methods
#'
#' @param source The name of the source community that is part of the edge
#' 
#' @param destination The name of the destination community that is part of the 
#'   edge
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
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
communityEdgeWeight <- function(dyncomm,source,destination){
  return(dyncomm$communityEdgeWeight(source,destination))
}

#' @name communityVertexCount
#'
#' @aliases communityNodeCount
#'
#' @title communityVertexCount(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the amount of vertices in the given community after the last iteration.
#'
#' @rdname communityVertexCount
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
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
communityVertexCount <- function(dyncomm,community){
  return(dyncomm$communityVertexCount())
}
communityNodeCount <- function(dyncomm,community){
  return(dyncomm$communityVertexCount())
}

#' @name community
#'
#' @title community(vertex)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the community of the given vertex after the last iteration.
#'
#' @rdname community
#'
#' @docType methods
#'
#' @param vertex The name of the intended vertex
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
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
community <- function(dyncomm,vertex){
  return(dyncomm$community(vertex))
}

#' @name vertexCount
#'
#' @aliases nodesCount
#'
#' @title vertexCount()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the total number of vertices after the last iteration. It can be useful 
#' since vertices can be added, if an edge being added has vertices that do not 
#' exist in the graph, or removed, if they are not part of any edge after 
#' removing an edge.
#'
#' @rdname vertexCount
#'
#' @docType methods
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
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
vertexCount <- function(dyncomm){
  return(dyncomm$vertexCount())
}
nodesCount <- function(dyncomm){
  return(dyncomm$vertexCount())
}

#' @name verticesAll
#'
#' @aliases nodesAll
#'
#' @title verticesAll()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all vertices in the graph after the last iteration.
#'
#' @rdname verticesAll
#'
#' @docType methods
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
#' 
#' @usage 
#' verticesAll()
#' verticesAll(dyncomm)
#' nodesAll()
#' nodesAll(dyncomm)
#'
#' @method DynComm verticesAll
#'
#' @return a list of all vertices in the graph
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$verticesAll()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' verticesAll(dc)
#'}
#'
verticesAll <- function(dyncomm){
  return(dyncomm$verticesAll())
}
nodesAll <- function(dyncomm){
  return(dyncomm$verticesAll())
}

#' @name neighbours
#'
#' @title neighbours(vertex)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all neighbours (vertices connected through direct edges) of the given 
#' vertex in the graph after the last iteration.
#'
#' @rdname neighbours
#'
#' @docType methods
#'
#' @param vertex The vertex to get neighbours from
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
#' 
#' @usage 
#' neighbours(vertex)
#' neighbours(dyncomm,vertex)
#'
#' @method DynComm neighbours
#'
#' @return a matrix of all vertices in the graph that are neighbours of the 
#' given vertex and their edge weight
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$neighbours(vertex)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' neighbours(dc,vertex)
#'}
#'
# NULL
neighbours <- function(dyncomm,vertex){
  return(dyncomm$neighbours(vertex))
}

#' @name edgeWeight
#'
#' @title edgeWeight(source,destination)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the weight of the edge that goes from source vertex to destination vertex 
#' after the last iteration.
#'
#' @rdname edgeWeight
#'
#' @docType methods
#'
#' @param source The name of the source vertex that is part of the edge
#' 
#' @param destination The name of the destination vertex that is part of the edge
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
#' 
#' @usage 
#' edgeWeight(source,destination)
#' edgeWeight(dyncomm,source,destination)
#'
#' @method DynComm edgeWeight
#'
#' @return a floating point number with the weight
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$edgeWeight(12,42)
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' edgeWeight(dc,12,42)
#'}
#'
edgeWeight <- function(dyncomm,source,destination){
  return(dyncomm$edgeWeight(source,destination))
}

#' @name vertices
#'
#' @aliases nodes
#'
#' @title vertices(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all vertices belonging to the given community after the last iteration.
#'
#' @rdname vertices
#'
#' @docType methods
#'
#' @param community The name of the intended community
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
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
  return(dyncomm$vertices(community))
}
nodes <- function(dyncomm,community){
  return(dyncomm$vertices(community))
}

#' @name communityMapping
#'
#' @title communityMapping(differential)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the community mapping for all communities after the last iteration.
#'
#' @details 
#' If file is not given, returns a two column matrix with vertices in 
#' the first column and the communities in the second.
#' 
#' If file is given, returns a single row, single column matrix with TRUE or 
#' FALSE, depending whether if writing to file succeeded or failed, respectively.
#' 
#' @rdname communityMapping
#'
#' @docType methods
#'
#' @param differential If TRUE, only values that have changed in the latest run 
#' will be returned
#' 
#' @param file If given, outputs the community mapping to the given file instead 
#' of the console
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
#' 
#' @usage 
#' communityMapping(differential)
#' communityMapping(dyncomm,differential)
#' communityMapping(dyncomm,differential,file)
#'
#' @method DynComm communityMapping
#'
#' @return a matrix with either the community mapping or a boolean value
#'
#' @examples
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' dc$communityMapping()
#'}
#' \dontrun{
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#' communityMapping(dc,file="")
#'}
#'
communityMapping <- function(dyncomm,differential=TRUE, file=""){
  return(dyncomm$communityMapping(differential,file=""))
}

#' @name time
#'
# @aliases time
#'
#' @title time(differential=FALSE)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the time, in nanoseconds, spent on processing after the last iteration.
#'
#' @details 
#' If the differential parameter is set, the time taken by the last iteration
#' will be returned. Otherwise, the default behaviour is to, return the 
#' accumulated time spent on processing since the creation of the DynComm 
#' object.
#' 
#' @rdname time
#'
#' @docType methods
#'
#' @param differential Select between differential and accumulated time.
#' 
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
#' 
#' @usage
#' time(dyncomm)
#' time(dyncomm,differential)
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
#' time(dc,differential=FALSE)
#' ## 2.3
#' }
#' 
time <- function(dyncomm,differential=FALSE){
  return(dyncomm$time(differential))
}
