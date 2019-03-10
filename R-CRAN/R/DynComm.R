

# List of available algorithms
ALGORITHM <- list(
  # C++ algorithms are listed from 1 to 10000
  LOUVAIN=1L
  ,SHAKEN=2L
  # Python algorithms are listed from 10001 to 20000
  ,TILES=10001L
  ,ETILES=10002L
  )

# List of available quality measures
QUALITY <- list(
  # C++ quality measures are listed from 1 to 10000
  MODULARITY=1L
  ,BALMOD=2L
  # Python quality measures are listed from 10001 to 20000
)

# DynComm<-function(Algorithm,Quality,parameters){
#   if(Algorithm==ALGORITHM$LOUVAIN){
#     new(DynCommR,Algorithm,Quality,parameters)
#   }
#   else{
#     print("Unknown algorithm :(")
#   }
# }

# Dynom <- setClass("Dynom",slots = c(alg="numeric"))

# setMethod("plot", c("Dynom", "missing"), function(x, y, ...) { alg=y })
# setMethod("plot", "Dynom", function(x, ...) { Dynom@alg=x })

# derived from example in https://www.cyclismo.org/tutorial/R/s3Classes.html
DynComm <- function(Algorithm,Quality,parameters)
{
  
  ## Get the environment for this
  ## instance of the function.
  # thisEnv <- environment()
  
  alg <- Algorithm
  qlt <- Quality
  prm <- parameters
  if(alg>=1 & alg<=10000){
    dc <- new(DynCommR,alg,qlt,prm)
    # print(dc)
  }
  else{
    print("Unknown algorithm :(")
  }
  # print(dc)

  ## Create the list used to represent an
  ## object for this class
  me <- list(
    
    ## Define the environment where this list is defined so
    ## that I can refer to it later.
    # thisEnv = thisEnv,
    
    ## Define the accessors for the data fields.
    # getEnv = function()
    # {
    #   return(get("thisEnv",thisEnv))
    # },
    
    getAlgorithm = function()
    {
      # return(alg)
      alg
    },

    addRemoveEdgesFile = function(graphAddRemoveFile){
      if(alg>=1 & alg<=10000){
        dc$addRemoveEdgesFile(graphAddRemoveFile)
      }
      else{
        print("Unknown :(")
      }
    },
    
    quality=function(){
      if(alg>=1 & alg<=10000){
        dc$quality()
      }
      else{
        print("Unknown :(")
      }
    },
    
      communityCount=function(){
        if(alg>=1 & alg<=10000){
          dc$communityCount()
        }
        else{
          print("Unknown :(")
        }
      },
    
      communities=function(){
        if(alg>=1 & alg<=10000){
          dc$communities()
        }
        else{
          print("Unknown :(")
        }
      },
    
    
    communityInnerEdgesWeight=function(community){
      if(alg>=1 & alg<=10000){
        dc$communityInnerEdgesWeight(community)
      }
      else{
        print("Unknown :(")
      }
    },
    
      communityTotalWeight=function(community){
        if(alg>=1 & alg<=10000){
          dc$communityTotalWeight(community)
        }
        else{
          print("Unknown :(")
        }
      },
      
        
        communityEdgeWeight=function(source,destination){
          if(alg>=1 & alg<=10000){
            dc$communityEdgeWeight(source,destination)
          }
          else{
            print("Unknown :(")
          }
        },
        
        communityNodeCount=function(community){
          if(alg>=1 & alg<=10000){
            dc$communityNodeCount(community)
          }
          else{
            print("Unknown :(")
          }
        },
        
        community=function(node){
          if(alg>=1 & alg<=10000){
            dc$community(node)
          }
          else{
            print("Unknown :(")
          }
        },
        
        nodesCount=function(){
          if(alg>=1 & alg<=10000){
            dc$nodesCount()
          }
          else{
            print("Unknown :(")
          }
        },
        
        nodesAll=function(){
          if(alg>=1 & alg<=10000){
            dc$nodesAll()
          }
          else{
            print("Unknown :(")
          }
        },
        
          nodes=function(community){
            if(alg>=1 & alg<=10000){
              dc$nodes(community)
            }
            else{
              print("Unknown :(")
            }
          },
        
    communityMapping = function(){
      if(alg>=1 & alg<=10000){
        dc$communityMapping(TRUE)
      }
      else{
        print("Unknown :(")
      }
    },
    
        time=function(){
          if(alg>=1 & alg<=10000){
            dc$time()
          }
          else{
            print("Unknown :(")
          }
        }
      
    
  )
  
  ## Define the value of the list within the current environment.
  # assign('this',me,envir=thisEnv)
  
  ## Set the name for the class
  class(me) <- append(class(me),"DynComm")
  return(me)
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
      