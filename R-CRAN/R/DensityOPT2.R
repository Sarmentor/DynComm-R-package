########################### Developer Notice ###########################
# Description:
# This file holds the density optimization post processing algorithm.
#
# Author: 
#   poltergeist0: Algorithm convertion to API
#   Rui Sarmento: Algorithm concept and implementation
#
# Date: 2019-01-01


library(igraph)

#example
# g <-erdos.renyi.game(135, 5/135, directed = TRUE)
# summary(g)
# res.comm <- walktrap.community(g)
# res.list <- densopt(graph=g, graph.directed = TRUE, comms = res.comm, type.names= "num")
# write.table(res.list$new.comms)


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
#' @section Methods:
#' \describe{
#' 
DensOpt <- function(dyncomm, Parameters=NULL)
{
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  densoptInternal <- function(){
    
    Density.df.results <- data.frame("meanOld"=c(),"mean"=c(),"modOld"=c(),"mod"=c(),stringsAsFactors = FALSE)
    n.comms <- max(igraph::membership(comms))
    new.n.comms <- n.comms
    
    if(names=="num"){
      #numeric node names
      df.comms <- data.frame(node = 1:length(igraph::membership(comms)), comm = as.numeric(igraph::membership(comms)))
    }else if(names=="alfa"){
      #alfanumeric node names
      df.comms <- data.frame(node = 1:length(igraph::membership(comms)), comm = as.numeric(igraph::membership(comms)))
    }
    
    #
    df.comms.old <- df.comms
    
    original.comm.density <- c()
    optimized.comm.density <- c()
    
    for (current.comm in 1:n.comms){
      
      comm <- which(igraph::membership(comms) == current.comm)
      subgraph <- igraph::induced.subgraph(graph = mygraph, v = comm, impl = 'copy_and_delete')
      comm.density <- igraph::graph.density(subgraph, loops = FALSE)
      original.comm.density <- c(original.comm.density, comm.density)
      ncompcomm <- igraph::clusters(subgraph, mode=c("strong"))
      
      if(ncompcomm$no > 1){
        
        densities <- c()
        compnodeslist <- list()
        
        for (comp in 1:ncompcomm$no){
          
          compnodes <- which(igraph::membership(ncompcomm) == comp)
          #browser()
          compnodeslist <- c(compnodeslist, list(comm[compnodes]))
          subsubgraph <- igraph::induced.subgraph(graph = subgraph, vids = compnodes, impl = "copy_and_delete")
          if(igraph::gorder(subsubgraph)==1){
            compdensity <- 0
          }else{
            compdensity <- igraph::graph.density(subsubgraph, loops=FALSE) 
          }
          densities <- c(densities, compdensity)
          
        }
        
        if (mean(densities, na.rm = TRUE) > comm.density){
          #create new communities
          new.comms <- new.n.comms + rep(1:ncompcomm$no)
          
          n <<- 0
          
          lapply(compnodeslist, FUN = function(x){
            n <<- n + 1 
            # cat("New community for a found component!!! \n\n")
            sapply(x,FUN=function(y){
              df.comms[as.numeric(df.comms$node)==y,2] <<- new.comms[n]
              cat("Changing node ", y, "community label from ", igraph::membership(comms)[y] , " to ", new.comms[n], ".\n")
            })
          })
          
          new.n.comms <- new.comms[length(new.comms)]
          optimized.comm.density <- c(optimized.comm.density, densities)
        }
      }else{
        optimized.comm.density <- c(optimized.comm.density, comm.density)
      }
    }
    
    #browser()
    assign("meanOld",mean(original.comm.density, na.rm = TRUE),thisEnv)
    assign("mean",mean(optimized.comm.density, na.rm = TRUE),thisEnv)
    assign("modOld",igraph::modularity(mygraph, df.comms.old[,2]),thisEnv)
    assign("mod",igraph::modularity(mygraph, df.comms[,2]),thisEnv)
    assign("commsNew",df.comms,thisEnv)
    
    return(TRUE)
  }
  
  communityCommunityMapping<-function(){
    #unique communities
    uni<-unique(commsNew[,2])
    len<-length(uni)
    #count neighboring communities of communities
    cntc<-matrix(rep(0,len*len),nrow=len,ncol=len)
    for (cnt in 1:nrow(edg)) {
      src<-edg[cnt,1]
      dst<-edg[cnt,2]
      srcc<-commsNew[commsNew[,1]==src,2]
      dstc<-commsNew[commsNew[,1]==dst,2]
      cntc[which(uni[]==srcc)[1],which(uni[]==dstc)[1]]<-1
    }
    #determine total size of matrix for community to community edges
    t<-sum(rowSums(cntc[,1:len]))
    #free cntc
    cntc<-NULL
    #create community to community edge matrix with weights
    ec<-matrix(c(0),nrow=t,ncol=3,byrow=TRUE);
    i<-1 #matrix line insert position
    for (cnt in 1:nrow(edg)) {
      src<-edg[cnt,1]
      dst<-edg[cnt,2]
      wei<-edg[cnt,3]
      srcc<-commsNew[commsNew[,1]==src,2]
      dstc<-commsNew[commsNew[,1]==dst,2]
      #get line with entry
      c<-which(apply(ec, 1, function(x) identical(x[1:2], c(srcc,dstc))))
      if(length(c)==0){#still no entry
        ec[i,1]<-srcc
        ec[i,2]<-dstc
        ec[i,3]<-wei
        i<-i+1
      }
      else{#entry exists
        ec[c,3]<-ec[c,3]+wei #increment weight
      }
    }
    #store community maping
    assign("edgcc",ec,thisEnv)
    #replace commsNew igraph vertex indexes by real vertex names
    for (cnt in 1:length(ver)) {
      commsNew[cnt,1]<-ver[commsNew[cnt,1]]
    }
    #assign back to environment
    assign("commsNew",commsNew,thisEnv)
    #clean unnecessary variables
    edg<-NULL
    
    return(TRUE)
  }
  
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
  #inputs to function
  directed = TRUE
  names= "num"  # c("num","alfa")
  mygraph <- igraph::make_empty_graph(prv$vertexCount(), directed)
  edg <- c()
  we<-c()
  cmm <- c()
  ver<-prv$verticesAll()#translation table (vector) from vertex name to igraph vertex IDs because they need to be continuous and sequential starting from zero
  for (n in ver) {
    nn<- prv$neighbours(n)
    for (nei in nn[,1]) {
      edg <- c(edg, which(ver==n),which(ver==nei),use.names =FALSE)
      we <- c(we,nn[nn[,1]==nei,2],use.names =FALSE)
    }
    cmm <- c(cmm, prv$community(n))
  }
  mygraph <- igraph::add_edges(mygraph,edg)
  igraph::E(mygraph)$weight=as.numeric(we)
  comms <- igraph::make_clusters(mygraph, membership = igraph::as_membership(cmm), algorithm = "DynComm", merges = NULL, modularity = TRUE)
  edg<-matrix(edg,ncol=2,byrow=TRUE,dimnames = NULL)
  edg<-cbind(edg,we,deparse.level = 0)
  we<-NULL
  #outputs from function
  meanOld <- NULL
  mean <- NULL
  modOld <- NULL
  mod <- NULL
  commsNew<-NULL #node community mapping
  edgcc<-NULL #community to community edges (optimization speed over memory)
  #process
  b<-densoptInternal()
  if(!b){
    #failed to process.
    print("TODO... Processing failed.")
    end_time <- floor(as.numeric(Sys.time())*1000000000) #nanoseconds
    return(NULL)
  }
  else{
    b<-communityCommunityMapping()
    end_time <- floor(as.numeric(Sys.time())*1000000000) #nanoseconds
    if(!b){
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
