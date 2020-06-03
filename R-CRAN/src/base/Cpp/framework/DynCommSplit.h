/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines the base class for DynComm. It implements the base
 * of the user interface dispatching function calls accordingly.
 *
 * There should never be any reason to change it unless the API or user
 * interface changes.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-02-02
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_DYNCOMMSPLIT_H_
#define SRC_DYNCOMMSPLIT_H_

#include "defines.h"
#include "DynCommBaseInterface.h"
#include "algorithm/algorithm.h"
#include "criterion/criterion.h"
#include "time/timeFunctions.h"
#include "graph/mapReversable.h"

/**
 * @brief Dynamic Communities base class.
 *
 * @details
 *
 * Simple class that only converts between data types and dispatches function
 * calls to the correct object (graph, criterion or algorithm).
 *
 * @author poltergeist0
 *
 * @date 2019-02-02
 */
class DynCommSplit:private DynCommBaseInterface{
private:
	Algorithm algrthm;//algorithm
	GraphUndirectedGroupable grph;//graph with edges
	Criterion qlt;//criterion
	ProgramParameters prmtrs;//algorithm parameters

	/**
	 * total accumulated time used for processing
	 */
	// uint64 timeTotal=0;

	/**
	 * auxiliary time used to store the start time
	 */
	// uint64 timeStart=0;

	/**
	 * total time used for processing
	 */
	// uint64 timeProcessing=0;

	/*
	 * map used to keep the old relation between the graph vertices and their communities
	 * It is used to keep track of vertices that change communities between iterations
	 * of the algorithm to reply to differential=true communityMapping calls.
	 */
	typeCommunityList oldCommunities;

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the chosen algorithm and the graph
	 */
	DynCommSplit()=delete;

	/**
	 * Constructor
	 */
	DynCommSplit(
			 ALGORITHM algorithm=ALGORITHM::LOUVAIN
			,const Criterion::CRITERION & quality=Criterion::CRITERION::MODULARITY
			, ProgramParameters & algorithmParameters=argumentsDefault
	)
	:
		algrthm(grph,qlt,algorithm,algorithmParameters)
	,qlt(grph,quality,algorithmParameters)
	,prmtrs(algorithmParameters)
	// ,timeTotal(0)
	// ,timeStart(Time::currentTime())
	// ,timeProcessing(0)
	{
	}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @return true if adding/removing succeeded
	 */
	bool addRemoveEdges(ReaderInterface<Edge> * reader){
		// timeStart=Time::currentTime();
		//store old communities
		oldCommunities=typeCommunityList();//first clean map
		const typeVertexList & vs=grph.getVertices();
		for(typeVertexListIteratorConst it=vs.cbegin();it!=vs.cend();++it){
			const typeVertex & v=*it;
			oldCommunities.add(v,grph.community(v));
		}
		bool b=algrthm.addRemoveEdges(reader);
		// uint64 tm=Time::currentTime();
		// timeProcessing=tm-timeStart;
		// timeTotal+=timeProcessing;
		return b;
	}

	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	typeCriterion quality()const {
		return qlt.quality();
	}

	/**
	 * @return the number of existing communities
	 */
	int communityCount()const {
		return grph.communityCount();
	}

	/**
	 * @return the list of all existing communities
	 */
	typeCommunities communities()const {
		return grph.communities();
	}

	/**
	 * Get the number of community to community edges in the graph
	 *
	 * @return the number of edges
	 */
	typeWeight communitiesEdgeCount()const {
		return grph.communitiesEdgeCount();
	}

	/**
	 * Get the communities that are neighbours of the given community
	 *
	 * @param community
	 * @return the neighbouring communities
	 */
	typeLinksRangeConst communityNeighbours(typeCommunity community)const {
		return grph.neighbouringCommunities(community);
	}

	/**
	 * Get the sum of the weights of all edges of the given community where both
	 * vertices of an edge belong to the given community.
	 *
	 * @param community
	 * @return the sum of the weights of all inner edges
	 */
	typeWeight communityInnerEdgesWeight(typeCommunity community)const {return grph.innerEdges(community);}

	//		int communityInnerEdgesCount(int community){return grph.i

