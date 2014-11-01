/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@ 
	@
	@      vertex.cpp
	@      
	@      @AUTHOR:        Kevin Zeng
	@      Copyright 2012 – 2013 
	@      Virginia Polytechnic Institute and State University
	@
	@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/


#include "vertex.hpp"

Vertex::Vertex(){
	m_Input.clear();
	m_InPorts.clear();
	m_Output.clear();
	m_Name = "";
	m_Level = -1;
}

Vertex::Vertex(int vID, std::string type){
	m_Type = type;		
	m_ID = vID;
	m_Input.clear();
	m_InPorts.clear();
	m_Output.clear();
	m_Level = -1;
	m_Name = "";
}

Vertex::Vertex(int vID, std::string type, std::string name){
	m_Type = type;		
	m_ID = vID;
	m_Input.clear();
	m_InPorts.clear();
	m_Output.clear();
	m_Level = -1;
	m_Name = name;
}

Vertex::Vertex(int vID){
	m_Type = "";		
	m_ID = vID;
	m_Input.clear();
	m_InPorts.clear();
	m_Output.clear();
	m_Level = -1;
	m_Name = "";
}


Vertex::Vertex(const Vertex& rhs){
	//TODO
	printf("VERTEX CCONSTRUCTOR\n");
	exit(1);
	m_Type = rhs.m_Type;
	m_ID = rhs.m_ID;
	m_Name= rhs.m_Name;
/*
	for(unsigned int i = 0; i < rhs.m_Input.size(); i++)
		m_Input.push_back(rhs.m_Input[i]);
	for(unsigned int i = 0; i < rhs.m_Output.size(); i++)
		m_Output.push_back(rhs.m_Output[i]);
		*/
}

void Vertex::setLevel(int level){
	m_Level = level;
}

void Vertex::setLUT(std::string func){
	m_LUT = 0;
	unsigned long base = 1;
	//Convert function string into an integer
	for(int i = func.size()-1; i >= 0; i--){
		if( func[i] <= '9'){
			m_LUT += (func[i]-48)*base;
		}
		else{
			if(func[i] < 'a')
				m_LUT +=((func[i]-'A'+10)*base);
			else if (func[i] > 'Z')
				m_LUT +=((func[i]-'a'+10)*base);
		}
		base *= 16;
	}
}


void Vertex::setLUT(unsigned long lut){
	m_LUT = lut;
}

unsigned long Vertex::getLUT(){
	return m_LUT;
}

int Vertex::getLevel(){
	return m_Level;
}

int Vertex::getNumInputs(){
	return m_Input.size();
}

int Vertex::getNumOutputs(){
	return m_Output.size();
}


void Vertex::getInPorts(std::vector<std::string>& portlist){
	portlist = m_InPorts;
}





void Vertex::addInput(Vertex* input){
	m_Input.push_back(input);
}

void Vertex::addInPort(std::string port){
	m_InPorts.push_back(port);
}

void Vertex::addOutput(Vertex* output, std::string port){
	m_Output[port].push_back(output);
}

std::string Vertex::removeOutputValue(int outputVal){
	std::map<std::string, std::vector<Vertex*> >::iterator it;
	for(it = m_Output.begin(); it != m_Output.end(); it++){
		for(unsigned int i = 0; i < it->second.size(); i++){
			if(it->second[i]->getID() == outputVal){
				it->second.erase(it->second.begin() + i);
				std::string port = it->first;
				if(it->second.size() == 0)
					m_Output.erase(it->first);
				return port;
			}
		}
	}
	printf("Remove output value %d Not found in output list\n", outputVal);
	printf("Vertex: %d\tOutputList: ", m_ID);
	for(it = m_Output.begin(); it != m_Output.end(); it++)
		for(unsigned int i = 0; i < it->second.size(); i++)
			printf("%d ", it->second[i]->getID());
	exit(1);
	return "";
}

int Vertex::removeInputValue(int inputVal){
	for(unsigned int i = 0; i < m_Input.size(); i++){
		if(m_Input[i]->getID() == inputVal){
			m_Input.erase(m_Input.begin() + i);
			return i;
		}
	}

	printf("Remove input value %d Not found in input list\n", inputVal);
	exit(1);

	return -1;
}

void Vertex::removeInPortValue(int index){
	m_InPorts.erase(m_InPorts.begin() + index);
}

void Vertex::getOutput(std::vector<Vertex*> &v){
	std::map<std::string, std::vector<Vertex*> >::iterator it;
	for(it = m_Output.begin(); it != m_Output.end(); it++){
		for(unsigned int i = 0; i < it->second.size(); i++)
			v.push_back(it->second[i]);
	}
}

void Vertex::getInput(std::vector<Vertex*> &v){
	v = m_Input;
}

void Vertex::getPortOutput(std::string port, std::vector<Vertex*>&v){
	v = m_Output[port];
}

void Vertex::getInputPorts(std::vector<std::string>&v){
	v = m_InPorts; 
}


	void Vertex::getOutputPorts(std::vector<std::string> &v){
		std::map<std::string, std::vector<Vertex*> >::iterator it;
		for(it = m_Output.begin(); it != m_Output.end(); it++){
			v.push_back(it->first);
		}
	}

std::string  Vertex::getOutputPortName(int id){
	std::map<std::string, std::vector<Vertex*> >::iterator it;
	for(it = m_Output.begin(); it != m_Output.end(); it++){
		for(unsigned int i = 0; i < it->second.size(); i++){
			if(it->second[i]->getID() == id)
				return it->first; 
		}
	}

	printf("Output ID %d not found\n", id);
	exit(1);
}

void Vertex::getOutputPortID(std::string port, std::vector<Vertex*>& ids){
	if(m_Output.find(port) == m_Output.end()){
		printf("NO OUTPUT PORT FOUND WITH LABEL: %s\n", port.c_str());
		exit(1);
	}

	ids = m_Output[port];
}

std::string  Vertex::getInputPortName(int id){
	for(unsigned int index = 0; index < m_Input.size(); index++){
		if(m_Input[index]->getID() == id)
			return m_InPorts[index];
	}

	printf("Input ID %d not found\n", id);
	exit(1);

}

int Vertex::getInputPortID(std::string port){
	for(unsigned int index = 0; index < m_InPorts.size(); index++){
		if(m_InPorts[index] == port)
			return m_Input[index]->getID();
	}
	//	printf("[WARNING] -- Input name %s not found\n", port.c_str());
	return -1;


}

unsigned int Vertex::getOVSize(){
	unsigned int size = 0;
	std::map<std::string, std::vector<Vertex*> >::iterator it;
	for(it = m_Output.begin(); it != m_Output.end(); it++){
		size += it->second.size();
	}
	return size;
}

unsigned int Vertex::getIVSize(){
	return m_Input.size();
}

int Vertex::getID(){
	return m_ID;
}

void Vertex::setVertexID(int ID){
	m_ID = ID;
}

std::string Vertex::getType(){
	return m_Type;
}

void Vertex::setType(std::string type){
	m_Type = type;
}

std::string Vertex::getName(){
	return m_Name;
}

void Vertex::setName(std::string name){
	m_Name= name;
}













