/*@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@
	@
	@      graph.cpp
	@      
	@      @AUTHOR:Kevin Zeng
	@      Copyright 2012 – 2013 
	@      Virginia Polytechnic Institute and State University
	@
	@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@*/

#include "graph.hpp"



/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *     Constructors
####*
############################################################## 
##############################################################*/

/***************************************************************************
 *  Constructor
 *    Empty
 ****************************************************************************/
Graph::Graph(std::string name){
	m_Name = name;
}





/***************************************************************************
 * Copy Constructor 
 *   Creates a deep copy of Graph 
 ****************************************************************************/
Graph::Graph(const Graph& copy){
	printf("[GRAPH] -- Copy Constructor");
	std::map<int, Vertex*>::const_iterator it;

	//Make new vertices
	for(it = copy.m_GraphV.begin(); it != copy.m_GraphV.end(); it++){
		Vertex* v = new Vertex (it->first, it->second->getType());
		v->setLUT(it->second->getLUT());
		v->setLevel(it->second->getLevel());
		m_GraphV[it->first] = v;
	}

	//Copy interconnections
	for(it = copy.m_GraphV.begin(); it != copy.m_GraphV.end(); it++){
		//Copy inputs
		std::vector<Vertex*> input;
		std::vector<std::string> inputPort;

		it->second->getInPorts(inputPort);
		it->second->getInput(input);

		//Make sure the two sizes are the same
		assert(input.size() == inputPort.size());

		for(unsigned int i = 0; i < input.size(); i++){
			m_GraphV[it->first]->addInput(m_GraphV[input[i]->getID()]);
			m_GraphV[it->first]->addInPort(inputPort[i]);
		}


		//Copy Outputs
		std::vector<Vertex*> output;
		it->second->getOutput(output);

		for(unsigned int i = 0; i < output.size(); i++){
			m_GraphV[it->first]->addOutput( m_GraphV[output[i]->getID()], output[i]->getOutputPortName(output[i]->getID()));
		}
	}

	std::map<std::string, int>::const_iterator itin;    
	for(itin = copy.m_Inputs.begin(); itin != copy.m_Inputs.end(); itin++){
		m_Inputs[itin->first] = itin->second;
	}

	std::map<std::string, int>::const_iterator inout;    
	for(inout = copy.m_Outputs.begin(); inout != copy.m_Outputs.end(); inout++){
		m_Outputs[inout->first] = inout->second;
	}

	for(unsigned int i = 0; i < m_Constants.size(); i++){
		m_Constants.push_back(copy.m_Constants[i]);
	}

	std::map<unsigned long long, int>::const_iterator itlut;
	for(itlut = copy.m_Luts.begin(); itlut != copy.m_Luts.end(); itlut++){
		m_Luts[itlut->first] = itlut->second;
	}

	m_Name = copy.m_Name;

}

/***************************************************************************
 *  Destructor
 *    Deletes allocated space. Does not delete AIG Graph
 ****************************************************************************/
Graph::~Graph(){
	std::map<int, Vertex*>::iterator it; 
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		if(it->second != NULL)
			delete it->second;
	}
}






/***************************************************************************
 * Operator =  
 *   Assignment operator overloaded  
 ****************************************************************************/
Graph& Graph::operator=(const Graph& copy){
	std::map<int, Vertex*>::const_iterator it;

	//Make new vertices
	for(it = copy.m_GraphV.begin(); it != copy.m_GraphV.end(); it++){
		Vertex* v = new Vertex (it->first, it->second->getType());
		v->setLUT(it->second->getLUT());
		v->setLevel(it->second->getLevel());
		m_GraphV[it->first] = v;
	}

	//Copy interconnections
	for(it = copy.m_GraphV.begin(); it != copy.m_GraphV.end(); it++){
		//Copy inputs
		std::vector<Vertex*> input;
		std::vector<std::string> inputPort;

		it->second->getInPorts(inputPort);
		it->second->getInput(input);

		//Make sure the two sizes are the same
		//printf("in: %d\tport:%d\n", input.size(), inputPort.size());
		assert(input.size() == inputPort.size());

		for(unsigned int i = 0; i < input.size(); i++){
			m_GraphV[it->first]->addInput(m_GraphV[input[i]->getID()]);
			m_GraphV[it->first]->addInPort(inputPort[i]);
		}


		//Copy Outputs
		std::vector<Vertex*> output;
		it->second->getOutput(output);

		for(unsigned int i = 0; i < output.size(); i++){
			m_GraphV[it->first]->addOutput( m_GraphV[output[i]->getID()], it->second->getOutputPortName(output[i]->getID()));
		}
	}

	std::map<std::string, int>::const_iterator itin;    
	for(itin = copy.m_Inputs.begin(); itin != copy.m_Inputs.end(); itin++){
		m_Inputs[itin->first] = itin->second;
	}

	std::map<std::string, int>::const_iterator inout;    
	for(inout = copy.m_Outputs.begin(); inout != copy.m_Outputs.end(); inout++){
		m_Outputs[inout->first] = inout->second;
	}

	for(unsigned int i = 0; i < m_Constants.size(); i++){
		m_Constants.push_back(copy.m_Constants[i]);
	}

	std::map<unsigned long long, int>::const_iterator itlut;
	for(itlut = copy.m_Luts.begin(); itlut != copy.m_Luts.end(); itlut++){
		m_Luts[itlut->first] = itlut->second;
	}

	m_Name = copy.m_Name;
	return *this;
}





/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *     Importers
####*
############################################################## 
##############################################################*/

/***************************************************************************
 *  importPrimitive
 *    Inputs AIG primitives (Output port names)
 *
 *  @PARAMS:
 *    fileName: File name of the graph to import
 *    offset:   Offset for VID to start importing as
 *  @Return: Success or not
 ****************************************************************************/