	/**
	 * Get the sum of the weights of all edges of the given community.
	 *
	 * @param community
	 * @return the sum of the weights of all edges
	 */
	typeWeight communityTotalWeight(typeCommunity community)const {return grph.totalEdges(community);}

	//		int communityTotalEdgesCount(int community){

	/**
	 * Get the weight of the edge form by two communities
	 *
	 * @param source community
	 * @param destination community
	 * @return the weight of the edge
	 */
	typeWeight communityEdgeWeight(typeCommunity source,typeCommunity destination)const{
		return grph.weightCommunity(source, destination);
	}

	/**
	 * Get the number of vertices in the given community
	 *
	 * @param community
	 * @return the number of vertices
	 */
	int communityVertexCount(typeCommunity community)const {
		unsigned int cnt=0;
		typeCommunityListRange r=grph.vertices(community);
		for(typeCommunityListRangeIteratorConst it=r.first; it!=r.second; ++it){
			++cnt;
		}
		return cnt;
	}

	/**
	 *
	 * @param node
	 * @return the community of the given vertex or a special community of noGRoup if the vertex does not exist
	 */
	typeCommunity community(typeVertex vertex)const{
		return grph.community(vertex);
	}

	/**
	 * @return the number of vertices in the graph
	 */
	unsigned int vertexCount()const{
		return grph.vertexCount();
	}

	/**
	 * @return a constant set with all vertices
	 */
	typeVertexList vertices()const{
		return grph.getVertices();
	}

	/**
	 * @return a list of all vertices belonging to the given community
	 */
	typeVertexList vertices(typeCommunity community)const {
		typeVertexList lst;
		typeCommunityListRange r=grph.vertices(community);
		for(typeCommunityListRangeIteratorConst it=r.first; it!=r.second; ++it){
			typeCommunityListRangePair p=*it;
			lst.insert(p.second);
		}
		return lst;
	}

	/**
	 * Get the number of vertex to vertex edges in the graph
	 *
	 * @return the number of edges
	 */
	typeWeight edgeCount()const {
		return grph.edgeCount();
	}

