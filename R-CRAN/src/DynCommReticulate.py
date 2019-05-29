import 

class DynCommReticulate:
  
  def results(bool differential):
    
    return Matrix #returns a matrix with 2 columns (name and value)
    
  def addRemoveEdgesFile(str graphAddRemoveFile):
  
    return Bool
    
  def addRemoveEdgesMatrix(graphAddRemoveMatrix):#input matrix/array
    
    return Bool
    
  def quality(): #metric for the grouping of vertices (ex: distance, modularity...etc)
  
    return double
    
  def communityCount():

    return integer
    
  def communities():
    
    return commvector 
    
  def communityNeighbours(int community):
    
    return Matrix #returns a matrix with (neighbor, weight)
  
  def communityInnerEdgesWeight(int community):
    
    return double

  def communityTotalWeight(int community):
    
    return double
    
  def communityEdgeWeight(int source, int destination):
    
    return double
    
  def communityVertexCount():
    
    return integer
    
  def community(int vertex):
    
    return integer

  def vertexCount():
    
    return integer
    
  def verticesAll():
    
    return vector #vector with all vertices 
  
  def vertices(community):
    
    return array #vector with commmunity vertices
    
  def communityMappingMatrix(bool differential):
    
    return Matrix #returns a matrix (vertex, community)
  
  def communityMappingFile(bool communityFirst, bool differential, str file):
    
    #write the communityMappingMatrix to file (vertex, community)
    
    return Matrix #returns a matrix, one line, one column with TRUE or FALSE (if succeded or not to write to file) 
  
  def neighbors(int vertex):
    
    return Matrix #returns matrix (neighbor, weight)
  
  def edgeCount():
    
    return integer
  
  
  def edgeWeight(int source, int destination):
    
    return double
  
  def time():
    
    return integer #time in nanoseconds
