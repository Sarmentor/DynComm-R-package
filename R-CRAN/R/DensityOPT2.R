library(igraph)


#' The implementation of eTILES algorithm.
#'
#' @param graph input edge list graph
#' @param graph.directed is graph directed or not
#' @param comms community node assignment for each node in graph
#' @param type.names are node names numeric or alfanumeric 
#' @return dataframe with nodes and new assigned community
#' @seealso \code{\link{nchar}} which this function wraps
#' @export
#' @examples
#' str_length(letters)
densopt <- function(graph, graph.directed = TRUE, comms = NULL, type.names= c("num","alfa")){
  
  mygraph <- read.graph(graph, format = "ncol", directed = graph.directed)
  
  n.comms <- max(membership(comms))
  new.n.comms <- n.comms
  
  if(type.names=="num"){
    #numeric node names
    df.comms <<- data.frame(node = as.numeric(names(membership(comms))), comm = as.numeric(membership(comms)))
  }else if(type.names=="alfa"){
    #alfanumeric node names
    df.comms <<- data.frame(node = names(membership(comms)), comm = as.numeric(membership(comms)))
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
  cat("\nNew Modularity: ", mod.new)
  
  Density.df.results <<- rbind(Density.df.results, c(graph.n,ori.mean,comm.mean, ori.mod, mod.new))
  names(Density.df.results) <- c("graph.n","ori.mean","comm.mean", "ori.mod", "mod.new")
  return(Density.df.results)
}