	/**
	 * Get a snapshot of the current community mapping.
	 * If communityFirst is true the result will be one community per line with
	 * each line being a community followed by a list of vertices. If false, the
	 * result will be a vertex per line with each line being a single vertex and
	 * a single community.
	 * The differential parameter will probably be moved inside the writer as a parameter
	 *
	 * @param writer
	 * @param communityFirst if true returns a community followed by a list of vertices, otherwise a vertex and its community
	 * @param differential return only what changed in the last iteration of the algorithm
	 * @return true if the operation succeeded
	 */
	bool communityMapping(WriterInterface * writer,bool communityFirst=true,bool differential=true) const{
		if(differential){
			if(communityFirst){
				const typeCommunities & gc=grph.communities();
				for(typeCommunities::const_iterator it=gc.cbegin();it!=gc.cend();++it){
					const typeCommunity &c=*it;
					typeVertexList n=vertices(c);
					typeVertexList changed=typeVertexList();//empty set of vertices that changed community
					//check vertices that existed or are new
					for(typeVertexListIteratorConst itn=n.cbegin();itn!=n.cend();++itn){
						const typeVertex & v=*itn;
						const typeCommunityListIteratorConst & itc=oldCommunities.value(v);
						if(itc==oldCommunities.cend()){//vertex did not exist so consider it changed community
							changed.insert(v);
						}
						else{//vertex existed
							const typeCommunity & oc=(*itc).second;
							if(c!=oc){//changed community
								changed.insert(v);
							}
						}
					}
					if(changed.size()>0){
						unsigned int i=1;
						writer->write(std::to_string(c),WriterInterface::WRITETYPE::VALUE);
						for(typeVertexListIteratorConst itn=changed.cbegin();itn!=changed.cend();++itn){
							const typeVertex & v=*itn;
							if(i==changed.size()){//last value
								writer->write(std::to_string(v),WriterInterface::WRITETYPE::LINE);
							}
							else{
								writer->write(std::to_string(v),WriterInterface::WRITETYPE::VALUE);
							}
							++i;
						}
					}
					//check vertices that no longer exist
					changed.clear();
					if(grph.vertexCount()!=oldCommunities.size()){//some vertices no longer exist
						const typeVertexList & vs=grph.getVertices();
						for(typeCommunityListIteratorConst itn=oldCommunities.cbegin();itn!=oldCommunities.cend();++itn){
							const typeVertex & v=(*itn).first;
							if(vs.find(v)==vs.end()){//vertex no longer exists so consider it changed community
								changed.insert(v);
							}
						}
					}
					if(changed.size()>0){
						unsigned int i=1;
						writer->write("NA",WriterInterface::WRITETYPE::VALUE);
						for(typeVertexListIteratorConst itn=changed.cbegin();itn!=changed.cend();++itn){
							const typeVertex & v=*itn;
							if(i==changed.size()){//last value
								writer->write(std::to_string(v),WriterInterface::WRITETYPE::LINE);
							}
							else{
								writer->write(std::to_string(v),WriterInterface::WRITETYPE::VALUE);
							}
							++i;
						}
					}
				}
			}
			else{//vertex first
				//check vertices that existed or are new
				const typeVertexList & vs=grph.getVertices();
				for(typeVertexListIteratorConst it=vs.cbegin();it!=vs.cend();++it){
					const typeVertex & v=*it;
					const typeCommunity & c=grph.community(v);
					const typeCommunityListIteratorConst itc=oldCommunities.value(v);
					if(itc==oldCommunities.cend()){//vertex did not exist so consider it changed community
						writer->write(std::to_string(v),WriterInterface::WRITETYPE::VALUE);
						writer->write(std::to_string(c),WriterInterface::WRITETYPE::LINE);
					}
					else{//vertex existed
						const typeCommunity & oc=(*itc).second;
						if(c!=oc){//changed community
							writer->write(std::to_string(v),WriterInterface::WRITETYPE::VALUE);
							writer->write(std::to_string(c),WriterInterface::WRITETYPE::LINE);
						}
					}
				}
				//check vertices that no longer exist
				if(vs.size()!=oldCommunities.size()){
					for(typeCommunityListIteratorConst itn=oldCommunities.cbegin();itn!=oldCommunities.cend();++itn){
						const typeVertex & v=(*itn).first;
						if(vs.find(v)==vs.end()){//vertex no longer exists so consider it changed community
							writer->write(std::to_string(v),WriterInterface::WRITETYPE::VALUE);
							writer->write("NA",WriterInterface::WRITETYPE::LINE);
						}
					}
				}
			}
		}
		else{//not differential
			if(communityFirst){
				const typeCommunities & gc=grph.communities();
				for(typeCommunities::const_iterator it=gc.cbegin();it!=gc.cend();++it){
					const typeCommunity &c=*it;
					writer->write(std::to_string(c),WriterInterface::WRITETYPE::VALUE);
					typeVertexList n=vertices(c);
					unsigned int i=1;
					for(typeVertexListIteratorConst itn=n.cbegin();itn!=n.cend();++itn){
						if(i==n.size()){//last value
							writer->write(std::to_string(*itn),WriterInterface::WRITETYPE::LINE);
						}
						else{
							writer->write(std::to_string(*itn),WriterInterface::WRITETYPE::VALUE);
						}
						++i;
					}
				}
			}
			else{//vertex first
				const typeVertexList & vs=grph.getVertices();
				for(typeVertexListIteratorConst it=vs.cbegin();it!=vs.cend();++it){
					const typeVertex & v=*it;
					writer->write(std::to_string(v),WriterInterface::WRITETYPE::VALUE);
					writer->write(std::to_string(grph.community(v)),WriterInterface::WRITETYPE::LINE);
				}
			}
		}
		return true;
	}

	/**
	 * @brief Get the neighbours of a vertex
	 * @details The pair can change between calls if the Indexed edge list is modified.
	 * @param vertex
	 * @return pointers to the first and last neighbour of the vertex
	 */
	typeLinksRangeConst neighbours(typeVertex vertex)const {
		return grph.neighbours(vertex);
	}

	/**
	 * @return the weight of the edge
	 */
	typeWeight weight(const typeVertex & source, const typeVertex & destination) const {
		return grph.weight(source,destination);
	}

	/**
	 *
	 * @return the total processing time in nanoseconds
	 */
	// uint64 time(bool accumulated=true)const{
	//   if(!accumulated) return timeProcessing;
	//   return timeTotal;
	// }

};

#endif /* SRC_DYNCOMMSPLIT_H_ */