bool Graph::importPrimitive(std::string fileName, int offset){

	std::ifstream inFile;
	inFile.open(fileName.c_str());
	if (!inFile.is_open())	{
		fprintf(stderr, "[ERROR] -- Cannot open the file %s for import...exiting\n", fileName.c_str());
		fprintf(stderr, "\n*******************************************************************\n\n");
		exit(-1);
	}

	//printf("Reading in circuit information for %s\n", fileName.c_str());

	int numNodes;
	int last = 0;
	int numInputNodes;
	int numOutputNodes;

	inFile >> numNodes >> numInputNodes;


	//printf("Reading in input information...\n");
	for(int i = 0; i < numInputNodes; i++){
		int input; 
		inFile >> input;

		std::string port;
		inFile >> port;

		m_Inputs[port] = input + offset;
	}


	inFile >> numOutputNodes;
	for(int i = 0; i < numOutputNodes; i++){
		int output; 
		inFile >> output;

		std::string port;
		inFile >> port;

		//printf("PORT %s ID: %d\n", port.c_str(), output+offset);
		m_Outputs[port] = output + offset;
	}


	//printf("Reading in gate information...\n");
	for(int i = 0; i < numNodes; i++){
		int vID;
		std::string type;
		int fanin;
		int fanout;

		inFile >> vID;	
		//printf("VID: %d\n", vID);
		vID += offset;
		//printf("VID + offset: %d OFF: %d\n", vID, offset);

		if(vID > last)
			last = vID;
		//printf("VID: %d\n", vID);

		inFile >> type;
		//printf("ID: %d\tTYPE: %s\n", vID, type.c_str());
		Vertex* vertex;

		/*		if((std::string) type == "IN"){
					numInputs++;
					vertex = addInput(vID + last, type);
					}
					else*/
		if(m_GraphV.find(vID) == m_GraphV.end())
			vertex = addVertex(vID, type);
		else{
			vertex = m_GraphV[vID];
			vertex->setType(type);
		}


		inFile >> fanin;
		for(int j = 0; j < fanin; j++){
			int input;
			inFile >> input;

			std::string port;
			inFile >> port;

			input += offset;

			Vertex* in;
			if(m_GraphV.find(input) == m_GraphV.end())
				in = addVertex(input);
			else
				in = m_GraphV[input];	
			vertex->addInput(in);
			vertex->addInPort(port);
		}

		inFile >> fanout;
		for(int j = 0; j < fanout; j++){
			int output;
			inFile >> output;
			output += offset;

			std::string port;
			inFile >> port;

			Vertex* out;
			if(m_GraphV.find(output) == m_GraphV.end())
				out = addVertex(output);
			else
				out = m_GraphV[output];	
			vertex->addOutput(out, port);
		}


	}

	//printf("Import Complete\n");
	//printf("NUMV: %d\n", (unsigned int) m_GraphV.size());
	inFile.close();
	//std::map<std::string, int>::iterator it;
	//printf("MAP SIZE: %d\n", (int)m_Outputs.size());
	//for(it = m_Outputs.begin(); it != m_Outputs.end(); it++){
	//	printf("OUT: %d\n", it->second);
	//}

	return true;
}



/***************************************************************************
 *  importGraph
 *    Imports graph file
 *
 *  @PARAMS:
 *    fileName: File name of the graph to import
 *    offset:   Offset for VID to start importing as
 *  @Return: Success or not
 ****************************************************************************/
bool Graph::importGraph(std::string fileName, int offset){

	std::ifstream inFile;
	inFile.open(fileName.c_str());
	if (!inFile.is_open())	{
		fprintf(stderr, "[ERROR] -- Cannot open the file %s for import...exiting\n", fileName.c_str());
		fprintf(stderr, "\n*******************************************************************\n\n");
		exit(-1);
	}

	//printf("Reading in circuit information for %s\n", fileName.c_str());

	int numNodes;
	int last = 0;
	int numInputNodes;
	int numOutputNodes;
	int edges = 0;

	inFile >> numNodes >> numInputNodes;


	//printf("Reading in input information...\n");
	for(int i = 0; i < numInputNodes; i++){
		int input; 
		inFile >> input;

		std::string port;
		inFile >> port;

		m_Inputs[port] = input + offset;
	}

	inFile >> numOutputNodes;
	for(int i = 0; i < numOutputNodes; i++){
		int output; 
		inFile >> output;

		std::string port;
		inFile >> port;

		m_Outputs[port] = output + offset;
	}


	//printf("Reading in gate information...\n");
	for(int i = 0; i < numNodes; i++){
		int vID;
		std::string type;
		std::string name; 
		int fanin;
		int fanout;

		inFile >> vID;	
		vID += offset;

		if(vID > last)
			last = vID;
		//printf("VID: %d\n", vID);

		inFile >> type;
		inFile >> name;
		m_NodeName[vID] = name;




		//printf("ID: %d\tTYPE: %s\n", vID, type.c_str());
		Vertex* vertex;

		/*		if((std::string) type == "IN"){
					numInputs++;
					vertex = addInput(vID + last, type);
					}
					else*/
		if(m_GraphV.find(vID) == m_GraphV.end()){
			vertex = addVertex(vID, type);
			vertex->setName(name);
		}
		else{
			vertex = m_GraphV[vID];
			vertex->setType(type);
			vertex->setName(name);
		}


		inFile >> fanin;
		for(int j = 0; j < fanin; j++){
			int input;
			inFile >> input;

			std::string port;
			inFile >> port;

			input += offset;

			Vertex* in;
			if(m_GraphV.find(input) == m_GraphV.end())
				in = addVertex(input);
			else
				in = m_GraphV[input];	
			vertex->addInput(in);
			vertex->addInPort(port);
		}

		inFile >> fanout;


		for(int j = 0; j < fanout; j++){
			int output;
			inFile >> output;
			output += offset;

			std::string port;
			inFile >> port;

			Vertex* out;
			if(m_GraphV.find(output) == m_GraphV.end())
				out = addVertex(output);
			else
				out = m_GraphV[output];	
			vertex->addOutput(out, port);
			edges++;
		}

		if(type.find("LUT") != std::string::npos){
			std::string lutfunction;
			inFile >> lutfunction;
			vertex->setLUT(lutfunction);
			if(m_Luts.find(vertex->getLUT()) == m_Luts.end())
				m_Luts[vertex->getLUT()] = 0;
			else
				m_Luts[vertex->getLUT()]++;
		}
	}

	//printf("Import Complete\n");
	//printf("NUME: %d\n", edges);
	inFile.close();

	return true;
}


/***************************************************************************
 * exportGraph 
 *   exports graph into a G file 
 *   @PARAM: filename- File name of the new G file
 ****************************************************************************/
