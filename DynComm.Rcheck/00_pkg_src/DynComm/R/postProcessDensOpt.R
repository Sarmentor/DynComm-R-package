########################### Developer Notice ###########################
# Description:
# This file holds the density optimization post processing algorithm.
#
# Author: 
#   poltergeist0: Algorithm convertion to API
#   Rui Sarmento: Algorithm concept and implementation
#
# Date: 2019-01-01

# library(igraph) #cannot use library in packages http://r-pkgs.had.co.nz/namespace.html#namespace

#' @name postProcessDensOpt
#' 
#' @keywords internal
#' 
# @aliases Densopt densopt
#' 
#' @title Density Optimization
#'
#' @author poltergeist0
#' 
#' @description 
#' Implementation of the density optimization algorithm as a post processing
#' algorithm.
#' 
#' @details 
#' Is an algorithm that provides a community structure, not explicitly based on 
#' modularity, but based on the increase of the average community density. 
#' Contrary to modularity-based algorithms, it tends to disband large communities 
#' into smaller ones.
#' It is an algorithm currently developed for directed networks only. 
#'
#' @rdname postProcessDensOpt
#' 
#' @docType class
#' 
#' @usage postProcessDensOpt(dyncomm,Parameters)
#' 
#' @param dyncomm a DynCom post processing algorithm. See \code{\link{DynCommPostProcess}}
#' 
#' @param Parameters A two column matrix defining the parameters for this 
#' algorithm. See the PARAMETERS section on this page
#'
#' @return \code{postProcessDensOpt} object
#'
#' @section Performance:
#' \describe{
#'   \item{Initialization}{
#'   Uses a matrix with three columns and a maximum of verticelAll()^2 rows 
#'   with the edges between vertices and their weight (vertex<->vertex<->weight)
#'   of the original graph.
#'   Temporarily stores a copy of the graph to calculate a new community mapping.
#'   }
#'   \item{Results}{
#'   Uses a matrix with two columns and verticesAll() rows with the new community
#'   mapping (vertex<->community).
#'   Uses a matrix with three columns and a maximum of 
#'   communityCount()^2+communityCount() rows with the edges between communities 
#'   and their weight (community<->community<->weight).
#'   }
#' }
#' 
# do not export this object. It is only for internal use of the algorithm
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
postProcessDensOpt <- function(dyncomm, Parameters=NULL)
{
  ## Get the environment for this
  ## instance of the function.
  thisEnv <- environment()
  
  densoptInternal <- function(){
    directed = TRUE
    names= "num"  # c("num","alfa")
    mygraph <- igraph::make_empty_graph(prv$vertexCount(), directed)
    mygraph <- igraph::add_edges(mygraph,edg)
    igraph::E(mygraph)$weight=as.numeric(we)
    comms <- igraph::make_clusters(mygraph, membership = igraph::as_membership(cmm), algorithm = "DynComm", merges = NULL, modularity = TRUE)
    
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
              # cat("Changing node ", y, "community label from ", igraph::membership(comms)[y] , " to ", new.comms[n], ".\n")
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
    # assign("commsNew",df.comms,thisEnv)
    # print(df.comms)
    # print(matrix(data=df.comms,ncol=2,byrow=TRUE,dimnames = list(c(),c("vertex","community"))))
    # print(data.matrix(df.comms))
    commsNew<-data.matrix(df.comms)
    colnames(commsNew) <-c("vertex","community")
    assign("commsNew",commsNew,thisEnv)
    
    return(TRUE)
  }
  
  communityCommunityMapping<-function(){
    edg<-matrix(edg,ncol=2,byrow=TRUE,dimnames = NULL)
    edg<-cbind(edg,we,deparse.level = 0)
    we<-NULL
    #unique communities
    uni<-unique(commsNew[,2])
    len<-length(uni)
    #count neighbouring communities of communities
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
      # c<-which(apply(ec, 1, function(x) identical(x[1:2], c(srcc,dstc))))
      c<-which(apply(ec, 1, function(x){x[1]==srcc && x[2]==dstc}))
      # print(cnt)
      # print(edg[cnt,])
      # print(commsNew[commsNew[,1]==src,])
      # print(commsNew[commsNew[,1]==dst,])
      # print(c)
      if(length(c)==0){#still no entry
        ec[i,1]<-srcc
        ec[i,2]<-dstc
        ec[i,3]<-wei
        i<-i+1
      }
      else{#entry exists
        # print(ec[c,])
        ec[c,3]<-ec[c,3]+wei #increment weight
      }
      # print(ec)
    }
    #store community maping
    assign("edgcc",ec,thisEnv)
    #replace commsNew igraph vertex indexes by real vertex names
    for (cnt in 1:length(ver)) {
      commsNew[cnt,1]<-ver[commsNew[cnt,1]]
    }
    #assign back to environment
    assign("commsNew",commsNew,thisEnv)

    return(TRUE)
  }
  
  prv <- dyncomm  #previous object in the chain
  prm <- NULL  #parameters for this post processing algorithm
  
  ########## constructor #############
  # end_time<-0
  # start_time <- floor(as.numeric(Sys.time())*1000000000) #nanoseconds
  if(is.null(Parameters)){
    #set default parameters
  }
  else{
    # TODO validate parameters
    assign("prm",Parameters,thisEnv)
  }
  # TODO check NULL or empty graph
  #inputs to function
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
  # directed = TRUE
  # names= "num"  # c("num","alfa")
  # mygraph <- igraph::make_empty_graph(prv$vertexCount(), directed)
  # mygraph <- igraph::add_edges(mygraph,edg)
  # igraph::E(mygraph)$weight=as.numeric(we)
  # comms <- igraph::make_clusters(mygraph, membership = igraph::as_membership(cmm), algorithm = "DynComm", merges = NULL, modularity = TRUE)
  # edg<-matrix(edg,ncol=2,byrow=TRUE,dimnames = NULL)
  # edg<-cbind(edg,we,deparse.level = 0)
  # we<-NULL
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
    return(NULL)
  }
  else{
    b<-communityCommunityMapping()
    if(!b){
      return(NULL)
    }
  }
  #clean unnecessary variables after calculations
  edg<-NULL
  # cmm<-NULL
  we<-NULL
  # TODO: optimize commsNew and edgcc away if the community mapping has not changed
  
  ## Create the list used to represent an object for this class
  me <- list(
    
    ## Define the environment where this list is defined so
    ## that I can refer to it later.
    thisEnv = thisEnv,
    
    has = function(apiFunction){
      # print(apiFunction)
      if(apiFunction==APIFUNCTIONS$COMMUNITIES){
        return(TRUE)
      }
      else if(apiFunction==APIFUNCTIONS$COMMUNITIESEDGECOUNT){
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
    
    #' 
    #'   \item{results(differential)}{Get additional results of the algorithm or the currently selected post processing steps. See \code{\link{results}}}
    #'   
    results = function(differential=TRUE){#postProcessing,ID=1){
		if(differential){}#do nothing. Checks differential just to avoid an error about being unused
      return(matrix(data=c("old modularity",modOld,"new modularity",mod,"old mean", meanOld,"new mean", mean),ncol=2,byrow = TRUE))
    },

    #' 
    #'   \item{quality()}{Get the quality measurement of the graph after the last iteration. See \code{\link{quality}}}
    #'   
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
    #'   \item{communitiesEdgeCount()}{Get the number of community to community edges in the graph. See \code{\link{communitiesEdgeCount}}}
    #'   
    communitiesEdgeCount=function(){
      return(nrow(edgcc))
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
    vertices=function(community){
      return(commsNew[commsNew[,2]==community,1])
    },
    
    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \link{communityMapping}}
    #'   
    communityMappingMatrix = function(differential=TRUE){
      if(differential){
        # print(ver)
        # print(cmm)
        a<-matrix(data=c(ver,cmm),ncol=2,byrow = FALSE)
        return(commsNew[which(is.element(commsNew[,1],a[,1])) & commsNew[,2]!=a[,2],])
      }
      return(commsNew)
    },

    #' 
    #'   \item{communityMapping()}{Get the community mapping for all communities after the last iteration.See \link{communityMapping}}
    #'   
    communityMappingFile = function(differential=TRUE,file="communityMapping.txt"){
      if(differential){
        a<-matrix(data=c(ver,cmm),ncol=2,byrow = FALSE)
        a<-commsNew[which(is.element(commsNew[,1],a[,1])) & commsNew[,2]!=a[,2],]
        write.table(a, file=file, row.names=FALSE, col.names=FALSE, sep = "\t")
      }
      else{
        write.table(commsNew, file=file, row.names=FALSE, col.names=FALSE, sep = "\t")
      }
      return(TRUE)
    }
    
  )
  # close methods section of the documentation
  #' 
  #' }
  #' 
  
  ## Define the value of the list within the current environment.
  assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"postProcessDensOpt")
  return(me)
}
