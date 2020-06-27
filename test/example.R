library(DynComm)
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
DynComm.communities(dc)
## Several alias have been defined.
## In this case, communityNodeCount is alias of communityVertexCount
dc$communityNodeCount(10)
dc$communityNeighbours(10)
dc$communityInnerEdgesWeight(10)
dc$communityTotalWeight(10)
dc$communityEdgeWeight(10,40)
dc$community(10) ##this parameter is a vertex not a community. Do not confuse them 
dc$vertices(10)
dc$communityMapping(TRUE);dc$quality();dc$time(TRUE);dc$time(FALSE);dc$results()

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
dc$time()
## get back to main algorithm results to check they haven't changed
dc$communityMapping(TRUE);dc$quality();dc$time(TRUE);dc$time(FALSE);dc$results()
dc$select(POSTPROCESSING$NONE)
dc$communityMapping(TRUE);dc$quality();dc$time(TRUE);dc$time(FALSE);dc$results()

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
dc$communityMapping(TRUE);dc$quality();dc$time(TRUE);dc$time(FALSE);dc$results()

## get back to main algorithm results to check them
dc$select(POSTPROCESSING$NONE)
dc$communityMapping(TRUE);dc$quality();dc$time(TRUE);dc$time(FALSE);dc$results()

## lets reset/remove post processing
dc$postProcess()