bool Graph::exportGraph(std::string filename){
	printf("[GRP] -- Exporting circuit to file: %s\n", filename.c_str());

	std::ofstream ofs(filename.c_str(), std::ofstream::out);

	//Size
	std::map<int, Vertex*>::iterator it;
	ofs<<m_GraphV.size()<<"\n";
	//std::cout<<m_GraphV.size()<<"\n";

	//Inputs
	std::map<std::string, int>::iterator it2;
	ofs<<m_Inputs.size()<< " ";
	//std::cout<<m_Inputs.size()<< " ";
	for(it2 = m_Inputs.begin(); it2 != m_Inputs.end(); it2++){
		//printf("%d %s  ", it2->second, it2->first.c_str());
		ofs<<it2->second<<" "<<it2->first<< "  ";
	}
	//printf("\n");
	ofs<<"\n";
	
	ofs<<m_Outputs.size()<< " ";
	//std::cout<<m_Outputs.size()<< " ";
	for(it2 = m_Outputs.begin(); it2 != m_Outputs.end(); it2++){
		//printf("%d %s  ", it2->second, it2->first.c_str());
		ofs<<it2->second<<" "<<it2->first<< "  ";
	}
	//printf("\n");
	ofs<<"\n";

	//Graph Content
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		//VID
		//printf("%d ", it->first);
		ofs<<it->first<<" ";

		//Type
		//printf("%s ", it->second->getType().c_str());
		ofs<<it->second->getType()<<" "<<it->second->getName()<<" ";

		//TODO:Node name
		//printf("%s ", m_NodeName[it->first].c_str());

		//Inputs
		std::vector<Vertex*> in;
		std::vector<std::string> port;
		it->second->getInput(in);
		it->second->getInPorts(port);
		ofs<<in.size()<<" ";
		//std::cout<<in.size()<<" ";
		for(unsigned int i = 0; i < in.size(); i++){
			//printf("%d %s ", in[i]->getID(), port[i].c_str() );
			ofs<<in[i]->getID()<<" "<<port[i]<<" ";
		}

		std::vector<Vertex*> out;
		it->second->getOutput(out);
		ofs<<out.size()<<" ";
		//std::cout<<out.size()<<" ";
		for(unsigned int i = 0; i < out.size(); i++){
			//printf("%d %s ", out[i]->getID(), it->second->getOutputPortName(out[i]->getID()).c_str());
			ofs<<out[i]->getID()<<" "<< it->second->getOutputPortName(out[i]->getID())<<" ";
		}

		if(it->second->getType().find("LUT") != std::string::npos){
			//printf("%llx", it->second->getLUT());	
			ofs<<std::hex<<it->second->getLUT()<<std::dec;
		}
		ofs<<"\n";
		//printf("\n");
	}

	//printf("\n\n");
	ofs.close();
	return true;
}	



/***************************************************************************
 * exportGraphSDFV2000
 *   exports graph into a SDF V2000 file 
 *   @PARAM: filename- File name of the new G file
 ****************************************************************************/
bool Graph::exportGraphSDFV2000(std::string cname, int ID){

	std::string filename = "converted/" + cname  + ".sdf";
	printf("[GRP] -- Exporting circuit to V2000 file: %s\n", filename.c_str());

	std::ofstream ofs(filename.c_str(), std::ofstream::out);

	//Header
	std::map<int, Vertex*>::iterator it;
	std::cout<<"11280714432D 1   1.00000     0.00000     0\n\n\n";
	ofs<<"\n          11280714432D 1   1.00000     0.00000     0\n\n";

	//Size
	std::cout<<m_GraphV.size()<<" "<<getNumNets()<<" 0 0 0 999 V2000\n";
	ofs<<m_GraphV.size()<<" "<<getNumNets()<<"  0  0  0  0  0  0  0  0  1 V2000\n";

	//vid, sequential value
	std::map<int,int> mapping;
	std::map<int,int>::iterator mit;
	int val = 1;

	//NODES
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		//VID
		std::string type = it->second->getType();
		std::string element;

		if(type.find("INV") != std::string::npos)
			element = "C";
		else if(type.find("AND") != std::string::npos)
			element = "O";
		else if(type.find("FF") != std::string::npos)
			element = "N";
		else if(type.find("IN") != std::string::npos)
			element = "H";
		else
			element = "Cl";

		std::cout<<"0.0000 0.0000\t0.0000 "<<element<<" ("<<it->second->getType()<<") 0 0 0 0 0 0\t0 0 0\n";
		ofs<<"    3.4042  -19.7042    0.0000 "<<element<<"   0  0  0  0  0  0  0  0  0  0  0  0\n";
		mapping[it->first] = val;
		val++;
	}

	//Nets
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		std::vector<Vertex*> out;
		it->second->getOutput(out);
		for(unsigned int i = 0; i < out.size(); i++){
			printf("%d %d 1 0\t0 0\n", mapping[it->first], mapping[out[i]->getID()]);
			ofs<<mapping[it->first]<<"  "<<mapping[out[i]->getID()]<<"  1  0     0  0\n";
		}
	}


	ofs<<"M  END\n>  <chebi_id>\n"<<cname<<ID<<"\n\n$$$$\n";
	std::cout<<"M  END\n>  <chebi_id>\n"<<cname<<ID<<"\n\n$$$$\n";

	ofs.close();
	return true;
}	





/***************************************************************************
 * exportGraphSDFV3000
 *   exports graph into a SDF V3000 file 
 *   @PARAM: filename- File name of the new G file
 ****************************************************************************/
bool Graph::exportGraphSDFV3000(std::string cname, int ID){

	std::string filename = "converted/" + cname  + ".sdf";
	printf("[GRP] -- Exporting circuit to V3000 file: %s\n", filename.c_str());

	std::ofstream ofs(filename.c_str(), std::ofstream::out);

	//Header
	std::map<int, Vertex*>::iterator it;
	ofs<<cname<<"\n\n\n  0  0  0     0  0            999 V3000\n"; 
	//std::cout<<cname<<"\n\n\n  0  0  0     0  0            999 V3000\n"; 

	//Size
	ofs<<"M  V30 BEGIN CTAB\nM  V30 COUNTS "<<m_GraphV.size()<<" "<<getNumNets()<<" 0 0 0\n";
	//std::cout<<"M  V30 BEGIN CTAB\nM  V30 COUNTS "<<m_GraphV.size()<<" "<<getNumNets()<<" 0 0 0\n";

	//vid, sequential value
	std::map<int,int> mapping;
	std::map<int,int>::iterator mit;
	int val = 1;

	//NODES
	ofs<<"M  V30 BEGIN ATOM\n";
	//std::cout<<"M  V30 BEGIN ATOM\n";
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		//VID
		std::string type = it->second->getType();
		std::string element;

		if(type.find("INV") != std::string::npos)
			element = "C";
		else if(type.find("AND") != std::string::npos)
			element = "O";
		else if(type.find("FF") != std::string::npos)
			element = "N";
		else if(type.find("IN") != std::string::npos)
			element = "H";
		else
			element = "Cl";

		ofs<<"M  V30 "<<val<<" " <<element<<" 7.90196 6.19608 0.00000 0\n";
		//std::cout<<"M  V30 "<<val<<" " <<element<<" 7.90196 6.19608 0.00000 0\n";
		mapping[it->first] = val;
		val++;
	}
	ofs<<"M  V30 END ATOM\n";
	//std::cout<<"M  V30 END ATOM\n";

	//Nets
	ofs<<"M  V30 BEGIN BOND\n";
	//std::cout<<"M  V30 BEGIN BOND\n";
	val = 1;
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		std::vector<Vertex*> out;
		it->second->getOutput(out);
		for(unsigned int i = 0; i < out.size(); i++){
			ofs<<"M  V30 "<<val<<" 1 "<<mapping[it->first]<<" "<<mapping[out[i]->getID()]<<"\n";
			//std::cout<<"M  V30 "<<val<<" 1 "<<mapping[it->first]<<" "<<mapping[out[i]->getID()]<<"\n";
			val++;
		}
	}
	ofs<<"M  V30 END BOND\nM  V30 END CTAB\n";
	//std::cout<<"M  V30 END BOND\nM  V30 END CTAB\n";


	ofs<<"M  END\n>  <chebi_id>\n"<<cname<<ID<<"\n\n$$$$\n";
	//std::cout<<"M  END\n>  <chebi_id>\n"<<cname<<ID<<"\n\n$$$$\n";

	ofs.close();
	return true;
}	






