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
}


GraphExporter::GraphExporter() {
	fp = false;
}




GraphExporter::~GraphExporter() {
	// TODO Auto-generated destructor stub
}





bool GraphExporter::graphExport(std::string outDir, torc::generic::RootSharedPtr &rootPtr) {
	//Initial Declarations
	std::vector<torc::generic::LibrarySharedPtr, std::allocator<LibrarySharedPtr> > vLibrary;
	
	
	std::string oFile = "";
	std::string graph = "";
	std::string graphNode = "";
	std::string graphAssignment = "";

	try {
		std::cout<<"\n*********************************\n";
		std::cout<<  "Begin EDIF to Graph Translation\n\n";

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
			oFile = outDir + cell->getName() + ".dot";
			mStream.open(oFile.c_str(), std::ios::out);
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
						//instPrimitive = "IN";
					continue;

				if(simpName[instName] == "")
					instName = getSimpleName(instName, instPrimitive);
				else
					instName = simpName[instName];
				
			}

			if(!fp)
				graphNode += "\t" + instName + "[shape=record, label=\"";
			
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
								if(simpName[iName] == "")
									iName = getSimpleName(iName, iPrim); 
								else
									iName = simpName[iName];

								dotGraph = "\t" + instName+":" + ref->getName() + " -> " + 
									iName + ":" + portref->getName();
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
		
		graph += graphNode + graphAssignment +"}";
		std::cout << graph << std::endl;

		//Output contents to file
		if(mStream.is_open()){
			mStream << graph;
			mStream.close();
			if(!fp){
				std::string command = "dot -Tgif " + outDir + cell->getName() + ".dot -o" + outDir + cell->getName() + ".gif";
				std::cout<<"OUTPUT FILE: "<<outDir<<cell->getName()<<".gif"<<std::endl;
				system(command.c_str());
			}
			else{
				std::cout<<"OUTPUT FILE: "<<outDir<<cell->getName()<<".dot"<<std::endl;
			}
		}
		
		}


		std::cout << "CONVERSION COMPLETE"<< std::endl;
		std::cout<<  "*******************\n"<<std::endl;

	} catch (std::exception &e) {
		std::cout << "EXCEPTION IN exportGraph\n\n" << e.what() << std::endl << std::endl;
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
		std::cout<<instance<<std::endl;
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
	std::cout << "TRAVERSING INPUT OUTPUT PORTS....." << std::endl;
	
	//Go through each port of the cell and determine if it's input or output
	for (unsigned int l = 0; l < vPort.size(); l++) {
		Port* port = vPort.at(l).get();
		std::vector<torc::generic::NetSharedPtr> connectedNet;
		port->getConnectedNets(connectedNet);
		PortDirection pdirect = port->getDirection();

		bool isOutput = false;
		//Get the Port Names for the graph nodes. 	
		if(pdirect == ePortDirectionOut){
			graphOutput += "<" + port->getName() + "> " + port->getName() + "|";
			isOutput = true;
		}
		else if (pdirect == ePortDirectionIn){
			graphInput += "<" + port->getName() + "> " + port->getName() + "|";
			std::cout<<"GRAPH INPUT NAME: "<<graphInput<<std::endl;
		}


		std::list<std::string> prevNet;
		//Go through net to find the specific portbit 
		for(unsigned int p = 0; p < connectedNet.size(); p++){
			Net* net = connectedNet.at(p).get();
			std::cout<<"NET NAME: "<<net->getName()<<std::endl;
				
			std::string netLength = "";
			std::string iPrim = "";
			if(net->getSize() > 1)
				netLength = boost::lexical_cast<std::string>( net->getSize());

			std::vector<torc::generic::PortReferenceSharedPtr> refTemp;
			net->getConnectedPortRefs(refTemp);

			//Find the port that is connected to the instance
			for(unsigned q = 0; q < refTemp.size(); q++){
				PortReference* portref = refTemp.at(q).get();
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
				if ((isOutput && ( portDirection == ePortDirectionOut)) || (!isOutput && (portDirection == ePortDirectionIn))){
					std::string dotGraph = "";	
					
					if(fp){
						if(simpName[iName] == "")
							iName = getSimpleName(iName, iPrim); 
						else
							iName = simpName[iName];
					}

					//Depending on Input or output, arrange nodes. 
					if(portDirection == ePortDirectionIn){
						std::string portInName;
						if(fp){
							
							if(simpName[net->getName()] == "")
								portInName = getSimpleName(net->getName(), "IN"); 
							else
								portInName = simpName[net->getName()];
							
							dotGraph = "\t" + portInName + ":O -> " + 
								iName + ":" + portref->getName();

						}
						else{
							portInName = port->getName();
							dotGraph = "\t" + portInName + " -> " + 
								iName + ":" + portref->getName();
						}
					}
					else
						//Skip output ports (FEEDBACK LOOP)
						if(fp){
							//dotGraph = "\t" + iName + ":" + portref->getName() + " -> " + port->getName();
							continue;
						}
						else
							dotGraph = "\t" + iName + ":" + portref->getName() + "->" + port->getName();
						
					

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
	return graphAssignment;
}



std::string GraphExporter::getSimpleName(std::string name, std::string simpleName){
	unsigned int in;
	if((in = simpleName.rfind("__")) != std::string::npos)
		simpleName = simpleName.substr(0, in);
	int count = instNum[simpleName];
	instNum[simpleName] = ++count;
								
	std::stringstream ss;
	ss<<simpleName<<"__"<<count;
	simpName[name] = ss.str();
	printf("OLD: %s\tNEW: %s\n", name.c_str(),  ss.str().c_str());

	return ss.str();
}
} //namespace
} //namespace
