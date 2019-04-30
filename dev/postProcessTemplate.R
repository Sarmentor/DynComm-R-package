########################### Developer Notice ###########################
# Description:
# This file holds the density optimization post processing algorithm.
#
# Author: 
#   poltergeist0: Algorithm convertion to API
#   Rui Sarmento: Algorithm concept and implementation
#
# Date: 2019-01-01



#' @name DensOpt
#' 
#' @keywords internal
#' 
#' @aliases Densopt densopt
#' 
#' @title DensOpt
#'
#' @author poltergeist0
#' 
#' @description 
#' Implementation of the density optimization algorithm as a post processing
#' algorithm.
#' 
# @details 
#' 
#'
#' @rdname DensOpt
#' 
#' @docType class
#' 
#' @usage DensOpt(dyncomm,Parameters)
#' 
#' @param dyncomm a DynCom post processing algorithm. See \code{\link{DynCommPostProcess}}
#' 
#' @param Parameters A two column matrix defining the parameters for this 
#' algorithm. See the PARAMETERS section on this page
#'
#' @return \code{DensOpt} object
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
#'#' @section Methods:
#' \describe{
#' 
DensOpt <- function(dyncomm, Parameters=NULL)
{
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  
  prv <- dyncomm  #previous object in the chain
  prm <- NULL  #parameters for this post processing algorithm
  
  ########## constructor #############
  end_time<-0
  start_time <- floor(as.numeric(Sys.time())*1000000000) #nanoseconds
  if(is.null(Parameters)){
    #set default parameters
  }
  else{
    # TODO validate parameters
    assign("prm",Parameters,thisEnv)
  }
  if(){
    #failed to process.
    print("TODO... Processing failed.")
    end_time <- floor(as.numeric(Sys.time())*1000000000) #nanoseconds
    return(NULL)
  }
  else{
    # process further
    end_time <- floor(as.numeric(Sys.time())*1000000000) #nanoseconds
    if(){
      # failed last step of processing
      return(NULL)
    }
  }
  
  ## Create the list used to represent an object for this class
  me <- list(
    
    ## Define the environment where this list is defined so
    ## that I can refer to it later.
    thisEnv = thisEnv,
    
    has = function(apiFunction){
      if(apiFunction==APIFUNCTIONS$COMMUNITIES){
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
      # else if(apiFunction==APIFUNCTIONS$EDGEWEIGHT){
      #   return(TRUE)
      # }
      # else if(apiFunction==APIFUNCTIONS$NEIGHBOURS){
      #   return(TRUE)
      # }
      else if(apiFunction==APIFUNCTIONS$QUALITY){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$RESULTS){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$TIME){
        return(TRUE)
      }
      # else if(apiFunction==APIFUNCTIONS$VERTEXCOUNT){
      #   return(TRUE)
      # }
      # else if(apiFunction==APIFUNCTIONS$VERTICESALL){
      #   return(TRUE)
      # }
      else if(apiFunction==APIFUNCTIONS$VERTICES){
        return(TRUE)
      }
      else{#function not implemented
        return(FALSE)
      }
    },
    
    #get additional results. Returns 2 column matrix with name of result and value
    results = function(){#postProcessing,ID=1){
      return(matrix(data=c("old modularity",modOld,"new modularity",mod,"old mean", meanOld,"new mean", mean),ncol=2,byrow = TRUE))
    },

    quality=function(){#postProcessing,ID=1){
      return(mod)
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \link{communityCount}}
    #'   
    communityCount=function(){#postProcessing,ID=1){
      return(length(unique(commsNew[[2]])))
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \link{communities}}
    #'   
    communities=function(){#postProcessing,ID=1){
      return(unique(commsNew[[2]]))
    },
    
    
    #' 
    #'   \item{communityNeighbours(community)}{Get the neighbours of the given community after the last iteration. See \link{communityNeighbours}}
    #'   
    communityNeighbours=function(community){#,postProcessing,ID=1){
      return(edgcc[edgcc[,1]==community & edgcc[,2]!=community,2:3])
    },
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \link{communityInnerEdgesWeight}}
    #'   
    communityInnerEdgesWeight=function(community){#,postProcessing,ID=1){
      return(edgcc[edgcc[,1]==community & edgcc[,2]==community,3])
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \link{communityTotalWeight}}
    #'   
    communityTotalWeight=function(community){#,postProcessing,ID=1){
      return(sum(edgcc[edgcc[,1]==community,3]))
    },
    
    
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \link{communityEdgeWeight}}
    #'   
    communityEdgeWeight=function(source,destination){#,postProcessing,ID=1){
      return(edgcc[edgcc[,1]==source & edgcc[,2]==destination,3])
    },
    
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \link{communityVertexCount}}
    #'   
    communityVertexCount=function(community){#,postProcessing,ID=1){
      return(length(commsNew[commsNew[,2]==community,1]))
    },
    
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \link{community}}
    #'   
    community=function(vertex){#,postProcessing,ID=1){
      return(commsNew[commsNew[,1]==vertex,2])
    },
    
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \link{vertexCount}}
    #'   
    # vertexCount=function(postProcessing,ID=1){
    #   return(NA)
    # },
    
    #' 
    #'   \item{verticesAll()}{Get all vertices in the graph after the last iteration. See \link{verticesAll}}
    #'   
    # verticesAll=function(postProcessing,ID=1){
    #   return(NA)
    # },
    
    #' 
    #'   \item{neighbours(vertex)}{Get the neighbours of the given vertex after the last iteration. See \link{neighbours}}
    #'   
    # neighbours=function(vertex){
    #   return(list())
    # },
    
    #' 
    #'   \item{edgeWeight(source,destination)}{Get the weight of the edge that goes from source vertex to destination vertex after the last iteration. See \link{edgeWeight}}
    #'   
    # edgeWeight=function(source,destination){
    #   return(NA)
    # },
    
    #' 
    #'   \item{vertices(community)}{Get all vertices belonging to the given community after the last iteration. See \link{vertices}}
    #'   
    vertices=function(community){#,postProcessing,ID=1){
      return(commsNew[commsNew[,2]==community,1])
    },
    
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \link{communityMapping}}
    #'   
    communityMapping = function(differential=TRUE){#postProcessing,ID=1){
      return(commsNew)
    },
    
    #' 
    #'   \item{time()}{Get the cumulative time spent on processing after the last iteration. See \link{time}}
    #'   
    time=function(differential=FALSE){#PostProcessing, ID){
      return((end_time-start_time)+prv$time(differential))
    }
  )
  # close methods section of the documentation
  #' 
  #' }
  #' 
  
  ## Define the value of the list within the current environment.
  assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"DensOpt")
  return(me)
}
