/*****************************************************************
**************		GraphExporter.cpp		**********************
*
*				Parses through EDIF and Converts
*				to Graph
*
*				@Author: 	kaiwen
*				@Last Edit:	2012-01-06
*				@Started:	2011-09-02
*
*****************************************************************/


#include "torc/generic/GraphExporter.hpp"
#include <sstream>

namespace torc {
namespace generic {


GraphExporter::GraphExporter(std::string option) {
	if(option == "-fp")
		fp = true;

	ckt = new Graph("circuit");
	lastVID = 0; 
}


GraphExporter::GraphExporter() {
	fp = false;
}




GraphExporter::~GraphExporter() {
	// TODO Auto-generated destructor stub
	delete ckt;
}





bool GraphExporter::graphExport(std::string outDir, std::string filename,  torc::generic::RootSharedPtr &rootPtr) {
	//Initial Declarations
	std::vector<torc::generic::LibrarySharedPtr, std::allocator<LibrarySharedPtr> > vLibrary;
	
	
	std::string oFile = "";
	std::string gFile = "";
	std::string graph = "";
	std::string graphNode = "";
	std::string graphAssignment = "";

	try {

		//Get library of interest
		rootPtr->getLibraries(vLibrary);
		Library* lib = vLibrary.at(vLibrary.size() - 1).get();
		std::vector<torc::generic::CellSharedPtr> vCell;
		lib->getCells(vCell);


		//Get cell of interest
		for(unsigned int i = 0; i < vCell.size(); i++){
		graph = "";
		graphNode = "";
		graphAssignment = "";	
		Cell* cell = vCell.at(i).get();
		std::vector<torc::generic::ViewSharedPtr> vViews;
		cell->getViews(vViews);

		View* view = vViews.at(0).get();

		//Get instances
		std::vector<torc::generic::InstanceSharedPtr> vInst;
		view->getInstances(vInst);
		

		if(vInst.size() > 0){
			//Get the module name from cell
			oFile = outDir + filename + ".dot";
			gFile = outDir + filename + ".g";
			//mStream.open(oFile.c_str(), std::ios::out);
			graph = "digraph edif {\n";
			if(!fp)
				graph += "\trankdir=LR;\n\tedge[weight=10]\n";
			graphAssignment += getPorts(view);
		}
		else
			continue;
				
		//Go through each Instance and write out the instantiation of each module
		for (unsigned int l = 0; l < vInst.size(); l++) {
			std::string graphInput = "";
			std::string graphOutput= "";

			Instance* inst = (vInst.at(l).get());

			//Get the primitive name for the instance (Cellref)
			std::string instPrimitive = inst->getMaster()->getParent()->getName();

			//Gets the instance name
			std::string instName = inst->getName();

			//Get the port reference of the instance
			std::vector<torc::generic::PortReferenceSharedPtr> vRef;
			inst->getPortReferences(vRef);

			if(fp){
				if(instPrimitive == "VCC" || instPrimitive == "GND")
					instName = instPrimitive;
					/*
				else if(simpName[instName] == "")
					instName = getSimpleName(instName, instPrimitive);
				else
					instName = simpName[instName];
					*/
			}
	
	





			//Get LUT INIT VALUE
			//printf("INST NAME: %s\n", instName.c_str());
			PropertySharedPtr initPropertyPtr = inst->getProperty("INIT");
	
			if(initPropertyPtr != NULL){
				if(instPrimitive.find("LUT") != std::string::npos){
					std::string property = initPropertyPtr->getValue().get<Value::String>();
					//graphNode += "\t" + instName + "[label=\"" + property + "\"];\n";
					//graphNode += "\t" + instName + "[lut=$" + property + "$];\n";
					nameLUTMap[instName] = property;
				}
			}
	





			
			std::list<std::string> prevNet;
			//Go through each port and find the net that connects to that port
			for (unsigned int m = 0; m < vRef.size(); m++) {
				PortReference* ref = vRef.at(m).get();
				
				//Get the name of the port reference
				std::string refName = ref->getName();
					
				std::vector<torc::generic::NetSharedPtr> connectedNet;
				ref->getConnectedNets(connectedNet);
			
				bool isOutput = false;
				//Get the Port Names for the graph nodes. 	
				if(ref->getMaster()->getDirection() == ePortDirectionOut){
					graphOutput += "<" + refName + "> " + refName + "|";
					isOutput = true;
				}
				else if (ref->getMaster()->getDirection() == ePortDirectionIn){
					graphInput += "<" + refName + "> " + refName + "|";
				}


				//Go through net to find the specific portbit 
				for(unsigned int p = 0; p < connectedNet.size(); p++){
					Net* net = connectedNet.at(p).get();
					//std::cout<<"NET NAME "<<net->getName()<<" "<<net->getSize()<<std::endl;
					std::string netLength = "";
					if(net->getSize() > 1)
						netLength = boost::lexical_cast<std::string>( net->getSize());

					std::vector<torc::generic::PortReferenceSharedPtr> refTemp;
					net->getConnectedPortRefs(refTemp);
					
					//Find the port that is connected to the instance
					for(unsigned q = 0; q < refTemp.size(); q++){
						PortReference* portref = refTemp.at(q).get();
						CompositionType comType = portref->getCompositionType();
						std::string iName = "";
						std::string iPrim = "";
						PortDirection portDirection;
						if(comType == eCompositionTypeVectorBit){
							VectorPortBitReference* vectorPort = (VectorPortBitReference*)(&*portref);
							portDirection = vectorPort->getParentCollection()->getMaster()->getDirection();
							iName = vectorPort->getParentCollection()->getParent()->getName();
							iPrim = vectorPort->getParentCollection()->getParent()->getMaster()->getParent()->getName();
						}
						else{
							portDirection = portref->getMaster()->getDirection();
							iName = portref->getParent()->getName();
							iPrim = portref->getParent()->getMaster()->getParent()->getName();
						}
						//Check to see if the direction is output to input. If so, assign a edge
						//Used to prevent multiple edges between same nodes. (SINGLE DIRECTION)
						if (isOutput && ( portDirection == ePortDirectionIn)){
							std::string dotGraph = "\t" + instName + ":" + ref->getName() + " -> " + 
								iName + ":" + portref->getName();
							
							if(fp){
							/*
								if(simpName[iName] == "")
									iName = getSimpleName(iName, iPrim); 
								else
									iName = simpName[iName];
									*/

								dotGraph = "\t" + instName+ ":" +ref->getName() + " -> " + 
									iName + ":" + portref->getName();

							Vertex* source;
							if(nameIDMap.find(instName) == nameIDMap.end()){
								source = new Vertex(lastVID, instPrimitive, instName);
								nameIDMap[instName] = source;
								ckt->addVertex(source);

								lastVID++;
							}
							else source = nameIDMap[instName];
							
							
							Vertex* sink;
							if(nameIDMap.find(iName) == nameIDMap.end()){
								sink = new Vertex(lastVID, iPrim, iName);
								nameIDMap[iName] = sink;
								ckt->addVertex(sink);

								lastVID++;
							}
							else sink = nameIDMap[iName];

							//Source connections
							source->addOutput(sink, ref->getName());

							//Sink Connection
							sink->addInput(source);
							sink->addInPort(portref->getName());

							}

							//Check to see if edge already exists
							bool isPrev = false;
							for(std::list<std::string>::iterator it = prevNet.begin(); it != prevNet.end(); ++it){
								if(*it == dotGraph){
									isPrev = true;
									break;
								}
							}
	
							//Break if edge exists
							if(isPrev)
								break;
							else
								prevNet.push_back(dotGraph);

							if(netLength != "")
								graphAssignment += dotGraph + "[color=blue, label=\"" + netLength+ "\", style=bold]\n";
							else{
								graphAssignment += dotGraph + "\n";
							}
						}
					
					}
				}
			}


			graphOutput = "{" +  trim(graphOutput) + "}" ;
			graphInput = "{" + trim(graphInput) + "}";
			if(instName.length() > 15)
				instName = instName.substr(0,15);

			if(!fp)	
				graphNode += "{" + graphInput + "|" + instPrimitive + "\\n" + instName + "|" + graphOutput + "}\"]\n"; 
		}
		
		
		std::map<std::string, std::string>::iterator iMap;
		for(iMap = nameLUTMap.begin(); iMap != nameLUTMap.end(); iMap++){
			nameIDMap[iMap->first]->setLUT(iMap->second);
		}
		
		//ckt->print();
		ckt->exportGraph(gFile);

		
		graph += graphNode + graphAssignment +"}";
		//std::cout << graph << std::endl;

		//Output contents to file
		/*
		if(mStream.is_open()){
			mStream << graph;
			mStream.close();
			if(!fp){
				std::string command = "dot -Tgif " + outDir + cell->getName() + ".dot -o" + outDir + cell->getName() + ".gif";
				std::cout<<"[E2G] -- OUTPUT FILE: "<<outDir<<cell->getName()<<".gif"<<std::endl;
				system(command.c_str());
			}
			else{
			//	std::cout<<"[E2G] -- OUTPUT FILE: "<<oFile<<std::endl;
			}
		}
		*/
		
		}


		std::cout << "CONVERSION COMPLETE"<< std::endl;

	} catch (std::exception &e) {
		std::cout << "[E2G] -- EXCEPTION IN exportGraph\n\n" << e.what() << std::endl << std::endl;
		return false;
	}

	return true;
}



bool GraphExporter::checkComponent(std::string instance, std::string& pinAssignment, Library* lib, Instance* inst){

	PropertySharedPtr initPropertyPtr = inst->getProperty("PIN");

	if(initPropertyPtr != NULL){
		std::string property = initPropertyPtr->getValue().get<Value::String>();

		std::vector<torc::generic::PortSharedPtr> directPortList;
		//getCellPortDirection(lib, instance, directPortList);
		Port* directPort = directPortList.at(0).get();
		//std::cout<<instance<<std::endl;
		std::string direction = "\toutput ";
		if(directPort->getDirection() == ePortDirectionOut)
			direction = "\tinput ";
		else if(directPort->getDirection() == ePortDirectionInOut)
			direction = "\tinput ";

		std::string size = "";
		if(directPortList.size() > 1){
			std::string indexVal = boost::lexical_cast<std::string>(directPortList.size()-1);
			size = "[" + indexVal + ":0] ";
		}
		pinAssignment = direction + size + instance +  " /* synthesis altera_chip_pin_lc=\"" + property + "\"*/ ;\n";

	}
	else
		return false;

	return true;

}




std::string GraphExporter::trim(std::string str){
	if(str == "")
		return "";
	else	
		return str.erase(str.length() - 1, 1);

}





std::string GraphExporter::getPorts(View* view){

	std::vector<torc::generic::PortSharedPtr> vPort;
	view->getPorts(vPort);

	std::string graphInput = "";
	std::string graphOutput= "";
	std::string graphAssignment = "";
	//std::cout << "TRAVERSING INPUT OUTPUT PORTS....." << std::endl;
	
	//Go through each port of the cell and determine if it's input or output
	for (unsigned int l = 0; l < vPort.size(); l++) {
		Port* port = vPort.at(l).get();
		PortDirection pdirect = port->getDirection();

		printf("PORT NAME: %s - \n", port->getName().c_str());
		bool isOutput = false;
		//Get the Port Names for the graph nodes. 	
		if(pdirect == ePortDirectionOut){
			graphOutput += "<" + port->getName() + "> " + port->getName() + "|";
			isOutput = true;
			printf("output\n");
		}
		else if (pdirect == ePortDirectionIn){
			graphInput += "<" + port->getName() + "> " + port->getName() + "|";
			//std::cout<<"GRAPH INPUT NAME: "<<graphInput<<std::endl;
			printf("input\n");
		}
		else
			printf(" * Unknown Port Direction\n");
				
				
				CompositionType compType = port->getCompositionType();
				std::vector<torc::generic::NetSharedPtr> connectedNet;
				if(compType == eCompositionTypeVector){
					printf("VECTOR PORT\n");
					//VectorPortReference* vectorPort = (VectorPortReference*)(&*port);
					//vectorPort->getConectedNets(connectedNet);
				}
				else{
					printf("Single PORT\n");
					printf("\tNAME: %s\n", port->getName().c_str());
					port->getConnectedNets(connectedNet);
				}
				printf("\n\n");


		std::list<std::string> prevNet;
		//Go through net to find the specific portbit 
		for(unsigned int p = 0; p < connectedNet.size(); p++){
			Net* net = connectedNet.at(p).get();
			//std::cout<<"NET NAME: "<<net->getName()<<std::endl;
				
			std::string netLength = "";
			std::string iPrim = "";
			if(net->getSize() > 1)
				netLength = boost::lexical_cast<std::string>( net->getSize());

			std::vector<torc::generic::PortReferenceSharedPtr> refTemp;
			net->getConnectedPortRefs(refTemp);

			//Find the port that is connected to the instance
			for(unsigned q = 0; q < refTemp.size(); q++){

				PortReference* portref = refTemp.at(q).get();
				
				//Output only has one input
				if(isOutput){
					if(portref->getName() == port->getName())
						continue;
				}

				CompositionType comType = portref->getCompositionType();
				std::string iName = "";
				PortDirection portDirection;
				if(comType == eCompositionTypeVectorBit){
					VectorPortBitReference* vectorPort = (VectorPortBitReference*)(&*portref);
					portDirection = vectorPort->getParentCollection()->getMaster()->getDirection();
					iName = vectorPort->getParentCollection()->getParent()->getName();
					iPrim = vectorPort->getParentCollection()->getParent()->getMaster()->getParent()->getName();
				}
				else{
					portDirection = portref->getMaster()->getDirection();
					iName = portref->getParent()->getName();
					iPrim = portref->getParent()->getMaster()->getParent()->getName();
				}

				//Check to see if the direction is output to input. If so, assign a edge
				//Used to prevent multiple edges between same nodes. (SINGLE DIRECTION)
				if ((!isOutput && (portDirection == ePortDirectionIn))){
					std::string dotGraph = "";	

/*
					if(fp){
						if(simpName[iName] == "")
							iName = getSimpleName(iName, iPrim); 
						else
							iName = simpName[iName];
					}
					*/

					//Depending on Input or output, arrange nodes. 
						std::string portInName;
						if(fp){
							
							if(simpName[port->getName()] == "")
								portInName = getSimpleName(port->getName(), "IN"); 
							else
								portInName = simpName[port->getName()];
							
							dotGraph = "\t" + portInName +  ":O -> " + 
								iName + ":" + portref->getName();

							Vertex* source;
							if(nameIDMap.find(port->getName()) == nameIDMap.end()){
								source = new Vertex(lastVID, "IN", port->getName());
								nameIDMap[port->getName()] = source;
								ckt->addVertex(source);
								ckt->addInput(port->getName(), lastVID);

								lastVID++;
							}
							else source = nameIDMap[port->getName()];
							
							
							Vertex* sink;
							if(nameIDMap.find(iName) == nameIDMap.end()){
								sink = new Vertex(lastVID, iPrim, iName);
								nameIDMap[iName] = sink;
								ckt->addVertex(sink);

								lastVID++;
							}
							else sink = nameIDMap[iName];

							//Source connections
							source->addOutput(sink, "O");

							//Sink Connection
							sink->addInput(source);
							sink->addInPort(portref->getName());

						}
						else{
							portInName = port->getName();
							dotGraph = "\t" + portInName + " -> " + 
								iName + ":" + portref->getName();
						}
						
					

				}
				else if ((isOutput && (portDirection == ePortDirectionOut))){
						//Skip output ports (FEEDBACK LOOP)
						if(fp){
							/*
							//TODO: Have output as an actual node
							std::string portOutName;
							//dotGraph = "\t" + iName + ":" + portref->getName() + " -> " + port->getName();
							if(simpName[port->getName()] == "")
								portOutName = getSimpleName(port->getName(), "OUT"); 
							else
								portOutName = simpName[port->getName()];
							
							dotGraph = "\t" + iName + ":" + portref->getName() + " -> "  + 
								portOutName + ":I";
								//continue;
								*/
							//std::cout<<"OUTPUT! NAME: "<<iName<<std::endl;
							Vertex* source;
							if(nameIDMap.find(iName) == nameIDMap.end()){
								source = new Vertex(lastVID, iPrim, iName);
								nameIDMap[iName] = source;
								ckt->addVertex(source);
								ckt->addOutput(port->getName(), lastVID);

								lastVID++;
							}	
							else{
								ckt->addOutput(port->getName(), nameIDMap[iName]->getID());
							}
						}
						//jJelse
							//dotGraph = "\t" + iName + ":" + portref->getName() + "->" + port->getName();
			
				}
			}
		}
	}
	//ckt->print();
	return graphAssignment;
}



std::string GraphExporter::getSimpleName(std::string name, std::string simpleName){
	int count = instNum[simpleName];
	instNum[simpleName] = ++count;
								
	std::stringstream ss;
	ss<<simpleName<<"---"<<count<<"---"<<name;
	simpName[name] = ss.str();
	//printf("OLD: %s\tNEW: %s\n", name.c_str(),  ss.str().c_str());

	return ss.str();
}
} //namespace
} //namespace