/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####	
#### *    Getters 
####*
############################################################## 
##############################################################*/

/***************************************************************************
 *  getNumNets
 *    Gets the number of nets 
 *
 *  @Return: Number of nets 
 ****************************************************************************/
int Graph::getNumNets(){
	int numNets = 0; 

	//BFS
	std::list<unsigned int> queue;
	std::set<unsigned int> marked;

	//Put initial inputs into BFS and marked
	std::map<std::string, int>::iterator it;
	for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		queue.push_back(it->second);
		marked.insert(it->second);
	}

	while(queue.size() != 0){
		int item = queue.front();
		queue.pop_front();

		std::vector<Vertex*> outputs;
		m_GraphV[item]->getOutput(outputs);

		for(unsigned int i = 0; i < outputs.size(); i++){
			if(marked.find(outputs[i]->getID()) == marked.end()){
				marked.insert(outputs[i]->getID());
				queue.push_back(outputs[i]->getID());
			}
			numNets++;
		}
	}
	//END BFS
	return numNets;	
}






/***************************************************************************
 *  getVertex 
 *    Gets the vertex given the VID
 *
 *  @PARAMS:
 *    vertex:   Vertex ID of the Vertex
 *  @Return: Vertex object
 ****************************************************************************/
Vertex* Graph::getVertex(int vertex){
	if(m_GraphV.find(vertex) == m_GraphV.end())
		return NULL;
	else
		return m_GraphV[vertex];
}



/***************************************************************************
 *  getNumVertex 
 *    Gets the number of vertices in the graph
 *
 *  @Return: Number of vertices 
 ****************************************************************************/
unsigned int Graph::getNumVertex(){
	return m_GraphV.size();
}



/***************************************************************************
 *  getNumInputs
 *    Gets the number of inputs to the graph
 *
 *  @Return: Number of inputs
 ****************************************************************************/
unsigned int Graph::getNumInputs(){
	return m_Inputs.size();
}


/***************************************************************************
 *  getNumOutputs
 *    Gets the number of outputs to the graph
 *
 *  @Return: Number of outputs
 ****************************************************************************/
unsigned int Graph::getNumOutputs(){
	return m_Outputs.size();
}

/***************************************************************************
 *  getInputs
 *    Gets the inputs to the graph
 *
 *  @PARAM:
 *    input:  Reference to the list of inputs to graph
 ****************************************************************************/
void Graph::getInputs(std::vector<int>& input){
	std::map<std::string, int>::iterator it;
	for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		input.push_back(it->second);
	}
}

void Graph::getInputs(std::map<std::string, int>& input){
	input = m_Inputs;
}



/***************************************************************************
 *  getOutputs
 *    Gets the outputs to the graph
 *
 *  @PARAM:
 *    output:  Reference to the list of outputs to graph
 ****************************************************************************/
void Graph::getOutputs(std::vector<int>& output){
	std::map<std::string, int>::iterator it;
	//printf("MAP SIZE: %d\n", (int)m_Outputs.size());
	for(it = m_Outputs.begin(); it != m_Outputs.end(); it++){
		//printf("OUT: %d\n", it->second);
		output.push_back(it->second);
	}
}

void Graph::getOutputs(std::map<std::string, int>& output){
	output = m_Outputs;
}


/***************************************************************************
 *  getLast
 *    Gets the largest VID of the graph
 *
 *  @RETURN: The last VID of the graph
 ****************************************************************************/
int Graph::getLast(){
	std::map<int, Vertex*>::iterator it; 
	it = m_GraphV.end();
	it--;
	return it->first;
}



/***************************************************************************
 *  getMaxLevel
 *    Gets the largest level of the graph
 *
 *  @RETURN: The largest level of the graph
 ****************************************************************************/
unsigned int Graph::getMaxLevel(){
	//printf("\nGetting Max Level\n");
	unsigned int max = 0;
	std::map<int, Vertex*>::iterator it; 
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		if(it->second->getLevel() == -1){
			printf("Level not set:\t%d\n", it->first);
			exit(1);
		}
		if((unsigned int)it->second->getLevel() > max)
			max = it->second->getLevel();
	}
	return max;
}



/***************************************************************************
 *  getName
 *    Gets the name of the graph 
 *
 *  @RETURN: Returns the name of the graph 
 ****************************************************************************/
std::string Graph::getName(){
	return m_Name;
}




/***************************************************************************
 *  getNodeName
 *    Gets the name of a node 
 *
 *  @RETURN: Returns the name of the node 
 ****************************************************************************/
std::string Graph::getNodeName(unsigned int node){
	return m_NodeName[node];
}





void Graph::getLUTs(std::map<unsigned long long, int>& lut){
	lut = m_Luts;
}




bool Graph::hasLUTs(){
	if(m_Luts.size() == 0)
		return false;
	else
		return true;
}


bool Graph::isOutput(std::string nodeName){
	if(m_Outputs.find(nodeName) == m_Outputs.end())
		return false;
	else return true;
}

std::string Graph::isOutput(int nodeID){
	std::map<std::string, int>::iterator it; 
	for(it = m_Outputs.begin(); it != m_Outputs.end(); it++){
		if(it->second == nodeID)
			return it->first;
	}
	
	return "";
}

void Graph::setOutput(std::string nodeName, int nodeID){
	m_Outputs[nodeName] = nodeID;
}



/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *    Setters 
####*
############################################################## 
##############################################################*/


/***************************************************************************
 *  setName
 *    Sets the name of the graph
 *
 *  @PARAM:
 *    name:  Name to set the graph
 ****************************************************************************/
void Graph::setName(std::string name){
	m_Name = name;
}





/***************************************************************************
 *  setLevels 
 *    Sets the order of the gates (Depth) 
 ****************************************************************************/
void Graph::setLevels(){
	//printf("Setting Levels\n");
	std::map<std::string, int>::iterator it;
	std::list<int> mark;
	for(unsigned int i = 0; i < m_Constants.size(); i++)
		DFSlevel(m_GraphV[m_Constants[i]], mark, 0);

	for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		//printf("Pushing\tINPUTV%d\n", it->second);
		DFSlevel(m_GraphV[it->second], mark, 0);
	}

}



