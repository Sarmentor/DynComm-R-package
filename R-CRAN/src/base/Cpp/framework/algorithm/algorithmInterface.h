/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Interface for main algorithms implemented in C++11.
 *
 * New algorithms must implement the functions defined in the class declared in
 * this file. They should also extend the class "AlgorithmBase" which stores
 * references to the graph, quality and program parameters objects.
 *
 * Copies of those objects must not be executed because the actual adding and
 * removing from the graph is performed outside the control of the algorithm
 * object the developer is implementing. For that same reason, the algorithm
 * being implemented should not change the graph unless required.
 *
 * Due to optimization, and to relief the programmer of new algorithms from
 * having to duplicate code, the functions used to add and remove edges are
 * split into two: the addRemoveEdgePre and addRemoveEdgePost which are used,
 * respectively, to process any data required before and after the edge is
 * added/removed from the graph.
 *
 * The arguments passed to both functions are the same, not just identical.
 *
 * Actually adding/removing the edge from the graph is performed automatically
 * between the call to addRemoveEdgePre and addRemoveEdgePost so the programmer
 * does not have to be concerned with it.
 *
 * The call to function run also occurs automatically but, this time, after the
 * last call to addRemoveEdgePost.
 *
 * See the implementation of the addRemoveEdges function of class "Algorithm"
 * for details of when functions are called.
 *
 * Because all algorithms have an object of its class instantiated, even if it
 * is not used and is never called, new algorithms should have a minimal
 * memory footprint. This is implemented like so to improve performance by
 * getting rid of object pointers and, consequently, of indirection and pointer
 * resolution which would significantly slow down the program. Otherwise, every
 * cycle in the program would require many function calls from object pointers.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_ALGORITHMINTERFACE_H_
#define SRC_ALGORITHMINTERFACE_H_

/**
 * @brief Algorithms API.
 *
 * @details
 *
 * All algorithms must implement the functions defined in this class.
 *
 * Also, at the end of this class, there are two comments with the skeletons
 * of two constructors that must be defined in classes that implement this
 * interface.
 *
 * The first is the deletion of the default constructor which should not exist.
 * Calling a constructor without the graph, quality and program parameters is
 * not permitted.
 *
 * The second constructor is the one that must be called and that receives the
 * graph, quality and program parameters.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 */
class AlgorithmInterface{
protected:
	/**
	 * Function to execute before adding or removing a single edge from the graph.
	 * It is used internally by the addRemoveEdges function of class "Algorithm".
	 * Useful if pre-processing is required, for example, to update some of
	 * variables before the edge can be added or removed.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @param source source vertex
	 * @param destination destination vertex
	 * @param weight optional if adding an edge. Must be exactly zero to remove an edge.
	 * @return true if all operations succeeded. False, otherwise.
	 */
	virtual bool addRemoveEdgePre(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0)=0;

	/**
	 * Function execute after adding or removing a single edge from the graph.
	 * It is used internally by the addRemoveEdges function of class "Algorithm".
	 * Useful if post-processing is required, for example, to update some of
	 * variables after the edge has be added or removed.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @param source source vertex
	 * @param destination destination vertex
	 * @param weight optional if adding an edge. Must be exactly zero to remove an edge.
	 * @return true if all operations succeeded. False, otherwise.
	 */
	virtual bool addRemoveEdgePost(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0)=0;

	/**
	 * Function where the actual algorithm is implemented
	 * It is called at the end of the addRemoveEdges function of class "Algorithm".
	 * @return true if all operations succeeded. False, otherwise.
	 */
	virtual bool run()=0;

public:
	/**
	 * Destructor
	 */
	virtual ~AlgorithmInterface(){}

	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the graph, quality and parameters.
	 * Classes implementing this interface should delete the default constructor.
	 */
//	AlgorithmInterface()=delete;

	/**
	 * Constructor.
	 *
	 * Must be implemented with the skeleton provided here, receiving the same
	 * arguments without copying.
	 *
	 * @param graph reference to the graph object
	 * @param quality reference to the quality object
	 * @param parameters reference to the parameters object
	 */
//	AlgorithmInterface(
//			GraphUndirectedGroupable & graph
//			, const Quality & quality
//			, const ProgramParameters & parameters=argumentsDefault
//	):
//		AlgorithmBase(graph,quality,parameters)
//	{
//	}

};

#endif /* SRC_ALGORITHMINTERFACE_H_ */
