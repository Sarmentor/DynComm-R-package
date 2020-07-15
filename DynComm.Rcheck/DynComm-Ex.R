pkgname <- "DynComm"
source(file.path(R.home("share"), "R", "examples-header.R"))
options(warn = 1)
base::assign(".ExTimings", "DynComm-Ex.timings", pos = 'CheckExEnv')
base::cat("name\tuser\tsystem\telapsed\n", file=base::get(".ExTimings", pos = 'CheckExEnv'))
base::assign(".format_ptime",
function(x) {
  if(!is.na(x[4L])) x[1L] <- x[1L] + x[4L]
  if(!is.na(x[5L])) x[2L] <- x[2L] + x[5L]
  options(OutDec = '.')
  format(x[1L:3L], digits = 7L)
},
pos = 'CheckExEnv')

### * </HEADER>
library('DynComm')

base::assign(".oldSearch", base::search(), pos = 'CheckExEnv')
base::assign(".old_wd", base::getwd(), pos = 'CheckExEnv')
cleanEx()
nameEx("ALGORITHM")
### * ALGORITHM

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: ALGORITHM
### Title: List of available algorithms.
### Aliases: ALGORITHM algorithm Algorithm
### Keywords: datasets

### ** Examples

ALGORITHM$LOUVAIN





