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
# This file should only be changed at predefined locations to add algorithm 
# parameters and library imports need by new (main or post processing) algorithms.
#
# Libraries required by algorithms should be listed using roxygen after the 
# marker that says "List imports here".
#
# Algorithm parameters should be added to the matrix after the marker that says 
# "add parameters here". The first column is the parameter name and the second is 
# the default value.
#
# Algorithm parameters should be documented after the marker that says "document
# parameters here". The first column is the parameter name and the second is the 
# default value.
#
# Main algorithms are handled by the DynCommMain object. Post processing
# algorithms are handled by the DynCommPostProcess object. Changes should be in
# those objects. They can be found in the files with the same name as the object.
#
# More developer information can be found in the project source page on GitHub at
# https://github.com/softskillsgroup/DynComm-R-package
#
#
# Author: poltergeist0
# Date: 2019-01-01


########################### Package Documentation ###########################
#' @name DynComm-package
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
#' @rdname DynComm-package
#' 
#' @docType package
########################### List imports here ###########################
#' @import Rcpp methods igraph
#' @importFrom Rdpack reprompt
#' @importFrom utils write.table
#' @useDynLib DynComm
#' 
#' @section Referenced Work:
#' This package uses the following work as reference material for the 
#' implementation of the algorithms.
#' @references
#' \href{https://github.com/softskillsgroup/DynComm-R-package}{GitHub project source}
#' \insertRef{cordeiro2016dynamic}{DynComm}
#' \insertRef{Rossetti:2017:TOA:3127967.3128003}{DynComm}
#' \insertRef{RG17}{DynComm}
#' \insertRef{Sarmento2019Apr}{DynComm}
#' 
#' @seealso \code{\link{DynComm}} , \code{\link{DynComm-package-dev}}
#' 
#' 
NULL

########################### Package Developer Documentation ###########################
#' @name DynComm-package-dev
#' 
#' @title DynComm Documentation for Developers
#' 
#' @author poltergeist0
#' 
#' @rdname DynComm-package-dev
#' 
#' @docType class
#' 
#' @description 
#' Instructs delevopers how to add new algorithms, criterion and post processing
#' algorithms to the DynComm package.
#' 
#' @details 
#' Implementing new algorithms in new packages is a lot of work.
#' 
#' With this package, we try to accomplish two things: make the addition of new 
#' algorithms easier and concentrate dynamic community detection algorithms in a
#' single package, no matter the language used to write them.
#' 
#' Always read the entirety of the instructions even if they do not seem to apply 
#' to your case. Care was taken to make the instructions as general as possible, 
#' mentioning specificities only when they differ from the general case.
#' 
#' Most of the instructions described are for algorithms writen in R, since it is 
#' the language used for the user interface and is the easiest to integrate.
#' 
#' Algorithms writen in other languages will also need this information in order 
#' to know the types of the inputs and outputs of the functions.
#' 
#' It is advisable to always read the "Developer Notice" on the beginning of the 
#' files mentioned in these instructions. It will contain useful information about
#' the source code on the file and where new code can be added.
#' 
#' Whenever "Project", "Project Page" or "Project source" is mentioned, the 
#' developer should know that it refers to the project source code page on 
#' GitHub (\href{https://github.com/softskillsgroup/DynComm-R-package}{GitHub project source}).
#' 
#' The project source has the following organization:
#' \describe{
#'   \item{\strong{Root}}{
#' This is the root folder of the project source code.
#' It contains files about the project source code and the folders "dev", "R-CRAN", 
#' "test" and "standalone".
#'     \describe{
#'       \item{\strong{dev}}{
#' Folder with templates for developers of new main algorithms, new criterion and 
#' new post processing algorithms. Also contains these instructions in text 
#' format.
#'       }
#'       \item{\strong{R-CRAN}}{
#' Contains the source code for the actual DynComm package. Internally, has the 
#' same organization as required by any R package project. The most important 
#' folders are named "inst", "src" and "R".
#'         \describe{
#'           \item{\strong{inst}}{
#' Contains a file named "REFERENCES.bib" where bibliographic references are 
#' stored using the bibtex format.
#'           }
#'           \item{\strong{src}}{
#' The root of this folder contains files in other languages that implement an
#' interaction layer between R and the respective programming language. The actual
#' source code that implements a certain algorithm is placed inside a sub-folder
#' named after the programming language inside the folder "base".\cr
#' As an example, the Dynamic Louvain algorithm used in this package was 
#' implemented in C++11. There is a file named "DynCommRcpp.cpp" which
#' implements the interaction layer using Rcpp. This layer only converts data 
#' types from R to C++, instantiates a Louvain object and redirects calls to 
#' methods of that object on the C++ source file named "DynCommBase.h".
#'           }
#'           \item{\strong{R}}{
#' This is the folder that contains all R source code files where the architecture 
#' of the package is implemented, along with some main algorithms and post 
#' processing algorithms, and all the documentation.\cr
#' Some of the adaptation layers for some programming languages, like Python, are 
#' in this folder since they must be implemented inside an R source file, as 
#' opposed to programming languages like C++ where Rcpp must be inside a C++ 
#' source code file.
#'           }
#'         }
#'       }
#'       \item{\strong{test}}{
#' Folder with a few sample files with data that can be used to run examples and 
#' test the code.
#'       }
#'       \item{\strong{standalone}}{
#' Contains the standalone (command line) versions of the algorithms, for the 
#' algorithms that provide them, in case anyone wants to run the algorithms 
#' outside of the R environment.\cr
#' Each program is inside a folder with the name of the programming language used
#' to implement it. As an example, C++ programs are inside a folder named "Cpp".\cr
#' Not all algorithms may be implemented and some functionality might be slightly
#' different from the one used in the R environment.
#' Post processing algorithms are not provided.
#'       }
#'     }
#'   }
#' }
#' 
#' In case of doubt, missing information or if you are implementing in a language 
#' that is still not supported, contact the maintainer of the package.
#' 
#' Follow the instructions of the links below in order to add your main algorithm,
#' criterion or post processing algorithm, respectively. 
#' 
#' @section I am implementing a:
#' \describe{
#'   \item{Main algorithm}{See \code{\link{ALGORITHM-dev}}}
#'   \item{Criterion}{See \code{\link{CRITERION-dev}}}
#'   \item{Post processing algorithm}{See \code{\link{POSTPROCESSING-dev}}}
#' }
#' 
#' @seealso \code{\link{DynComm}} , \code{\link{DynComm-package}}
#' 
#' 
NULL