/***************************************************************************
 *  resetLevels 
 *    Resets the order of the gates (Depth) 
 ****************************************************************************/
void Graph::resetLevels(){
	std::map<int, Vertex* >::iterator it; 
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++)
		it->second->setLevel(-1);

}


void Graph::removeCycles(){
	std::map<std::string, int>::iterator it;
	std::list<int> mark;
	for(unsigned int i = 0; i < m_Constants.size(); i++)
		DFScycle(m_GraphV[m_Constants[i]], mark);

	for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		//printf("Pushing\tINPUTV%d\n", it->second);
		DFScycle(m_GraphV[it->second], mark);
	}
}





/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *    Iterators 
####*
############################################################## 
##############################################################*/

/***************************************************************************
 *  begin
 *    Gets the begin iterator for the graph
 *
 *  @RETURN: Begin iterator to the graph
 ****************************************************************************/
std::map<int, Vertex*>::iterator Graph::begin(){
	return m_GraphV.begin();
}



/***************************************************************************
 *  end
 *    Gets the end iterator for the graph
 *
 *  @RETURN: End iterator to the graph
 ****************************************************************************/
std::map<int, Vertex*>::iterator Graph::end(){
	return m_GraphV.end();
}



/***************************************************************************
 *  getIterators
 *    Gets the reference for both begin and end iterators
 *
 *  @PARAM:
 *    begin:  Reference to begin iterator
 *    end:    Reference to end iterator
 ****************************************************************************/
void Graph::getIterators(std::map<int, Vertex*>::iterator& begin, std::map<int,Vertex*>::iterator& end ){
	begin = m_GraphV.begin();
	end = m_GraphV.end();
}





/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *    Print
####*
############################################################## 
##############################################################*/

/***************************************************************************
 *  print
 *    Prints contents of the Graph
 ****************************************************************************/
void Graph::print(){
	std::map<int, Vertex*>::iterator it;
	/*for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		printf("V%d\tL:%d\tT: %s ", it->first, it->second->getLevel(), it->second->getType().c_str());

		printf("\tOUT: ");
		std::vector<int> out;
		it->second->getOutput(out);
		for(unsigned int i = 0; i < out.size(); i++)
		printf("%d ", out[i]);
		printf("\n");
		}*/
	std::map<std::string, int>::iterator it2;
	printf("Inputs:\tSIZE:%d\n", (int)m_Inputs.size());	
	for(it2 = m_Inputs.begin(); it2 != m_Inputs.end(); it2++){
		printf("%s:%d   ", it2->first.c_str(), it2->second);
	}


	printf("\nOutputs:\tSIZE:%d\n", (int)m_Outputs.size());	
	for(it2 = m_Outputs.begin(); it2 != m_Outputs.end(); it2++){
		printf("%s:%d   ", it2->first.c_str(), it2->second);
	}
	printf("\n");

	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		printf("V%d\t", it->first);
		printf("L:%d\t", it->second->getLevel());
		printf("T: %s\t", it->second->getType().c_str());
		printf("N:%s\tIN: ", it->second->getName().c_str());

		std::vector<Vertex*> in;
		std::vector<std::string> port;
		it->second->getInput(in);
		it->second->getInPorts(port);
		for(unsigned int i = 0; i < in.size(); i++)
			printf("%d %s ", in[i]->getID(), port[i].c_str() );

		printf("\tOUT: ");
		std::vector<Vertex*> out;
		it->second->getOutput(out);
		for(unsigned int i = 0; i < out.size(); i++)
			printf("%d ", out[i]->getID());

		if(it->second->getType().find("LUT") != std::string::npos){
			printf("FUNC: %llx", it->second->getLUT());	
		}

		printf("\n");
	}

	printf("\n\n");
}	



/***************************************************************************
 *  print
 *    Prints contents of the Graph in G format
 ****************************************************************************/
void Graph::printg(){
	std::map<std::string, int>::iterator it2;
	printf("Inputs:\tSIZE:%d\n", (int)m_Inputs.size());	
	for(it2 = m_Inputs.begin(); it2 != m_Inputs.end(); it2++){
		printf("%s:%d   ", it2->first.c_str(), it2->second);
	}


	printf("\nOutputs:\tSIZE:%d\n", (int)m_Outputs.size());	
	for(it2 = m_Outputs.begin(); it2 != m_Outputs.end(); it2++){
		printf("%s:%d   ", it2->first.c_str(), it2->second);
	}
	printf("\n");

	std::map<int, Vertex*>::iterator it;
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		printf("%d %s %s", it->first, it->second->getType().c_str(), it->second->getName().c_str());
		std::vector<Vertex*> in;
		std::vector<std::string> port;
		it->second->getInput(in);
		it->second->getInPorts(port);
		printf(" %d ", (unsigned int) in.size());
		for(unsigned int i = 0; i < in.size(); i++)
			printf("%d %s ", in[i]->getID(), port[i].c_str());

		std::vector<Vertex*> out;
		it->second->getOutput(out);
		printf(" %d ", (unsigned int)out.size());
		for(unsigned int i = 0; i < out.size(); i++)
			printf("%d %s ", out[i]->getID(), it->second->getOutputPortName(out[i]->getID()).c_str());
		printf("\n");

	}

	printf("\n\n");
}	


/***************************************************************************
 *  printVertex
 *    Prints contents of the Graph in G format
 ****************************************************************************/
void Graph::printVertex(int v){
	Vertex* vertex = m_GraphV[v];

	printf("%d %s ", v, vertex->getType().c_str());
	std::vector<Vertex*> in;
	std::vector<std::string> port;
	vertex->getInput(in);
	vertex->getInPorts(port);
	printf(" %d ", (unsigned int) in.size());
	for(unsigned int i = 0; i < in.size(); i++)
		printf("%d %s ", in[i]->getID(), port[i].c_str());

	std::vector<Vertex*> out;
	vertex->getOutput(out);
	printf(" %d ", (unsigned int)out.size());
	for(unsigned int i = 0; i < out.size(); i++)
		printf("%d %s ", out[i]->getID(), vertex->getOutputPortName(out[i]->getID()).c_str());
	printf("\n");

}	








/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *    Finders
####*
############################################################## 
##############################################################*/

/***************************************************************************
 *  findInPort
 *    Finds the vertex associated with the specific
 *    input port
 *
 *  @PARAMS:
 *    portname: Name of input port to search for
 *  @RETURN: Vertex ID of the input port
 ****************************************************************************/
int Graph::findInPort(std::string portname){
	if(m_Inputs.find(portname) != m_Inputs.end())
		return m_Inputs[portname];
	else{
		printf("[ERROR] -- (graph.findInPort)\n");
		printg();
		printf("-- Finding Input Port Name %s\n", portname.c_str());
		std::map<std::string, int>::iterator it;
		for(it=m_Inputs.begin(); it!=m_Inputs.end(); it++)
			printf("Port %s: %d\n", it->first.c_str(), it->second);

		exit(1);
		return -1;
	}
}



