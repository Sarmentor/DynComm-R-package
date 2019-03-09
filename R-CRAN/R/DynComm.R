

# List of available algorithms
ALGORITHM <- list(
  # C++ algorithms are listed from 1 to 1000
  LOUVAIN=1L
  ,SHAKEN=2L
  # Python algorithms are listed from 1001 to 2000
  ,TILES=1001L
  ,ETILES=1002L
  )

# List of available quality measures
QUALITY <- list(
  # C++ quality measures are listed from 1 to 1000
  MODULARITY=1L
  ,BALMOD=2L
  # Python quality measures are listed from 1001 to 2000
)

DynComm<-function(Algorithm,Quality,parameters){
  if(Algorithm==ALGORITHM$LOUVAIN){
    new(DynCommR,Algorithm,Quality,parameters)
  }
  else{
    print("Unknown algorithm :(")
  }
}

# parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE)
# dc<-DynComm(ALGORITHM$LOUVAIN,QUALITY$MODULARITY,parameters)
# dc$communityCount()
# dc$communities()
# dc$communityNodeCount(1)
# dc$nodes(1)
# dc$communityMapping(TRUE)
# dc$time()
# dc$addRemoveEdgesFile("test/full/sequences/s0000000000.txt")
      