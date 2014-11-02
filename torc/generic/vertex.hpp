/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@ 
  @
  @      vertex.hpp
  @      
  @      @AUTHOR:        Kevin Zeng
  @      Copyright 2012 – 2013 
	@      Virginia Polytechnic Institute and State University
  @
  @#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/


#ifndef VERTEX_GUARD
#define VERTEX_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>

class Vertex
{
	private:
		std::string m_Type;
		std::string m_Name;
		int m_ID;    //Unique ID assigned
		int m_Level;
		unsigned long long m_LUT;

		//Set of vertices connected into the current vertex
		std::vector<Vertex*> m_Input;
		std::vector<std::string> m_InPorts;
		std::map<std::string, std::vector<Vertex*> > m_Output;

	public:

		Vertex();
		Vertex(int, std::string);
		Vertex(int, std::string, std::string);
		Vertex(int);
		Vertex(const Vertex& rhs);

		void setLevel(int);
		void setLUT(std::string);
		void setLUT(unsigned long long);
		void setType(std::string);
		void setName(std::string);
		void setVertexID(int);

		unsigned long long getLUT();
		unsigned int getOVSize();
		unsigned int getIVSize();
		int getID();
		int getLevel();
		int getNumInputs();
		int getNumOutputs();
		int getInputPortID(std::string);
		void getInPorts(std::vector<std::string>&);
		void getOutput(std::vector<Vertex*> &);
		void getOutput(std::map<std::string, std::vector<Vertex*> >&);
		void getInput(std::vector<Vertex*> &);
		void getPortOutput(std::string, std::vector<Vertex*>&);
		void getInputPorts(std::vector<std::string>&);
		void getOutputPorts(std::vector<std::string> &);
		void getOutputPortID(std::string, std::vector<Vertex*>&);
		std::string getInputPortName(int);
		std::string getOutputPortName(int);
		std::string getType();
		std::string getName();

		void addInput(Vertex*);
		void addInPort(std::string);
		void addOutput(Vertex*, std::string);

		std::string removeOutputValue(int);
		int removeInputValue(int);
		void removeInPortValue(int);
};

#endif