/***************************************************************************
 *  findOutPort
 *    Finds the vertex associated with the specific
 *    output port
 *
 *  @PARAMS:
 *    portname: Name of output port to search for
 *  @RETURN: Vertex ID of the output port
 ****************************************************************************/
int Graph::findOutPort(std::string portname){
	if(m_Outputs.find(portname) != m_Outputs.end())
		return m_Outputs[portname];
	else{
		printf("[ERROR] -- (graph.findOutPort)\n");
		printf("-- Finding Output Port Name %s\n", portname.c_str());
		std::map<std::string, int>::iterator it;
		for(it=m_Outputs.begin(); it!=m_Outputs.end(); it++)
			printf("Port %s: %d\n", it->first.c_str(), it->second);

		exit(1);
		return -1;
	}
}




/***************************************************************************
 *  findInPortName
 *    Finds the port name associated with the specific
 *    input port
 *
 *  @PARAMS:
 *    port: Input Port ID
 *  @RETURN: Name of the input port
 ****************************************************************************/
std::string Graph::findInPortName(int port){
	std::map<std::string, int>::iterator it;
	for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		if(it->second == port)
			return it->first;
	}

	printf("Port ID:%d is not an input to the circuit\n", port);
	printg();
	exit(1);

}



/***************************************************************************
 *  findInPortName
 *    Finds the port name associated with the specific
 *    input port
 *
 *  @PARAMS:
 *    port: Input Port ID
 *  @RETURN: Name of the input port
 ****************************************************************************/
std::string Graph::findOutPortName(int port){
	std::map<std::string, int>::iterator it;
	for(it = m_Outputs.begin(); it != m_Outputs.end(); it++){
		if(it->second == port)
			return it->first;
	}

	printf("Port ID:%d is not an input to the circuit\n", port);
	printg();
	exit(1);

}










/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *    Graph Functions 
####*
############################################################## 
##############################################################*/

/***************************************************************************
 *  addVertex 
 *    Adds a Vertex to the Graph 
 *
 *  @PARAMS:
 *    vertex:  Vertex Object to be added
 *  @RETURN: Newly created Vertex Object
 ****************************************************************************/
void Graph::addVertex(Vertex* vertex){
	m_GraphV[vertex->getID()] = vertex;
}



/***************************************************************************
 *  addVertex 
 *    Adds a Vertex to the Graph 
 *
 *  @PARAMS:
 *    VID:  ID of the new Vertex
 *    type: Type of the Vertex
 *  @RETURN: Newly created Vertex Object
 ****************************************************************************/
Vertex* Graph::addVertex(int vID, std::string type){
	if(m_GraphV.find(vID) != m_GraphV.end()){
		printf("[ERROR] -- (graph.addVertex))\n");
		printf("-- VID %d already exists in graph\n", vID);
		exit(1);
	}
	Vertex *v = new Vertex(vID, type);
	m_GraphV[vID] = v;
	return v;
}



/***************************************************************************
 *  addVertex 
 *    Adds an empty Vertex to the Graph 
 *
 *  @PARAMS:
 *    VID:  ID of the new Vertex
 *  @RETURN: Newly created Vertex Object
 ****************************************************************************/
Vertex* Graph::addVertex(int vID){
	if(m_GraphV.find(vID) != m_GraphV.end()){
		printf("[ERROR] -- (graph.addVertex))\n");
		printf("-- VID %d already exists in graph\n", vID);
		exit(1);
	}
	Vertex *v = new Vertex(vID);
	m_GraphV[vID] = v;

	return v;
}


/***************************************************************************
 *  addVertex 
 *    Adds a Vertex to the Graph 
 *
 *  @PARAMS:
 *    VID:  ID of the new Vertex
 *    v:    Vertex to replace
 *  @RETURN: Newly created Vertex Object
 ****************************************************************************/
Vertex* Graph::addVertex(int vID, Vertex* v){
	if(m_GraphV.find(vID) != m_GraphV.end())
		delete m_GraphV[vID];

	m_GraphV[vID] = v;
	return v;
}



/***************************************************************************
 *  addVertex 
 *    Adds a Vertex to the Graph 
 *
 *  @PARAMS:
 *    VID:  ID of the new Vertex
 *    v:    Vertex to replace
 *  @RETURN: Newly created Vertex Object
 ****************************************************************************/
void Graph::removeVertex(Vertex* vertex){
	int id = vertex->getID();
	delete m_GraphV[id];
	m_GraphV.erase(id);

	//Check to see if it is in input list
	std::map<std::string, int>::iterator it;
	for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		if(it->second == id){
			m_Inputs.erase(it->first);
			break;
		}
	}

}



/***************************************************************************
 *  removeVertex 
 *    Removes a Vertex from the Graph 
 *
 *  @PARAMS:
 *    VID:  ID of the new Vertex
 *    v:    Vertex to replace
 *  @RETURN: Newly created Vertex Object
 ****************************************************************************/
void Graph::removeVertex(int vertex){
	delete m_GraphV[vertex];
	m_GraphV.erase(vertex);

	//Check to see if it is in input list
	std::map<std::string, int>::iterator it;
	for(it = m_Inputs.begin(); it != m_Inputs.end(); it++){
		if(it->second == vertex){
			m_Inputs.erase(it->first);
			break;
		}
	}
}



/***************************************************************************
 *  addInput
 *    Adds a Vertex as an input to the graph
 *
 *  @PARAMS:
 *    portName: Port name of the input
 *    node:     VID of the input port
 ****************************************************************************/
void Graph::addInput(std::string portName, int node){
	m_Inputs[portName] = node;
}



/***************************************************************************
 *  addOutput
 *    Adds a Vertex as an Output to the graph
 *
 *  @PARAMS:
 *    portName: Port name of the input
 *    node:     VID of the input port
 ****************************************************************************/
void Graph::addOutput(std::string portName, int node){
	m_Outputs[portName] = node;
}



/***************************************************************************
 *  addInput
 *    Adds a Vertex as an constant val to the graph
 *
 *  @PARAMS:
 *    node:     VID of the constant port
 ****************************************************************************/
void Graph::addConstant( int node){
	m_Constants.push_back(node);
}


void Graph::renameNodes(std::string name){
	std::map<int, Vertex*>::iterator it;
	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++)
		it->second->setName(name);
}



/***************************************************************************
 *  substitute 
 *    Substitutes a node in the graph with a graph 
 *
 *  @PARAMS:
 *    node:  VID of the node to be substituted
 *    sub:   Graph to substitute the node for
 ****************************************************************************/