source('R/DynCommMain.R')
source('R/DynCommPostProcess.R')

########################### API Documentation ###########################
#' @name DynComm
#' 
# @aliases Dyncomm dyncomm
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
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#' matrix(
#' c(10,20,10,30,20,30,30,60,40,60,40,50,50,70,60,70)
#' ,ncol=2,byrow=TRUE)
#' )
#' ## or
#' ## dc$addRemoveEdges("initial_graph.txt")
#' dc$communityCount()
#' ## You can use the non inline version of the functions
#' DynComm.communities(dc)
#' ## Several alias have been defined.
#' ## In this case, communityNodeCount is alias of communityVertexCount
#' dc$communityNodeCount(10)
#' dc$communityNeighbours(10)
#' dc$communityInnerEdgesWeight(10)
#' dc$communityTotalWeight(10)
#' dc$communityEdgeWeight(10,40)
#' dc$community(10) ##this parameter is a vertex not a community. Do not confuse them 
#' dc$vertices(10)
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
########################### document parameters here ###########################
#' @section PARAMETERS:
#' A two column matrix defining additional parameters to be passed to the
#' selected ALGORITHM and CRITERION.
#' The first column names the parameter and the second defines its value.
#' \describe{
#'   \item{
#'   c
#'   }{
#'   Owsinski-Zadrozny quality function parameter. 
#'   Values [0.0:1.0]. Default: 0.5
#'   }
#'   \item{
#'   k
#'   }{
#'   Shi-Malik quality function kappa_min value. 
#'   Value > 0 . Default 1
#'   }
#'   \item{
#'   w
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
#'   e
#'   }{
#'   Stops when, on a cycle of the algorithm, the quality is increased by less 
#'   than the value given in this parameter.
#'   Value > 0 . Default 0.01
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

# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
DynComm <- function(Algorithm=ALGORITHM$LOUVAIN,Criterion=CRITERION$MODULARITY,Parameters=NULL)
{
  
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  ########## constructor #############
  prm <- NULL
  # print(Parameters)
  if(is.null(Parameters) || !is.matrix(Parameters) || ncol(Parameters)!=2){
    # not a valid parameters matrix. Use default values for all parameters
    prm<-matrix(
      c(
        "c",0.5
        ,"k",1
        ,"e","0.000001"
        ,"w", "FALSE"
        ,"cv", "TRUE"
  ########################### add parameters here ###########################
        
      ),ncol=2, byrow=TRUE
    )
  }
  else{
    # is valid matrix
    prm<-Parameters
  }
  alg <- DynCommMain(Algorithm,Criterion,prm)  #main algorithm
  pst <- POSTPROCESSING$NONE  #post processing flag redirects function calls to post processing object. Set to NONE on add/remove edge
  pstid <- 1
  act <- NULL   #list of actions and their parameters to recreate after adding/removing edges. It is delayed until requested.
  prc <- NULL   #pointer to last object in list of post processing objects. They are chained in series. Head is main algorithm
  
  ## internal function that recreates the post processing chain
  internalRecreatePostProcessing = function(){
    # invalidate previous post processing. Results are outdated
    assign("pst",POSTPROCESSING$NONE,thisEnv)
    assign("prc",NULL,thisEnv)
    b<-FALSE
    # validate no actions
    if(is.null(act) || length(act)<=0){
      #setting to NULL always succeeds
      assign("act", NULL,thisEnv)
      return(TRUE)
    }
    else{#act not NULL and has values
      # validate actions list does not contain POSTPROCESSING$NONE
      for (cnt in act) {
        if((!is.null(cnt)) && is.list(cnt) && length(cnt)>0){
           browser()
            if(cnt[[1]]!=POSTPROCESSING$NONE){
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
            q<-prc$exists(cnt[[1]],i)
            # increment id while a post processing object of the given type exists
            while(q){
              i<-i+1
              q<-prc$exists(cnt[[1]],i)
            }
          }
        }
        # select the latest post processing algorithm and id as default for posterior user operations
        assign("pst", cnt[[1]],thisEnv)
        assign("pstid", i,thisEnv)
        # create post processing object and assign it to the end of the queue
        tmp <- DynCommPostProcess(pst,pstid,prc,cnt[2])
        if(is.null(tmp)){
          #TODO improve error message
          print("Invalid post processing")
          print(pst)
          print(cnt[[1]])
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
  
  internalCommunityVertexCount=function(community=1){
    if(pst==POSTPROCESSING$NONE){
      return(alg$communityVertexCount(community))
    }
    else{
      return(prc$communityVertexCount(community,pst,pstid))
    }
  }
  
  internalAddRemoveEdges = function(graphAddRemove=""){
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
      b<-internalRecreatePostProcessing()
    }
    return(b)
  }
  
  internalVertexCount=function(){
    if(pst==POSTPROCESSING$NONE){
      return(alg$vertexCount())
    }
    else{
      return(prc$vertexCount(pst,pstid))
    }
  }
  
  internalVerticesAll=function(){
    if(pst==POSTPROCESSING$NONE){
      return(alg$verticesAll())
    }
    else{
      return(prc$verticesAll(pst,pstid))
    }
  }
  
  internalVertices=function(community=1){
    if(pst==POSTPROCESSING$NONE){
      return(alg$vertices(community))
    }
    else{
      return(prc$vertices(community,pst,pstid))
    }
  }
  
  internalEdgeWeight=function(source=1,destination=1){
    if(pst==POSTPROCESSING$NONE){
      return(alg$edgeWeight(source,destination))
    }
    else{
      return(prc$edgeWeight(source,destination,pst,pstid))
    }
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
        return(internalRecreatePostProcessing())
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
          b<-internalRecreatePostProcessing()
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
      # # invalidate previous post processing queue. Results will be outdated
      # assign("pst",POSTPROCESSING$NONE,thisEnv)
      # assign("prc",NULL,thisEnv)
      # # update graph
      # b<-FALSE
      # if(is.matrix(graphAddRemove) && ncol(graphAddRemove)>1 && ncol(graphAddRemove)<4){#test for matrix
      #   b<-alg$addRemoveEdgesMatrix(graphAddRemove)
      # }
      # else if(is.character(graphAddRemove) && length(graphAddRemove)==1 && nchar(graphAddRemove)>0){#file was given
      #   b<-alg$addRemoveEdgesFile(graphAddRemove)
      # }
      # else{#neither file nor matrix
      #   print("Invalid input")
      #   return(FALSE)
      # }
      # if(b){
      #   #attempt to recreate post processing
      #   b<-internalRecreatePostProcessing()
      # }
      # return(b)
      return(internalAddRemoveEdges(graphAddRemove))
    },

    #' 
    #'   \item{addRemove(graphAddRemove)}{Alias for addRemoveEdges(). See \code{\link{addRemoveEdges}}}
    #'   
    addRemove = function(graphAddRemove=""){
      # return(addRemoveEdges(graphAddRemove))
      return(internalAddRemoveEdges(graphAddRemove))
    },
    
    #' 
    #'   \item{add(graphAddRemove)}{Alias for addRemoveEdges(). See \code{\link{addRemoveEdges}}}
    #'   
    add = function(graphAddRemove=""){
      # return(addRemoveEdges(graphAddRemove))
      return(internalAddRemoveEdges(graphAddRemove))
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
    #'   \item{communitiesEdgeCount()}{Get the number of community to community edges in the graph. See \code{\link{communitiesEdgeCount}}}
    #'   
    communitiesEdgeCount=function() {
      if(pst==POSTPROCESSING$NONE){
        return(alg$communitiesEdgeCount())
      }
      else{
        return(prc$communitiesEdgeCount(pst,pstid))
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
      # if(pst==POSTPROCESSING$NONE){
      #   return(alg$communityVertexCount(community))
      # }
      # else{
      #   return(prc$communityVertexCount(community,pst,pstid))
      # }
      return(internalCommunityVertexCount(community))
    },
        
    #' 
    #'   \item{communityNodeCount(community)}{Alias for communityVertexCount(). See \code{\link{communityVertexCount}}}
    #'   
    communityNodeCount=function(community=1){
      # return(communityVertexCount(community))
      return(internalCommunityVertexCount(community))
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
      # if(pst==POSTPROCESSING$NONE){
      #   return(alg$vertexCount())
      # }
      # else{
      #   return(prc$vertexCount(pst,pstid))
      # }
      return(internalVertexCount())
    },

    #' 
    #'   \item{nodesCount()}{Alias for vertexCount(). See \code{\link{vertexCount}}}
    #'   
    nodesCount=function(){
      # return(vertexCount())
      return(internalVertexCount())
    },
    
    #' 
    #'   \item{verticesAll()}{
    #'   Get all vertices in the graph after the last iteration. See \code{\link{verticesAll}}
    #'   }
    #'   
    verticesAll=function(){
      # if(pst==POSTPROCESSING$NONE){
      #   return(alg$verticesAll())
      # }
      # else{
      #   return(prc$verticesAll(pst,pstid))
      # }
      return(internalVerticesAll())
    },
        
    #' 
    #'   \item{nodesAll()}{Alias for verticesAll(). See \code{\link{verticesAll}}}
    #'   
    nodesAll=function(){
      # return(verticesAll())
      return(internalVerticesAll())
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
      # if(pst==POSTPROCESSING$NONE){
      #   return(alg$edgeWeight(source,destination))
      # }
      # else{
      #   return(prc$edgeWeight(source,destination,pst,pstid))
      # }
      return(internalEdgeWeight(source,destination))
    },
    
    #' 
    #'   \item{edge(source,destination)}{Alias for edgeWeight(). See \code{\link{edgeWeight}}}
    #'   
    edge=function(source=1,destination=1){
      # return(edgeWeight(source,destination))
      return(internalEdgeWeight(source,destination))
    },
    
    #' 
    #'   \item{vertices(community)}{
    #'   Get all vertices belonging to the given community after the last iteration. 
    #'   See \code{\link{vertices}}
    #'   }
    #'   
    vertices=function(community=1){
      # if(pst==POSTPROCESSING$NONE){
      #   return(alg$vertices(community))
      # }
      # else{
      #   return(prc$vertices(community,pst,pstid))
      # }
      return(internalVertices(community))
    },
    
    #' 
    #'   \item{nodes(community)}{Alias for vertices(community). See \code{\link{vertices}}}
    #'   
    nodes=function(community=1){
      # return(vertices(community))
      return(internalVertices(community))
    },
        
    #' 
    #'   \item{edgeCount()}{Get the number of vertex to vertex edges in the graph. See \code{\link{edgeCount}}}
    #'   
    edgeCount=function() {
      if(pst==POSTPROCESSING$NONE){
        return(alg$edgeCount())
      }
      else{
        return(prc$edgeCount(pst,pstid))
      }
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
      # print("DynComm")
      # print(pst)
      if(pst==POSTPROCESSING$NONE){
        return(alg$time(differential))
      }
      else{
        return(prc$time(differential,pst,pstid))
      }
    }
    
    #' 
    #'   \item{version()}{
    #'   Get the source code versions of the different sources. 
    #'   See \code{\link{version}}
    #'   }
    #'   
#    version=function(){
#      # print("DynComm")
#      # print(pst)
#      if(pst==POSTPROCESSING$NONE){
#        return(alg$version())
#      }
#      else{
#        return(prc$version(pst,pstid))
#      }
#    }
    
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
# @aliases postprocess
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
#' value) or passing an empty list.
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
#' DynComm.postProcess(dyncomm,actions)
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
#' @export DynComm.postProcess
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#'   dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#'   dc$addRemoveEdges(
#'    matrix(
#'       c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#'   )
#'   dc$postProcess(
#'     list(
#'  	     list(POSTPROCESSING$DENSOPT)
#'   	)
#'   )
#'  dc$select(POSTPROCESSING$DENSOPT)  #selects the results of densopt
#'  dc$select(POSTPROCESSING$NONE)  #selects the main algorithm results
#'  dc$postProcess(NULL)  #remove post processing
#'  ## or just
#'  ## dc$postProcess()
#'
DynComm.postProcess <- function(dyncomm,actions=NULL){
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
#' DynComm.select(dyncomm,postProcessing, id)
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
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.select
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#'   dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
#'   dc$addRemoveEdges(
#'    matrix(
#'       c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#'   )
#'   dc$postProcess(
#'     list(
#'  	     list(POSTPROCESSING$DENSOPT)
#'   	)
#'   )
#'  dc$select(POSTPROCESSING$DENSOPT)  #selects the results of densopt
#'  dc$select(POSTPROCESSING$NONE)  #selects the main algorithm results
#' 
#'  dc$postProcess(NULL)  #remove post processing
#'  ## or just
#'  ## dc$postProcess()
#'
#' 
DynComm.select <- function(dyncomm,postProcessing=POSTPROCESSING$NONE, id=1){
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
#' Passing the parameter differential set to TRUE, will return only results that
#' have changed from the previous to last iteration.
#' 
#' @rdname results
#' 
#' @docType methods
#' 
#' @usage
# results(differential)
#' DynComm.results(dyncomm,differential)
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
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.results
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(10,20,10,30,20,30,30,60,40,60,40,50,50,70,60,70)
#'    ,ncol=2,byrow=TRUE)
#' )
#' dc$results()
#' 
DynComm.results <- function(dyncomm,differential=TRUE){
  return(dyncomm$results(differential))
}

#' @name addRemoveEdges
#' 
#' @aliases DynComm.addRemove DynComm.add
#' 
#' @title addRemoveEdges(graphAddRemove)
#'
#' @author poltergeist0
#' 
#' @description 
#' This method reads edges from either a matrix or a file and adds or removes 
#' them to/from the graph.
#' 
#' @details 
#' If the weight is exactly zero, the edge is removed from the graph.
#' 
#' If a vertex, mentioned in the source or destination, does not exist it will be 
#' added to the graph.
#' 
#' If any post processing algorithm exists, it is automatically calculated after
#' the main algorithm.
#' 
#' \describe{
#'   \item{Matrix input}{
#' The matrix must have at least two columns with the source and destination
#' vertices.
#' 
#' If all edges are to be added with the default weight, a third column 
#' is optional. 
#' 
#' If any edge is to be removed, the third column is mandatory.
#'   }
#'   \item{File input}{
#' The file must have only one edge per line, with values separated by a white
#' space (both SPACE and TAB work in any amount and combination). The line must 
#' end with a newline character (also known as linefeed, LF or '\\n').
#' 
#' The first value is the source vertex, the second is the destination vertex, 
#' and the third is the weight.
#' 
#' The weight can be ommited if the edge is to be added using the default weight
#' of 1 (one), or if the parameter to ignore weights was set.
#' 
#' The method detects automatically if the weight is present on a row by row basis 
#' so some rows may have weights defined and others not.
#'   }
#'}
#' 
#' @rdname addRemoveEdges
#' 
#' @docType methods
#' 
#' @usage
# addRemoveEdges(graphAddRemove)
#' DynComm.addRemoveEdges(dyncomm,graphAddRemove)
# addRemove(graphAddRemove)
#' DynComm.addRemove(dyncomm,graphAddRemove)
# add(graphAddRemove)
#' DynComm.add(dyncomm,graphAddRemove)
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
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.addRemoveEdges
#'  
#' @export DynComm.addRemove
#'  
#' @export DynComm.add
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' #adding edges with the use of a matrix
#' dc$addRemoveEdges(
#'  matrix(
#'    c(10,20,10,30,20,30,30,60,40,60,40,50,50,70,60,70)
#'    ,ncol=2,byrow=TRUE)
#' )
#'
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' #adding edges with the use of a file
#' dc$addRemoveEdges(system.file("extdata","graphAddRemoveFile.txt",package = "DynComm"))
#' 

DynComm.addRemoveEdges <- function(dyncomm,graphAddRemove){
  return(dyncomm$addRemoveEdges(graphAddRemove))
}
DynComm.addRemove <- function(dyncomm,graphAddRemove){
  return(dyncomm$addRemoveEdges(graphAddRemove))
}
DynComm.add <- function(dyncomm,graphAddRemove){
  return(dyncomm$addRemoveEdges(graphAddRemove))
}

#' @name quality
#'
#' @title quality()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the quality measurement of the graph from the selected post processing
#' algorithm or the main algorithm, after the last iteration.
#'
#' @rdname quality
#'
#' @docType methods
#'
#' @usage 
# quality()
#' DynComm.quality(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm quality
#'
#' @return a floating point number
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.quality
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(10,20,10,30,20,30,30,60,40,60,40,50,50,70,60,70)
#'    ,ncol=2,byrow=TRUE)
#' )
#' dc$quality()
#'
DynComm.quality <- function(dyncomm){
  return(dyncomm$quality())
}

#' @name communityCount
#'
# @aliases communityCount
#'
#' @title communityCount()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the number of communities from the selected post processing algorithm or 
#' the main algorithm, after the last iteration.
#'
#' @rdname communityCount
#'
#' @docType methods
#'
#' @usage 
# communityCount()
#' DynComm.communityCount(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm communityCount
#'
#' @return an unsigned integer value with the number of communities
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communityCount
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$communityCount()
#'
DynComm.communityCount <- function(dyncomm){
  return(dyncomm$communityCount())
}

#' @name communities
#'
#' @title communities()
#'
#' @author poltergeist0
#' 
#' @description 
#' This method returns all communities from the selected post processing
#' algorithm or the main algorithm, after the last iteration.
#'
#' @rdname communities
#'
#' @docType methods
#'
#' @usage 
# communities()
#' DynComm.communities(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm communities
#'
#' @return a list of all communities
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communities
#'
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(10,20,10,30,20,30,30,60,40,60,40,50,50,70,60,70)
#'    ,ncol=2,byrow=TRUE)
#' )
#' 
#' dc$communities()
#'
DynComm.communities = function(dyncomm){
  return(dyncomm$communities())
}

#' @name communitiesEdgeCount
#'
#' @title communitiesEdgeCount()
#'
#' @author poltergeist0
#' 
#' @description 
#' This method returns the number of community to community edges in the graph
#' from the selected post processing algorithm or the main algorithm, after the 
#' last iteration.
#'
#' @rdname communitiesEdgeCount
#'
#' @docType methods
#'
#' @usage 
# communitiesEdgeCount()
#' DynComm.communitiesEdgeCount(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm communitiesEdgeCount
#'
#' @return the number of community to community edges in the graph
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communitiesEdgeCount
#'
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$communitiesEdgeCount()
#'
DynComm.communitiesEdgeCount=function(dyncomm) {
  return(dyncomm$communitiesEdgeCount())
}

#' @name communityNeighbours
#'
#' @title communityNeighbours(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all neighbours (communities connected through direct edges) of the given 
#' community in the graph from the selected post processing algorithm or the main 
#' algorithm, after the last iteration.
#' 
#' @details 
#' The return value is a matrix with two columns. The first is the neighbour and
#' the second is the weight of the edge that connects them.
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
# communityNeighbours(community)
#' DynComm.communityNeighbours(dyncomm,community)
#'
#' @method DynComm communityNeighbours
#'
#' @return a matrix of all communities in the graph that are neighbours of the 
#' given community and their edge weight
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communityNeighbours
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' ) 
#' 
#' dc$communities()
#' dc$communityNeighbours(12)
#'
DynComm.communityNeighbours <- function(dyncomm,community){
  return(dyncomm$communityNeighbours(community))
}

#' @name communityInnerEdgesWeight
#'
#' @title communityInnerEdgesWeight(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the sum of weights of the inner edges of the given community from the 
#' selected post processing algorithm or the main algorithm, after the last 
#' iteration.
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
# communityInnerEdgesWeight(community)
#' DynComm.communityInnerEdgesWeight(dyncomm,community)
#'
#' @method DynComm communityInnerEdgesWeight
#'
#' @return a floating point number with the weight
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communityInnerEdgesWeight
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' dc$communities()
#' dc$communityInnerEdgesWeight(1)
#' dc$communityInnerEdgesWeight(0)
#'
DynComm.communityInnerEdgesWeight <- function(dyncomm,community){
  return(dyncomm$communityInnerEdgesWeight(community))
}

#' @name communityTotalWeight
#'
#' @title communityTotalWeight(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the sum of weights of all edges of the given community from the selected 
#' post processing algorithm or the main algorithm, after the last iteration.
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
# communityTotalWeight(community)
#' DynComm.communityTotalWeight(dyncomm,community)
#'
#' @method DynComm communityTotalWeight
#'
#' @return a floating point number with the weight
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communityTotalWeight
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$communityTotalWeight(1)
#' dc$communityTotalWeight(12)
#'
DynComm.communityTotalWeight <- function(dyncomm,community){
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
#' community from the selected post processing algorithm or the main algorithm, 
#' after the last iteration.
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
# communityEdgeWeight(source,destination)
#' DynComm.communityEdgeWeight(dyncomm,source,destination)
#'
#' @method DynComm communityEdgeWeight
#'
#' @return a floating point number with the weight
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communityEdgeWeight
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$communityEdgeWeight(0,12)
#'
DynComm.communityEdgeWeight <- function(dyncomm,source,destination){
  return(dyncomm$communityEdgeWeight(source,destination))
}

#' @name communityVertexCount
#'
#' @aliases DynComm.communityNodeCount
#'
#' @title communityVertexCount(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the amount of vertices in the given community from the selected post 
#' processing algorithm or the main algorithm, after the last iteration.
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
# communityVertexCount(community)
#' DynComm.communityVertexCount(dyncomm,community)
# communityNodeCount(community)
#' DynComm.communityNodeCount(dyncomm,community)
#'
#' @method DynComm communityVertexCount
#'
#' @return an unsigned integer with the number of vertices in the given community
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communityVertexCount
#'  
#' @export DynComm.communityNodeCount
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$communityVertexCount(12)
#'
DynComm.communityVertexCount <- function(dyncomm,community){
  return(dyncomm$communityVertexCount())
}
DynComm.communityNodeCount <- function(dyncomm,community){
  return(dyncomm$communityVertexCount())
}

#' @name community
#'
#' @title community(vertex)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the community of the given vertex from the selected post processing
#' algorithm or the main algorithm, after the last iteration.
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
# community(vertex)
#' DynComm.community(dyncomm,vertex)
#'
#' @method DynComm community
#'
#' @return an unsigned integer with the community of the given vertex
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.community
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$community(1)
#'
DynComm.community <- function(dyncomm,vertex){
  return(dyncomm$community(vertex))
}

#' @name vertexCount
#'
#' @aliases DynComm.nodesCount
#'
#' @title vertexCount()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the total number of vertices from the selected post processing
#' algorithm or the main algorithm, after the last iteration. 
#' 
#' @details
#' It can be useful since vertices can be added, if an edge being added has vertices 
#' that do not exist in the graph, or removed, if they are not part of any edge after 
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
# vertexCount()
#' DynComm.vertexCount(dyncomm)
# nodesCount()
#' DynComm.nodesCount(dyncomm)
#'
#' @method DynComm vertexCount
#'
#' @return an unsigned integer with the number of vertices in the graph
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.vertexCount
#'  
# @export DynComm.nodeCount
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$communityVertexCount(12)
#'
DynComm.vertexCount <- function(dyncomm){
  return(dyncomm$vertexCount())
}
DynComm.nodesCount <- function(dyncomm){
  return(dyncomm$vertexCount())
}

#' @name verticesAll
#'
#' @aliases DynComm.nodesAll
#'
#' @title verticesAll()
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all vertices in the graph from the selected post processing algorithm or 
#' the main algorithm, after the last iteration.
#'
#' @rdname verticesAll
#'
#' @docType methods
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#' function call
#' 
#' @usage 
# verticesAll()
#' DynComm.verticesAll(dyncomm)
# nodesAll()
#' DynComm.nodesAll(dyncomm)
#'
#' @method DynComm verticesAll
#'
#' @return a list of all vertices in the graph
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.verticesAll
#'  
#' @export DynComm.nodesAll
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$verticesAll()
#'
DynComm.verticesAll <- function(dyncomm){
  return(dyncomm$verticesAll())
}
DynComm.nodesAll <- function(dyncomm){
  return(dyncomm$verticesAll())
}

#' @name edgeCount
#'
#' @title edgeCount()
#'
#' @author poltergeist0
#' 
#' @description 
#' This method returns the number of vertex to vertex edges in the graph from the
#' selected post processing algorithm or the main algorithm, after the last 
#' iteration.
#'
#' @rdname edgeCount
#'
#' @docType methods
#'
#' @usage 
# edgeCount()
#' DynComm.edgeCount(dyncomm)
#'
#' @param dyncomm A DynComm object, if not using the inline version of the 
#'   function call
#' 
#' @method DynComm edgeCount
#'
#' @return the number of vertex to vertex edges in the graph
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.edgeCount
#'
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$edgeCount()
#'
DynComm.edgeCount=function(dyncomm) {
  return(dyncomm$edgeCount())
}

#' @name neighbours
#'
#' @title neighbours(vertex)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all neighbours (vertices connected through direct edges) of the given 
#' vertex in the graph from the selected post processing algorithm or the main 
#' algorithm, after the last iteration.
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
# neighbours(vertex)
#' DynComm.neighbours(dyncomm,vertex)
#'
#' @method DynComm neighbours
#'
#' @return a matrix of all vertices in the graph that are neighbours of the 
#' given vertex and their edge weight
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.neighbours
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$neighbours(2)
#'
DynComm.neighbours <- function(dyncomm,vertex){
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
#' from the selected post processing algorithm or the main algorithm, after the 
#' last iteration.
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
# edgeWeight(source,destination)
#' DynComm.edgeWeight(dyncomm,source,destination)
#'
#' @method DynComm edgeWeight
#'
#' @return a floating point number with the weight
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.edgeWeight
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$edgeWeight(0,2)
#'
DynComm.edgeWeight <- function(dyncomm,source,destination){
  return(dyncomm$edgeWeight(source,destination))
}

#' @name vertices
#'
#' @aliases DynComm.nodes
#'
#' @title vertices(community)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get all vertices belonging to the given community from the selected post 
#' processing algorithm or the main algorithm, after the last iteration.
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
# vertices(community)
#' DynComm.vertices(dyncomm,community)
# nodes(community)
#' DynComm.nodes(dyncomm,community)
#'
#' @method DynComm vertices
#'
#' @return a list of vertices belonging to the given community
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.vertices
#'  
#' @export DynComm.nodes
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$vertices(12)
#'
DynComm.vertices <- function(dyncomm,community){
  return(dyncomm$vertices(community))
}
DynComm.nodes <- function(dyncomm,community){
  return(dyncomm$vertices(community))
}

#' @name communityMapping
#'
#' @title communityMapping(differential, file)
#'
#' @author poltergeist0
#' 
#' @description 
#' Get the community mapping for all communities from the selected post processing
#' algorithm or the main algorithm, after the last iteration.
#'
#' @details 
#' If file is not given, returns a two column matrix with vertices in 
#' the first column and the communities in the second.
#' 
#' If file is given, returns a single row, single column matrix with TRUE or 
#' FALSE, depending whether if writing to file succeeded or failed, respectively.
#' 
#' When writing to file, if the Community-Vertex program parameter is TRUE, each 
#' line of the file will have the community first, followed by a list of vertices
#' that belong to the community. If that parameter is FALSE, each line will have
#' a single vertex followed by its community. All values are separated by a white
#' character.
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
# communityMapping(differential)
#' DynComm.communityMapping(dyncomm,differential, file)
#'
#' @method DynComm communityMapping
#'
#' @return a matrix with either the community mapping or a boolean value
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.communityMapping
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$communities()
#' dc$communityMapping()
#'
DynComm.communityMapping <- function(dyncomm,differential=TRUE, file=""){
  return(dyncomm$communityMapping(differential,file))
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
#' If post processing exists, the time returned by this function will include
#' the processing time of all post processing algorithms up to the selected one.
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
# time()
#' DynComm.time(dyncomm,differential)
#' 
#' @method DynComm time
#'
#' @return an unsigned integer with the total processing time
#'
#' @seealso \code{\link{DynComm}} , \code{\link{postProcess}}
#' 
#' @export DynComm.time
#'  
#' @export
#'
#' @examples
#' library(DynComm)
#' 
#' Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
#' dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
#' dc$addRemoveEdges(
#'  matrix(
#'    c(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,12,13,1,1,1,2,2,2,18,12,19,20,2,3,11,12,4,9,5,9,22)
#'       ,ncol=2)
#' )
#' 
#' dc$time()
#' 
DynComm.time <- function(dyncomm,differential=FALSE){
  return(dyncomm$time(differential))
}
