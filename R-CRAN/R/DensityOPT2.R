library(igraph)


densoptOld <- function(graph, graph.directed = TRUE, comms = NULL, type.names= c("num","alfa")){
  
  mygraph <- graph
  Density.df.results <- data.frame("ori.mean"=c(),"comm.mean"=c(),"ori.mod"=c(),"mod.new"=c(),stringsAsFactors = FALSE)
  n.comms <- max(membership(comms))
  new.n.comms <- n.comms
  
  if(type.names=="num"){
    #numeric node names
    df.comms <<- data.frame(node = 1:length(membership(comms)), comm = as.numeric(membership(comms)))
  }else if(type.names=="alfa"){
    #alfanumeric node names
    df.comms <<- data.frame(node = 1:length(membership(comms)), comm = as.numeric(membership(comms)))
  }
  
  #
  df.comms.old <- df.comms
  
  original.comm.density <- c()
  optimized.comm.density <- c()
  
  for (current.comm in 1:n.comms){
    
    comm <- which(membership(comms) == current.comm)
    subgraph <- induced.subgraph(graph = mygraph, v = comm, impl = 'copy_and_delete')
    comm.density <- graph.density(subgraph, loops = FALSE)
    original.comm.density <- c(original.comm.density, comm.density)
    ncompcomm <- clusters(subgraph, mode=c("strong"))
    
    if(ncompcomm$no > 1){
      
      densities <- c()
      compnodeslist <- list()
      
      for (comp in 1:ncompcomm$no){
        
        compnodes <- which(membership(ncompcomm) == comp)
        #browser()
        compnodeslist <- c(compnodeslist, list(comm[compnodes]))
        subsubgraph <- induced.subgraph(graph = subgraph, vids = compnodes, impl = "copy_and_delete")
        if(gorder(subsubgraph)==1){
          compdensity <- 0
        }else{
          compdensity <- graph.density(subsubgraph, loops=FALSE) 
        }
        densities <- c(densities, compdensity)
        
      }
      
      if (mean(densities, na.rm = TRUE) > comm.density){
        #create new communities
        new.comms <- new.n.comms + rep(1:ncompcomm$no)
        
        n <<- 0
        
        lapply(compnodeslist, FUN = function(x){
          n <<- n + 1 
          cat("New community for a found component!!! \n\n")
          sapply(x,FUN=function(y){
            df.comms[as.numeric(df.comms$node)==y,2] <<- new.comms[n]
            cat("Changing node ", y, "community label from ", membership(comms)[y] , " to ", new.comms[n], ".\n")
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
  ori.mean <- mean(original.comm.density, na.rm = TRUE)
  comm.mean <- mean(optimized.comm.density, na.rm = TRUE)
  ori.mod <- modularity(mygraph, df.comms.old[,2])
  mod.new <- modularity(mygraph, df.comms[,2])
  
  cat("\nAverage Original Comm. Density: ", ori.mean)
  cat("\nAverage Optimized Comm. Density: ", comm.mean)
  cat("\nOld Modularity: ", ori.mod)
  cat("\nNew Modularity: ", mod.new,"\n")
  
  Density.df.results <<- rbind(Density.df.results, c(ori.mean,comm.mean, ori.mod, mod.new))
  names(Density.df.results) <<- c("ori.mean","comm.mean","ori.mod","mod.new")
  
  return(list(res=Density.df.results,new.comms=df.comms))
}

#example
g <-erdos.renyi.game(135, 5/135, directed = TRUE)
summary(g)
res.comm <- walktrap.community(g)
res.list <- densopt(graph=g, graph.directed = TRUE, comms = res.comm, type.names= "num")
write.table(res.list$new.comms)



########################### DynComm template ###########################
# This file presents a template of the Dyncomm interface to make it easier
# to create post processing algorithms. The interface is identical except that
# there are no functions to add/remove edges, or to query the graph, and all
# functions have two extra parameters naming the type of object and id.

# Receives an object that mimics the DynComm interface, which can be either
# a DynComm object or another post processing algorithm, and a sequencial id
# number, to distinguish the cases where more than one object of same type is
# used. The id number is incremented on creation only for objects of same type.

# Each post processing object must know, internally, its type so that it can
# either return its results or submit the request to the previous object. The
# DynComm object is at the bottom of the chain so, requests to the DynComm
# object are always answered by it.

#The chain is processed bottom up, meaning that, the DynComm object first, then 
#the first post processing algorithm and so on but, it is answered top bottom,
#meaning that, the last post processing algorithm answers first, then the second
#and so on until it reaches the DynComm object

# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
densopt <- function(DynComm, ID=1, Parameters=NULL)
{
  prv <- DynComm  #previous object in the chain or NULL if this is the first link
  id <- ID  #this object ID number
  pst <- POSTPROCESSING$DENSOPT  #immutable post processing flag indicating this object type
  prm <- NULL  #parameters for this post processing algorithm
  
  ########## constructor #############
  if(is.null(Parameters)){
    #set default parameters
  }
  else{
    # TODO validate parameters
    prm < - Parameters
  }
  # do something if required

  densoptInternal <- function(graph, graph.directed = TRUE, comms = NULL, type.names= c("num","alfa")){
    
    mygraph <- graph
    Density.df.results <- data.frame("ori.mean"=c(),"comm.mean"=c(),"ori.mod"=c(),"mod.new"=c(),stringsAsFactors = FALSE)
    n.comms <- max(membership(comms))
    new.n.comms <- n.comms
    
    if(type.names=="num"){
      #numeric node names
      df.comms <<- data.frame(node = 1:length(membership(comms)), comm = as.numeric(membership(comms)))
    }else if(type.names=="alfa"){
      #alfanumeric node names
      df.comms <<- data.frame(node = 1:length(membership(comms)), comm = as.numeric(membership(comms)))
    }
    
    #
    df.comms.old <- df.comms
    
    original.comm.density <- c()
    optimized.comm.density <- c()
    
    for (current.comm in 1:n.comms){
      
      comm <- which(membership(comms) == current.comm)
      subgraph <- induced.subgraph(graph = mygraph, v = comm, impl = 'copy_and_delete')
      comm.density <- graph.density(subgraph, loops = FALSE)
      original.comm.density <- c(original.comm.density, comm.density)
      ncompcomm <- clusters(subgraph, mode=c("strong"))
      
      if(ncompcomm$no > 1){
        
        densities <- c()
        compnodeslist <- list()
        
        for (comp in 1:ncompcomm$no){
          
          compnodes <- which(membership(ncompcomm) == comp)
          #browser()
          compnodeslist <- c(compnodeslist, list(comm[compnodes]))
          subsubgraph <- induced.subgraph(graph = subgraph, vids = compnodes, impl = "copy_and_delete")
          if(gorder(subsubgraph)==1){
            compdensity <- 0
          }else{
            compdensity <- graph.density(subsubgraph, loops=FALSE) 
          }
          densities <- c(densities, compdensity)
          
        }
        
        if (mean(densities, na.rm = TRUE) > comm.density){
          #create new communities
          new.comms <- new.n.comms + rep(1:ncompcomm$no)
          
          n <<- 0
          
          lapply(compnodeslist, FUN = function(x){
            n <<- n + 1 
            cat("New community for a found component!!! \n\n")
            sapply(x,FUN=function(y){
              df.comms[as.numeric(df.comms$node)==y,2] <<- new.comms[n]
              cat("Changing node ", y, "community label from ", membership(comms)[y] , " to ", new.comms[n], ".\n")
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
    ori.mean <- mean(original.comm.density, na.rm = TRUE)
    comm.mean <- mean(optimized.comm.density, na.rm = TRUE)
    ori.mod <- modularity(mygraph, df.comms.old[,2])
    mod.new <- modularity(mygraph, df.comms[,2])
    
    cat("\nAverage Original Comm. Density: ", ori.mean)
    cat("\nAverage Optimized Comm. Density: ", comm.mean)
    cat("\nOld Modularity: ", ori.mod)
    cat("\nNew Modularity: ", mod.new,"\n")
    
    Density.df.results <<- rbind(Density.df.results, c(ori.mean,comm.mean, ori.mod, mod.new))
    names(Density.df.results) <<- c("ori.mean","comm.mean","ori.mod","mod.new")
    
    return(list(res=Density.df.results,new.comms=df.comms))
  }
  
  ## Create the list used to represent an object for this class
  me <- list(
    
    exists = function(postProcessing,ID=1){
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
          if(is(prv,"DynCommBase")){
            #do not call exists on main algorithm because function does not exist
            return(FALSE)
          }
          else{
            return(prv$exists(postProcessing,ID))
          }
        }
      }
    },
    
    
    #get additional results. Returns 2 column matrix with name of result and value
    results = function(postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$results()))
          }
          else{
            return(prv$results(postProcessing,ID))
          }
        }
      }
    },
    
    
    quality=function(postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(NA)  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$quality()))
          }
          else{
            return(prv$quality(postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityCount()}{Get the number of communities after the last iteration. See \link{communityCount}}
    #'   
    communityCount=function(postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(NA)  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$communityCount()))
          }
          else{
            return(prv$communityCount(postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communities()}{Get all communities after the last iteration. See \link{communities}}
    #'   
    communities=function(postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$communities()))
          }
          else{
            return(prv$communities(postProcessing,ID))
          }
        }
      }
    },
    
    
    #' 
    #'   \item{communityInnerEdgesWeight(community)}{Get the sum of weights of the inner edges of the given community after the last iteration. See \link{communityInnerEdgesWeight}}
    #'   
    communityInnerEdgesWeight=function(community,postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$communityInnerEdgesWeight(community)))
          }
          else{
            return(prv$communityInnerEdgesWeight(community,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityTotalWeight(community)}{Get the sum of weights of all edges of the given community after the last iteration. See \link{communityTotalWeight}}
    #'   
    communityTotalWeight=function(community,postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$communityTotalWeight(community)))
          }
          else{
            return(prv$communityTotalWeight(community,postProcessing,ID))
          }
        }
      }
    },
    
    
    #' 
    #'   \item{communityEdgeWeight(source,destination)}{Get the weight of the edge that goes from source to destination after the last iteration. See \link{communityEdgeWeight}}
    #'   
    communityEdgeWeight=function(source,destination,postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$communityEdgeWeight(source,destination)))
          }
          else{
            return(prv$communityEdgeWeight(source,destination,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{communityVertexCount(community)}{Get the amount of vertices in the given community after the last iteration. See \link{communityVertexCount}}
    #'   
    communityVertexCount=function(community,postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$communityVertexCount(community)))
          }
          else{
            return(prv$communityVertexCount(community,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{community(vertex)}{Get the community of the given vertex after the last iteration. See \link{community}}
    #'   
    community=function(vertex,postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$community(vertex)))
          }
          else{
            return(prv$community(vertex,postProcessing,ID))
          }
        }
      }
    },
    
    #' 
    #'   \item{vertexCount()}{Get the total number of vertices after the last iteration. See \link{vertexCount}}
    #'   
    vertexCount=function(postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$vertexCount()))
          }
          else{
            return(prv$vertexCount(postProcessing,ID))
          }
        }
      }
    },
    #' 
    #'   \item{vertexAll()}{Get all vertices in the graph after the last iteration. See \link{vertexAll}}
    #'   
    vertexAll=function(postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$vertexAll()))
          }
          else{
            return(prv$vertexAll(postProcessing,ID))
          }
        }
      }
    },
    
    vertices=function(community,postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$vertices(community)))
          }
          else{
            return(prv$vertices(community,postProcessing,ID))
          }
        }
      }
    },
    
    communityMapping = function(postProcessing,ID=1){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$communityMapping()))
          }
          else{
            return(prv$communityMapping(postProcessing,ID))
          }
        }
      }
    },
    
    time=function(PostProcessing, ID){
      if(pst==postProcessing && id==ID){
        #return the results of this object
      }
      else{
        #return from the previous object
        if(is.null(prv)){
          print("Internal error. Missing previous post processing object.")
          return(matrix(ncol=2, byrow=TRUE))  #should never get here. There is always a previous
        }
        else{
          if(is(prv,"DynCommBase")){
            return(return(prv$time()))
          }
          else{
            return(prv$time(postProcessing,ID))
          }
        }
      }
    }
  )
  
  ## Set the name for the class
  class(me) <- append(class(me),"densopt")
  return(me)
}