unsigned Graph::substitute(int node, Graph* sub){
	//printf("SUBSTITUTION! NODE: %d\n", node);
	Vertex* nodeV = m_GraphV[node];
	sub->renameNodes(nodeV->getName());
	std::map<int, Vertex*>::iterator it; 


	//Get IO from the Subgraph
	std::vector<int> subInputs;
	std::vector<int> subOutputs;
	sub->getInputs(subInputs);
	sub->getOutputs(subOutputs);
	assert(subOutputs.size() == 1);
	int subOutNode = subOutputs[0];


	//Get IO of the substituting node
	std::vector<Vertex*> nodeInputs;
	std::vector<Vertex*> nodeOutputs;
	std::vector<std::string> nodeInputPorts;

	nodeV->getInput(nodeInputs);
	nodeV->getInPorts(nodeInputPorts);
	nodeV->getOutput(nodeOutputs);




	/********************
		HANDLE INPUTS
	 ********************/
	//Make sure inputs to node and inputs to subgraph are the same
	assert(nodeInputs.size() == subInputs.size());

	//Have inputs of the node point to inputs of subgraph
	for(unsigned int i = 0; i < nodeInputs.size(); i++){

		//Remove node from the output of its inputs
		std::string outPortName = nodeInputs[i]->removeOutputValue(node);

		//Get the portname of input to match to sub
		std::string inPortName = nodeInputPorts[i];
		int subInputIndex = sub->findInPort(inPortName);
		//printf("Input port name: %s, Index of input in sub to delete: %d\n", inPortName.c_str(), subInputIndex);

		//Get the outputs of IN to sub
		std::vector<Vertex*> sub_Out_of_In;
		sub->getVertex(subInputIndex)->getOutput(sub_Out_of_In);

		//Connect the inputNode to the input of sub
		for(unsigned int j = 0; j < sub_Out_of_In.size(); j++){
			nodeInputs[i]->addOutput(sub_Out_of_In[j], outPortName);

			//Remove the original input of the currently added output
			//printf("removing input %d from %d\n",  subInputIndex, sub_Out_of_In[j]->getID());
			std::string inName = sub_Out_of_In[j]->getInputPortName(subInputIndex);
			int index = sub_Out_of_In[j]->removeInputValue(subInputIndex);
			assert(index != -1);
			sub_Out_of_In[j]->removeInPortValue(index);
			sub_Out_of_In[j]->addInput(nodeInputs[i]);
			sub_Out_of_In[j]->addInPort(inName);
		}
	}






	/********************
		HANDLE OUTPUTS 
	 ********************/
	//TODO: Add output parameter in graph. 
	//Makes sure there is an output from the sub		
	bool found = false;
	for(it = sub->begin(); it != sub->end(); it++){
		if(it->second->getOVSize() == 0){
			found = true;
			break;
		}
	}

	assert(found);



	std::vector<std::string> outPortNames;
	nodeV->getOutputPorts(outPortNames);

	for(unsigned int i = 0; i < outPortNames.size(); i++){
		//printf("Outport of node: %s\n", outPortNames[i].c_str());
		std::vector<Vertex*> outputs;
		nodeV->getPortOutput(outPortNames[i], outputs);

		int subOutputIndex = sub->findOutPort(outPortNames[i]);
		//printf("Index of output of prim %d\n", subOutputIndex);
		for(unsigned int j = 0; j < outputs.size(); j++){
			//printf("adding %d\n", outputs[j]->getID());
			sub->getVertex(subOutputIndex)->addOutput(outputs[j], outPortNames[i]);

			//printf("removing2 input %d from %d\n", node, outputs[j]->getID());
			int index = outputs[j]->removeInputValue(node);
			//printf("adding input %d\n", subOutputIndex);
			outputs[j]->addInput(sub->getVertex(subOutputIndex));

			std::vector<std::string> outPorts;
			outputs[j]->getInPorts(outPorts);
			outputs[j]->addInPort(outPorts[index]);
			outputs[j]->removeInPortValue(index);

		}
	}

	//Add the new vertices into the graph
	for(it = sub->begin(); it != sub->end(); it++){
		if(it->second->getType() != "IN"){
			addVertex(it->second->getID(), it->second);
			it->second = NULL; //Make sure delete sub does not delete content
		}
	}

	std::string outPort = isOutput(node);
	if(outPort != "")
		setOutput(outPort, subOutNode);

	delete sub;
	//printf("SUBSTITUTION COMPLETE\n");
	//print();
	//printf("DONE SUB\n");
	return subOutputs[0];

}




void Graph::renumber(int lb){
	std::map<int, Vertex*> newGraph;
	std::map<int, Vertex*>::iterator it;

	for(it = m_GraphV.begin(); it != m_GraphV.end(); it++){
		int newID = it->second->getID()+lb;
		it->second->setVertexID(newID);
		newGraph[newID] = it->second;
	}

	m_GraphV = newGraph;


	std::map<std::string, int>::iterator it2; 
	for(it2 = m_Inputs.begin(); it2 != m_Inputs.end(); it2++){
		it2->second = it2->second + lb;
	}
	for(it2 = m_Outputs.begin(); it2 != m_Outputs.end(); it2++){
		it2->second = it2->second + lb;
	}
	for(unsigned int i = 0; i < m_Constants.size(); i++)
		m_Constants[i] = m_Constants[i]+lb;
}



/*##########################################
#################################################
#####################################################
##########################################################
###############################################################
####*
#### *    Search 
####*
############################################################## 
##############################################################*/

int Graph::DFSearchOut(std::list<int>& mark, Vertex* start, std::set<int>& candidate){
	//printf("DFS CHECKING V: %d \n", start->getID());

	mark.push_back(start->getID());

	std::vector<Vertex*> out;
	if(start->getType().find("FD") != std::string::npos)
		start->getOutputPortID("Q", out);
	else
		start->getOutput(out);

	std::list<int>::iterator it; 
	bool found;

	for(unsigned int i = 0; i < out.size(); i++){
		//printf("OUT: %d\n", out[i]->getID());
		if(out[i]->getType().find("FD") != std::string::npos){
			mark.push_back(out[i]->getID());
			candidate.insert(out[i]->getID());
			continue;
		}

		found = false;
		for(it = mark.begin(); it!= mark.end(); it++){
			if(*it == out[i]->getID()){
				found = true;
				break;
			}
		}

		if(!found){
			DFSearchOut(mark, out[i], candidate);
		}
	}

	mark.pop_back();
	return 0;

}


/*
	 if return is 1, input output loopback is found
	 if return is -1, input output loopback is not found
 */
int Graph::DFS_FSM_S(std::set<int>& mark, int stop, Vertex* start){
	//printf("DFS CHECKING V: %d \n", start->getID());
	if(start->getID() == stop){
		//Found the same FF in the input
		return 1;
	}


	mark.insert(start->getID());
	std::vector<Vertex*> in;
	start->getInput(in);

	for(unsigned int i = 0; i < in.size(); i++){
		//Check if input is marked. If not, traverse
		if(mark.find(in[i]->getID()) == mark.end()){
			int rval = DFS_FSM_S(mark, stop, in[i]);
			if(rval > 0)	return rval;
		}
	}

	return -1;

}

