/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
  @
  @      graph.hpp
  @	
  @      @AUTHOR:Kevin Zeng
  @      Copyright 2012 – 2013 
  @      Virginia Polytechnic Institute and State University
  @
  @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/

#ifndef GRAPH_GUARD
#define GRAPH_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include "vertex.hpp"
#include <assert.h>

class Graph
{
	private:
		//Vertex ID, Vertex
		std::map<int, Vertex*> m_GraphV; //Main Datastructure for graph

		//Port name, Vertex ID
		std::map<std::string, int> m_Inputs;         //PI of circuit
		std::map<std::string, int> m_Outputs;        //PO of circuit
		std::vector<int> m_Constants;                //Constants of circuit
		
		//function, node
		std::map<unsigned long long, int> m_Luts;         //LUTs of circuits

		//node, name
		std::map<int, std::string> m_NodeName;       //Name of node

		std::string m_Name;                          //Graph Name

	public:	
		//Constructors
		Graph(std::string);
		Graph(const Graph&);
		Graph& operator=(const Graph& copy);
		~Graph();


		//Importers
		bool importGraph(std::string, int);
		bool importPrimitive(std::string, int);
		bool exportGraph(std::string);
		bool exportGraphSDFV2000(std::string, int);
		bool exportGraphSDFV3000(std::string, int);


		//Iterators
		void getIterators(std::map<int, Vertex*>::iterator&, std::map<int,Vertex*>::iterator&);
		std::map<int,Vertex*>::iterator begin();
		std::map<int,Vertex*>::iterator end();


		//Getters
		Vertex* getVertex(int);
		void getNumType(std::map<std::string, std::vector<Vertex*> >&);
		void getInputs(std::vector<int>&);
		void getInputs(std::map<std::string, int>&);
		void getOutputs(std::vector<int>&);
		void getOutputs(std::map<std::string, int>&);
		unsigned int getNumVertex();
		unsigned int getNumInputs();
		unsigned int getNumOutputs();
		int getNumGate(std::string);
		int getLast();
		unsigned int getMaxLevel();
		std::string getName();
		void getLUTs(std::map<unsigned long long, int>&);
		int getNumNets();
		std::string getNodeName(unsigned int);

		//Boolean
		bool hasLUTs();
		bool isOutput(std::string);
		std::string isOutput(int); //returns portname, else returns ""


		//Setters
		void setName(std::string);
		void setLevels();
		void resetLevels();
		void removeCycles();
		void setOutput(std::string, int);


		//Finders
		int findInPort(std::string);
		int findOutPort(std::string);
		std::string findInPortName(int);
		std::string findOutPortName(int);


		//Graph Functions
		void addVertex(Vertex*);
		Vertex* addVertex(int, std::string);
		Vertex* addVertex(int);
		Vertex* addVertex(int, Vertex*);
		void addInput(std::string, int);
		void addOutput(std::string, int);
		void addConstant(int);
		void removeVertex(Vertex*);
		void removeVertex(int);
		unsigned substitute(int, Graph*);  //returns output node
		void subLUT(int, Graph*);
		void renumber(int);
		void renameNodes(std::string);


		//Search 
		void BFSLevel(std::queue<Vertex*, std::list<Vertex*> >, std::map<int,bool>&, int, int);
		void DFSlevel(Vertex*, std::list<int>&, int);
		int DFScycle(Vertex*, std::list<int>&);
		int DFSearchIn(std::list<int>& , std::set<int>&,  Vertex*, std::set<int>& );
		int DFSearchOut(std::list<int>& , Vertex*, std::set<int>& );
		int DFS_FSM_S(std::set<int>& , int,  Vertex*);
		void DFS_FSM_M(std::set<int>&, std::set<Vertex*>&, Vertex*, std::set<Vertex*>&);
		void DFS_FSM_T(std::set<int>&, Vertex*, std::set<Vertex*>&);


		//Print
		void print();
		void printg();
		void printVertex(int);
};

#endif