base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("ALGORITHM", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("CRITERION")
### * CRITERION

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: CRITERION
### Title: List of available CRITERION (quality measurement functions).
### Aliases: CRITERION criterion Criterion
### Keywords: datasets

### ** Examples

CRITERION$MODULARITY




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("CRITERION", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("DynComm")
### * DynComm

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: DynComm
### Title: DynComm
### Aliases: DynComm

### ** Examples

Parameters<-matrix(c("e","0.1","w", "FALSE"),ncol=2, byrow=TRUE)
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
dc$addRemoveEdges(
matrix(
c(10,20,10,30,20,30,30,60,40,60,40,50,50,70,60,70)
,ncol=2,byrow=TRUE)
)
## or
## dc$addRemoveEdges("initial_graph.txt")
dc$communityCount()
## You can use the non inline version of the functions
communities.DynComm(dc)
## Several alias have been defined.
## In this case, communityNodeCount is alias of communityVertexCount
dc$communityNodeCount(10)
dc$communityNeighbours(10)
dc$communityInnerEdgesWeight(10)
dc$communityTotalWeight(10)
dc$communityEdgeWeight(10,40)
dc$community(10) ##this parameter is a vertex not a community. Do not confuse them 
dc$vertices(10)
dc$communityMapping(TRUE)
dc$quality()
dc$mytime()
## lets add post processing :)
dc$postProcess(
list(
list(POSTPROCESSING$DENSOPT)
)
)
## the results of the last step of post processing are selected automatically
## densopt post processing algorithm may change the community mapping so...
## check it
dc$communityMapping(TRUE)
## densopt post processing algorithm may change quality so check it
dc$quality()
## time is now the total time of the main algorithm plus the time of every...
## post processing algorithm up to the one selected
dc$mytime()
## get back to main algorithm results to check they haven't changed
dc$select(POSTPROCESSING$NONE)
dc$communityMapping(TRUE)
dc$quality()
dc$mytime()
## add and remove edges. Notice that there is one more column to give...
## weights of zero on the edges to remove. In this case, all other weights...
## are ignored because the graph is set to ignore weights (parameter w is...
## false).
dc$addRemoveEdges(
matrix(
c(30,60,0,40,60,0.23,10,80,2342,80,90,3.1415)
,ncol=3,byrow=TRUE)
)
## since the post processing was not reset, it will be automatically...
## calculated and results switched to the last step. In this case, to the...
## densopt algorithm
dc$communityMapping(TRUE)
dc$quality()
dc$mytime()
## get back to main algorithm results to check them
dc$select(POSTPROCESSING$NONE)
dc$communityMapping(TRUE)
dc$quality()
dc$mytime()
## lets reset/remove post processing
dc$postProcess()





base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("DynComm", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("DynCommMain")
### * DynCommMain

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: DynCommMain
### Title: DynCommMain
### Aliases: DynCommMain
### Keywords: internal

### ** Examples

## No test: 
Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
dc<-DynCommMain(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
dc$addRemoveEdgesFile("initial_graph.txt")
dc$communityCount()
dc$communities()
dc$communityNodeCount(1)
dc$vertices(1)
dc$communityMapping(TRUE)
dc$time()
dc$addRemoveEdgesFile("s0000000000.txt")
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("DynCommMain", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("DynCommMainR")
### * DynCommMainR

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: DynCommMainR
### Title: DynCommMainR
### Aliases: DynCommMainR
### Keywords: internal

### ** Examples

## No test: 
Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
dc<-DynCommMainR(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
dc$addRemoveEdgesFile("initial_graph.txt")
dc$communityCount()
dc$communities()
dc$communityNodeCount(1)
dc$vertices(1)
dc$communityMapping(TRUE)
dc$time()
dc$addRemoveEdgesFile("s0000000000.txt")
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("DynCommMainR", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("DynCommPostProcess")
### * DynCommPostProcess

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: DynCommPostProcess
### Title: DynCommPostProcess(postProcessing, id, previous, Parameters)
### Aliases: DynCommPostProcess
### Keywords: internal

### ** Examples

## No test: 
Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
dc$addRemoveEdgesFile("initial_graph.txt")
dc$communityCount()
dc$communities()
dc$communityNodeCount(1)
dc$vertices(1)
dc$communityMapping(TRUE)
dc$time()
dc$addRemoveEdgesFile("s0000000000.txt")
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("DynCommPostProcess", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("DynCommPostProcessR")
### * DynCommPostProcessR

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: DynCommPostProcessR
### Title: DynCommPostProcessR(postProcessing, previous, Parameters)
### Aliases: DynCommPostProcessR
### Keywords: internal

### ** Examples

## No test: 
Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
dc$addRemoveEdgesFile("initial_graph.txt")
dc$communityCount()
dc$communities()
dc$communityNodeCount(1)
dc$vertices(1)
dc$communityMapping(TRUE)
dc$time()
dc$addRemoveEdgesFile("s0000000000.txt")
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("DynCommPostProcessR", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("POSTPROCESSING")
### * POSTPROCESSING

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: POSTPROCESSING
### Title: List of available post processing algorithms.
### Aliases: POSTPROCESSING
### Keywords: datasets

### ** Examples

POSTPROCESSING$DENSOPT




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("POSTPROCESSING", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("addRemoveEdges")
### * addRemoveEdges

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: addRemoveEdges
### Title: addRemoveEdges
### Aliases: addRemoveEdges addRemoveEdges.DynComm addRemove.DynComm
###   add.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$addRemoveEdges("graphAddRemoveFile.txt")
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
addRemoveEdges(dc,"graphAddRemoveFile.txt")
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("addRemoveEdges", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communities")
### * communities

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communities
### Title: communities
### Aliases: communities communities.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communities()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communities(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communities", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communitiesEdgeCount")
### * communitiesEdgeCount

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communitiesEdgeCount
### Title: communitiesEdgeCount
### Aliases: communitiesEdgeCount communitiesEdgeCount.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communitiesEdgeCount()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communitiesEdgeCount(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communitiesEdgeCount", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("community")
### * community

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: community
### Title: community
### Aliases: community community.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$community(8)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
community(dc,8)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("community", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communityCount")
### * communityCount

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communityCount
### Title: communityCount
### Aliases: communityCount communityCount.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communityCount()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communityCount(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communityCount", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communityEdgeWeight")
### * communityEdgeWeight

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communityEdgeWeight
### Title: communityEdgeWeight
### Aliases: communityEdgeWeight communityEdgeWeight.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communityEdgeWeight(12,42)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communityEdgeWeight(dc,12,42)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communityEdgeWeight", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communityInnerEdgesWeight")
### * communityInnerEdgesWeight

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communityInnerEdgesWeight
### Title: communityInnerEdgesWeight
### Aliases: communityInnerEdgesWeight communityInnerEdgesWeight.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communityInnerEdgesWeight(1)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communityInnerEdgesWeight(dc,1)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communityInnerEdgesWeight", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communityMapping")
### * communityMapping

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communityMapping
### Title: communityMapping
### Aliases: communityMapping communityMapping.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communityMapping()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communityMapping(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communityMapping", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communityNeighbours")
### * communityNeighbours

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communityNeighbours
### Title: communityNeighbours
### Aliases: communityNeighbours communityNeighbours.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communityNeighbours(community)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communityNeighbours(dc,community)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communityNeighbours", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communityTotalWeight")
### * communityTotalWeight

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communityTotalWeight
### Title: communityTotalWeight
### Aliases: communityTotalWeight communityTotalWeight.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communityTotalWeight(1)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communityTotalWeight(dc,1)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communityTotalWeight", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("communityVertexCount")
### * communityVertexCount

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: communityVertexCount
### Title: communityVertexCount
### Aliases: communityVertexCount communityVertexCount.DynComm
###   communityNodeCount.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$communityVertexCount(3)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
communityVertexCount(dc,3)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("communityVertexCount", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("edgeCount")
### * edgeCount

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: edgeCount
### Title: edgeCount
### Aliases: edgeCount edgeCount.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$edgeCount()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
edgeCount(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("edgeCount", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("edgeWeight")
### * edgeWeight

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: edgeWeight
### Title: edgeWeight
### Aliases: edgeWeight edgeWeight.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$edgeWeight(12,42)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
edgeWeight(dc,12,42)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("edgeWeight", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("mytime")
### * mytime

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: mytime
### Title: mytime
### Aliases: mytime mytime.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$mytime()
## 2.3
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
mytime(dc)
## 2.3
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("mytime", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("neighbours")
### * neighbours

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: neighbours
### Title: neighbours
### Aliases: neighbours neighbours.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$neighbours(vertex)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
neighbours(dc,vertex)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("neighbours", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("postProcess")
### * postProcess

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: postProcess
### Title: postProcess
### Aliases: postProcess postProcess.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$postProcess(
  list(
    list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE))
    ,list(POSTPROCESSING$DENSOPT)
    ,list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))
  )
)
# first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
# POSTPROCESSING$DENSOPT uses default parameters
dc$select(POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
dc$select(POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
dc$select(POSTPROCESSING$NONE)  #selects the main algorithm results
dc$select(POSTPROCESSING$DENSOPT)  #selects the results of densopt
dc$postProcess(NULL)  #remove post processing
## or just
## dc$postProcess()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
postProcess(dc,
  list(
    list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE))
    ,list(POSTPROCESSING$DENSOPT)
    ,list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))
  )
)
# first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
# POSTPROCESSING$DENSOPT uses default parameters
select(dc,POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
select(dc,POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
select(dc,POSTPROCESSING$NONE)  #selects the main algorithm results
select(dc,POSTPROCESSING$DENSOPT)  #selects the results of densopt
postProcess(dc,NULL)  #remove post processing
## or just
## postProcess(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("postProcess", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("postProcessDensOpt")
### * postProcessDensOpt

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: postProcessDensOpt
### Title: Density Optimization
### Aliases: postProcessDensOpt
### Keywords: internal

### ** Examples

## No test: 
Parameters<-matrix(c("-e","0.1"),1,2,TRUE)
dc<-DynCommMain(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,Parameters)
dc$addRemoveEdgesFile("initial_graph.txt")
dc$communityCount()
dc$communities()
dc$communityNodeCount(1)
dc$vertices(1)
dc$communityMapping(TRUE)
dc$time()
dc$addRemoveEdgesFile("s0000000000.txt")
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("postProcessDensOpt", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("quality")
### * quality

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: quality
### Title: quality
### Aliases: quality quality.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$quality()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
quality(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("quality", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("results")
### * results

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: results
### Title: results
### Aliases: results results.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$results()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
results(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("results", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("select")
### * select

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: select
### Title: select
### Aliases: select select.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$postProcess(
  list(
    list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE))
    ,list(POSTPROCESSING$DENSOPT)
    ,list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))
  )
)
# first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
# POSTPROCESSING$DENSOPT uses default parameters
dc$select(POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
dc$select(POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
dc$select(POSTPROCESSING$NONE)  #selects the main algorithm results
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
postProcess(dc,
  list(
    list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",10),ncol=2,byrow=TRUE))
    ,list(POSTPROCESSING$DENSOPT)
    ,list(POSTPROCESSING$WEIGHTTOP,matrix(data=c("n",3),ncol=2,byrow=TRUE))
  )
)
# first POSTPROCESSING$WEIGHTTOP gets ID=1 and second gets ID=2
# POSTPROCESSING$DENSOPT uses default parameters
select(dc,POSTPROCESSING$WEIGHTTOP,1)  #selects the results of the first WEIGHTTOP
select(dc,POSTPROCESSING$WEIGHTTOP,2)  #selects the results of the second WEIGHTTOP
select(dc,POSTPROCESSING$NONE)  #selects the main algorithm results
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("select", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("vertexCount")
### * vertexCount

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: vertexCount
### Title: vertexCount
### Aliases: vertexCount vertexCount.DynComm nodesCount.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$vertexCount()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
vertexCount(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("vertexCount", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("vertices")
### * vertices

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: vertices
### Title: vertices
### Aliases: vertices vertices.DynComm nodes.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$vertices(6)
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
vertices(dc,6)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("vertices", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
cleanEx()
nameEx("verticesAll")
### * verticesAll

flush(stderr()); flush(stdout())

base::assign(".ptime", proc.time(), pos = "CheckExEnv")
### Name: verticesAll
### Title: verticesAll
### Aliases: verticesAll verticesAll.DynComm nodesAll.DynComm

### ** Examples

## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
dc$verticesAll()
## End(No test)
## No test: 
dc<-DynComm(ALGORITHM$LOUVAIN,CRITERION$MODULARITY,parameters)
verticesAll(dc)
## End(No test)




base::assign(".dptime", (proc.time() - get(".ptime", pos = "CheckExEnv")), pos = "CheckExEnv")
base::cat("verticesAll", base::get(".format_ptime", pos = 'CheckExEnv')(get(".dptime", pos = "CheckExEnv")), "\n", file=base::get(".ExTimings", pos = 'CheckExEnv'), append=TRUE, sep="\t")
### * <FOOTER>
###
cleanEx()
options(digits = 7L)
base::cat("Time elapsed: ", proc.time() - base::get("ptime", pos = 'CheckExEnv'),"\n")
grDevices::dev.off()
###
### Local variables: ***
### mode: outline-minor ***
### outline-regexp: "\\(> \\)?### [*]+" ***
### End: ***
quit('no')