void Graph::DFS_FSM_M(std::set<int>& mark, std::set<Vertex*>& stop, Vertex* start, std::set<Vertex*>& candidate){
	//keep track which of the stop vertices are visited
	if(stop.find(start) != stop.end()){
		//printf("FOUND INITIAL FF\n");
		candidate.insert(start);
		return;
	}
	else if(start->getType().find("FD") != std::string::npos){
		mark.insert(start->getID());
		return;
	}

	//printf("DFS CHECKING V: %d \n", start->getID());
	mark.insert(start->getID());
	std::vector<Vertex*> in;
	start->getInput(in);

	for(unsigned int i = 0; i < in.size(); i++){
		//printf("OUT: %d\n", out[i]->getID());
		if(mark.find(in[i]->getID()) == mark.end())
			DFS_FSM_M(mark, stop, in[i], candidate);
	}
}

void Graph::DFS_FSM_T(std::set<int>& mark, Vertex* start, std::set<Vertex*>& candidate){
	//keep track which of the stop vertices are visited
	if(start->getType().find("FD") != std::string::npos){
		mark.insert(start->getID());
		candidate.insert(start);
		return;
	}

	//printf("DFS CHECKING V: %d \n", start->getID());
	mark.insert(start->getID());
	std::vector<Vertex*> out;
	start->getOutput(out);

	for(unsigned int i = 0; i < out.size(); i++){
		if(mark.find(out[i]->getID()) == mark.end())
			DFS_FSM_T(mark, out[i], candidate);
	}

}


int Graph::DFSearchIn (std::list<int>& mark, std::set<int>& stop, Vertex* start, std::set<int>& candidate){
	//keep track which of the stop vertices are visited
	if(stop.find(start->getID()) != stop.end()){
		//printf("FOUND INITIAL FF\n");
		candidate.insert(start->getID());
		return 0;
	}
	else if(start->getType().find("FD") != std::string::npos){
		mark.push_back(start->getID());
		return 0;
	}

	//printf("DFS CHECKING V: %d \n", start->getID());
	mark.push_back(start->getID());
	std::vector<Vertex*> in;
	start->getInput(in);

	std::list<int>::iterator it; 

	bool found;
	//TODO: Make mark into a set
	for(unsigned int i = 0; i < in.size(); i++){
		//printf("OUT: %d\n", out[i]->getID());

		found = false;
		for(it = mark.begin(); it!= mark.end(); it++){
			if(*it == in[i]->getID()){
				found = true;
				break;
			}
		}
		if(!found){
			DFSearchIn(mark, stop, in[i], candidate);
		}
	}

	mark.pop_back();
	return 0;
}

/***************************************************************************
 *  DFS 
 *    Depth First search 
 *
 *  @PARAMS: 
 *    vertex:  Starting vertex for DFS
 *    mark:    Marks visited to search for feedback
 *    level:   Level of traversal
 ****************************************************************************/
void Graph::DFSlevel(Vertex* vertex, std::list<int>& mark, int level){
	if(vertex->getLevel() < level)
		vertex->setLevel(level);
	else{
		return;
	}

	mark.push_back(vertex->getID());
	//printf("VID PUSHED %d\n", vertex->getID());

	std::vector<Vertex*> out;
	vertex->getOutput(out);
	std::list<int>::iterator it; 
	bool found;

	for(unsigned int i = 0; i < out.size(); i++){
		found = false;

		for(it = mark.begin(); it!= mark.end(); it++){
			if(*it == out[i]->getID()){
				found = true;
				break;
			}
		}

		if(!found)
			DFSlevel(out[i], mark, level+1);
	}

	mark.pop_back();
}



int Graph::DFScycle(Vertex* vertex, std::list<int>& mark){
	//printf("SEARCHING FOR AND REMOVING CYCLES\n");
	int previous = mark.back(); 
	mark.push_back(vertex->getID());

	std::vector<Vertex*> out;
	vertex->getOutput(out);
	std::list<int>::iterator it; 
	bool found;

	for(unsigned int i = 0; i < out.size(); i++){
		found = false;

		for(it = mark.begin(); it!= mark.end(); it++){
			if(*it == out[i]->getID()){
				found = true;
				break;
			}
		}

		if(!found){
			int prev = DFScycle(out[i], mark); 
			if(prev != vertex->getID()){
				mark.pop_back();
				return prev;
			}
		}
		else{

			printf("CHECKING FOR FF IN LOOP...");


			Vertex* nextV = out[i];
			Vertex* currentV = vertex;

			for(it = mark.begin(); it!= mark.end(); it++){
				if(m_GraphV[*it]->getType().find("FD") != std::string::npos){
					printf("FF EXISTS\n");	

					previous = *it;
					currentV = m_GraphV[*it];
					it++;
					nextV = m_GraphV[*it];

					break;
				}
			}

			printf("BREAKING FEEDBACK between %d - %d\n", currentV->getID(), nextV->getID());

			if(currentV->getType() == "IN")
				continue;

			//Break feedback loop, Remove output
			std::string outPortName = currentV->removeOutputValue(nextV->getID());

			int index = nextV->removeInputValue(currentV->getID());
			std::vector<std::string> outPorts;
			nextV->getInPorts(outPorts);
			nextV->addInPort(outPorts[index]);
			std::stringstream ss;
			ss<<"P" << getNumInputs();
			Vertex* inputV = addVertex(getLast(), "IN");
			addInput(ss.str(), inputV->getID());

			inputV->addOutput(nextV, outPortName);
			nextV->removeInPortValue(index);
			nextV->addInput(inputV);

			mark.pop_back();
			return previous;
		}
	}
	mark.pop_back();
	return previous;
}

/***************************************************************************
 *  
 *    Sets the order of the gates (Depth) 
 ****************************************************************************/
/*void Graph::BFSLevel(std::queue<Vertex*, std::list<Vertex*> > queue, std::map<int,bool>& mark, int level, int endLevel){

	if(queue.empty())
	return;

	Vertex* front;
	front = queue.front();
	queue.pop();

	if(mark.find(front->getID()) == mark.end()){
//printf("Node\t%d not marked. LEVEL: %d\n", front->getID(), level);
front->setLevel(level);
}
else{
if(front->getLevel() < level)
front->setLevel(level);
}

std::vector<Vertex*> output;
front->getOutput(output);

for(unsigned int i = 0; i < output.size(); i++){
queue.push(output[i]);
}

if(endLevel == 0){
level++;
endLevel = queue.size()-1;
}
else
endLevel--;

BFSLevel(queue, mark, level, endLevel);

}*/
