/*****************************************************************
 **************		EDIF To Custom Netlist Translator ************
 *
 *				Parses through EDIF and Converts
 *				to Verilog
 *
 *				@Author: 	kaiwen
 *				@Last Edit:	2011-10-28
 *				@Started:	2011-09-02
 *
 *****************************************************************/

#include "torc/Generic.hpp"
#include "torc/generic/GraphExporter.hpp"
#include <fstream>
#include <map>
#include <boost/regex.hpp>
#include "torc/generic/graph.hpp"
#include "torc/generic/vertex.hpp"

using namespace std;
using namespace torc::generic;

bool translate(std::string name, std::string outDir, RootSharedPtr &rootPtr);
void exportCell(std::string outDir, Cell* module, std::map<std::string, std::string>&);
void writetoNLV(std::string filePath, std::map<std::string, std::string>&);
Graph* ckt;

//OutPort, port of inst connected to outport
std::map<std::string, std::string> port2OutPortMap;

int main(int argc, char* argv[]) {
	if(argc != 3){
		std::cout<<"\n################################################################\n"<<std::endl;
		printf("    INVALID ARGUMENTS: ./ndf2nlv <EDIF FILE> <Out DIR>\n");
		std::cout<<"\n################################################################\n"<<std::endl;
		return 0 ;
	}	

	try{
		std::cout<<"\n============================================================================\n";
		std::cout<<  "[*]\tBegin EDIF to NLV translation\n";
		std::cout<<"============================================================================\n";


		std::string fileName = argv[1];
		std::string outDir= argv[2];


		std::cout<<"[NDF2NLV] -- EDIF FILE: "<<fileName<<endl;

		// import the EDIF design
		fstream fileStream(fileName.c_str());
		ObjectFactorySharedPtr factoryPtr(new ObjectFactory());
		EdifImporter importer(factoryPtr);
		importer(fileStream, fileName);
		RootSharedPtr rootPtr = importer.getRootPtr();

		//Grab File Name
		size_t startPos = fileName.find_last_of('/')+1;
		size_t endPos = fileName.find_last_of('.');
		std::string name= fileName.substr(startPos, endPos-startPos);

		//Export design by passing rootPtr of the imported EDIF File
		translate(name, outDir, rootPtr);
	}
	catch(exception& e){
		cout<<"[ndf2cnl] -- EXCEPTION ON MAIN"<<endl;
	}

	return 0;
}






bool translate(std::string name, std::string outDir, torc::generic::RootSharedPtr &rootPtr) {
	//printf("[Translate] -- Translating EDIF File...\n");
	//Initial Declarations
	std::vector<torc::generic::LibrarySharedPtr, std::allocator<LibrarySharedPtr> > vLibrary;

	try {
		//Get Current Design
		std::vector<DesignSharedPtr> designList;
		rootPtr->getDesigns(designList);

		//Make sure only one design (May Change)
		assert(designList.size() == 1);
		//std::cout<<"Number of Designs: "<<designList.size()<<std::endl;

		//Get the library
		Design* topDesign = designList.at(0).get();
		Library* library = rootPtr->findLibrary(topDesign->getLibraryRefName()).get();


		//Get the cells in the library
		std::vector<torc::generic::CellSharedPtr> cells;
		library->getCells(cells);

		for(unsigned int i = 0; i < cells.size(); i++){
			//Get the top cell
			Cell* cell = cells[i].get();

			//Check to see if there are any instances
			std::vector<torc::generic::ViewSharedPtr> vViews;
			std::vector<torc::generic::InstanceSharedPtr> instances;
			cell->getViews(vViews);
			View* view = vViews.at(0).get();
			view->getInstances(instances);
			if(instances.size() == 0) continue;



			std::string filepath;

			//Note the submodules
			if(cell->getName() != topDesign->getCellRefName())
				filepath += outDir + cell->getName()  + "_cell";
			else
				filepath += outDir + name;

			filepath+= ".nlv";

			//export each cell
			ckt = new Graph(name);
			std::map<std::string, std::string> primMap;
			exportCell(name, cell, primMap);
			writetoNLV(filepath, primMap);
			delete ckt;
		}


		std::cout << "[NDF2NLV] -- CONVERSION COMPLETE"<< std::endl;

	} catch (std::exception &e) {
		std::cout << "[NDF2NLV] -- EXCEPTION IN exportGraph\n\n" << e.what() << std::endl << std::endl;
		return false;
	}

	return true;
}


void exportCell(std::string filepath, Cell* module, std::map<std::string, std::string>& primMap){
	std::cout<<"[NDF2NLV] -- Exporting Cell: "<<filepath<<"\n";
	std::vector<torc::generic::ViewSharedPtr> vViews;
	module->getViews(vViews);
	View* view = vViews.at(0).get();


	//Get instances ports and nets
	std::vector<torc::generic::InstanceSharedPtr> instances;
	std::vector<torc::generic::PortSharedPtr> ports;
	std::vector<torc::generic::NetSharedPtr> nets;
	view->getInstances(instances);
	view->getPorts(ports);
	view->getNets(nets);

	//Name, VID
	std::map<std::string, Vertex*> nameVertexMap;
	std::map<std::string, Vertex*>::iterator iNVM;

	//Instance name, LUT function
	std::map<std::string, std::string> instanceLUTMap;

	/*********************************

		Parsing ports to the module

	 ***********************************/
	int lastVID = 0;
	for (unsigned int i = 0; i < ports.size(); i++) {
		Port* port = ports[i].get();
		CompositionType comType = port->getCompositionType();

		if(comType == eCompositionTypeVector){
			//printf("PORT IS VECTOR\n");
			continue;
		}

		if(port->getDirection() == ePortDirectionIn){
			//std::cout<<"DIR: Input \tID: "<<lastVID<<"\tPort: "<<port->getName()<<"\n";

			Vertex* source;
			source = new Vertex(lastVID, "IN", port->getName());
			nameVertexMap[port->getName()] = source;
			ckt->addVertex(source);
			ckt->addInput(port->getName(), lastVID);

			lastVID++;
		}
		else if(port->getDirection() == ePortDirectionOut){
			//std::cout<<"DIR: Output\tPort:  "<< port->getName()<< "\n";
			ckt->addOutput(port->getName(), -1);
		}
	}


	/*********************************

		Parsing Instances to the module

	 ***********************************/
	for (unsigned int i = 0; i < instances.size(); i++) {
		Instance* instance= instances[i].get();
		std::string type = instance->getMaster()->getParent()->getName();
		std::string name = instance->getName();
		//std::cout<<"VID:"<<lastVID<<"\tInstance: "<< name << "\tType: "<< type <<"\n";

		Vertex* source;
		source = new Vertex(lastVID, type, name);
		nameVertexMap[name] = source;
		ckt->addVertex(source);


		//Record the function of the LUT
		if(type.find("LUT") != std::string::npos){
			PropertySharedPtr property= instance->getProperty("INIT");

			std::string lutFunction = property->getValue().get<Value::String>();
			source->setLUT(lutFunction);
		}

		lastVID++;
	}
	//printf("CKT SIZE: %d\n", ckt->getNumVertex());

	/*********************************

		Parsing Nets to the module

	 ***********************************/
	std::map<std::string, int> vectorPortCountMap;

	//prim, prim symbol 
	for (unsigned int i = 0; i < nets.size(); i++) {

		Net* net= nets[i].get();
		std::string name = net->getName();
		//jstd::cout<<"\n\nNET: "<<name<<std::endl;

		std::vector<torc::generic::PortReferenceSharedPtr> portRefs;
		std::vector<torc::generic::PortSharedPtr> netports;
		net->getConnectedPortRefs(portRefs);
		net->getConnectedPorts(netports);

		if(portRefs.size() + netports.size() < 2){
			continue;
		}

		/*********************************

			Find Source vertex	

		 ***********************************/
		Vertex* source; 
		std::string sourcePortName = "";
		bool isSourceFound = false;
		for(unsigned k = 0; k < portRefs.size(); k++){
			PortReference* portref = portRefs.at(k).get();

			std::string primName = portref->getParent()->getMaster()->getParent()->getName();
			std::string primSymbol = "load symbol " + primName + " v1 ";
			if(primName.find("MX") != std::string::npos || 
					primName.find("MUX") != std::string::npos)
				primSymbol+= " MUX";
			else if(primName.find("NAND") != std::string::npos || 
					primName.find("Nand") != std::string::npos)
				primSymbol+= " NAND";
			else if(primName.find("AND") != std::string::npos || 
					primName.find("And") != std::string::npos)
				primSymbol+= " AND";
			else if(primName.find("NOR") != std::string::npos || 
					primName.find("Nor") != std::string::npos)
				primSymbol+= " NOR";
			else if(primName.find("XOR") != std::string::npos || 
					primName.find("Xor") != std::string::npos)
				primSymbol+= " XOR";
			else if(primName.find("OR") != std::string::npos || 
					primName.find("Or") != std::string::npos)
				primSymbol+= " OR";
			else if(primName.find("NOT") != std::string::npos || 
					primName.find("INV") != std::string::npos)
				primSymbol+= " INV";
			else if(primName.find("BUF") != std::string::npos || 
					primName.find("Buf") != std::string::npos)
				primSymbol+= " BUF";
			else if(primName.find("FD") != std::string::npos || 
					primName.find("FF") != std::string::npos ||
					primName.find("LAT") != std::string::npos)
				primSymbol+= " GEN";
			else
				primSymbol+= " BOX";


			std::vector<torc::generic::PortSharedPtr> primPorts;
			portref->getParent()->getMaster()->getPorts(primPorts);
			for (unsigned int i = 0; i < primPorts.size(); i++) {
				Port* port = primPorts[i].get();
				CompositionType comType = port->getCompositionType();

				if(comType == eCompositionTypeVector)
					continue;

				if(port->getDirection() == ePortDirectionIn)
					primSymbol += " \\\n\tport " + port->getName() + " in.left";
				else if(port->getDirection() == ePortDirectionOut)
					primSymbol += " \\\n\tport " + port->getName() + " out.right";
			}

			primMap[primName] = primSymbol;

			if(portref->getMaster()->getDirection() == ePortDirectionOut){
				std::string instanceRef = portref->getParent()->getName();
				source = nameVertexMap[instanceRef];
				sourcePortName = portref->getName();
				isSourceFound = true;
				break;
			}
		}

		if(!isSourceFound)
			for(unsigned k = 0; k < netports.size(); k++){
				Port* port = netports.at(k).get();
				//std::cout<<"\tPort:  "<<port->getName()<<"\t"<<"DIR: "<<port->getDirection()<<"\n";

				CompositionType comType = port->getCompositionType();
				std::string portName = port->getName();
				PortDirection portDirection= port->getDirection();

				if(comType == eCompositionTypeVectorBit){
					VectorPortBit* vectorPort = (VectorPortBit*)(&*port);
					//std::cout<<"\tVPort:  "<<vectorPort->getName()<<"\t";
					//std::cout<<"DIR: "<<vectorPort->getParentCollection()->getDirection()<<"\n";

					portName = vectorPort->getName();
					portDirection = vectorPort->getParentCollection()->getDirection();

					if(portDirection == ePortDirectionIn){
						int index = 0;
						if(vectorPortCountMap.find(portName) == vectorPortCountMap.end()){
							vectorPortCountMap[portName] = 1;
						}
						else{
							index = vectorPortCountMap[portName];
							vectorPortCountMap[portName]++;
						}
						std::stringstream ss;
						ss<<portName<<"_"<<index;
						portName = ss.str();
						//					printf("NEW PORT NAME: %s\n", portName.c_str());

						Vertex* newInput;
						newInput = new Vertex(lastVID, "IN", portName);
						nameVertexMap[portName] = newInput;
						ckt->addVertex(newInput);
						ckt->addInput(portName, lastVID);
						lastVID++;
					}

				}

				if(portDirection == ePortDirectionIn){
					source = nameVertexMap[portName];
					sourcePortName = "O";
					isSourceFound = true;
					break;
				}
			}



		assert(isSourceFound);




		/*********************************

			Connect source to the sinks	

		 ***********************************/
		for(unsigned k = 0; k < portRefs.size(); k++){
			PortReference* portref = portRefs.at(k).get();
			//std::cout<<"\tPortRef:  "<<portref->getName()<<" Instance: "<<portref->getParent()->getName()<<std::endl;

			//TODO: Check for buses. Ignore buses for now...
			CompositionType comType = portref->getCompositionType();
			assert(comType != eCompositionTypeVectorBit);

			std::string instanceRef = portref->getParent()->getName();
			if(portref->getMaster()->getDirection() == ePortDirectionIn){
				Vertex* sink = nameVertexMap[instanceRef];
				sink->addInput(source);
				sink->addInPort(portref->getName());

				source->addOutput(sink, sourcePortName);
			}
		}



		/*********************************

			Check nets connected to PORTS 

		 ***********************************/
		for(unsigned k = 0; k < netports.size(); k++){
			Port* port = netports.at(k).get();

			//TODO: Check for buses. Ignore buses for now...
			CompositionType comType = port->getCompositionType();
			std::string portName = port->getName();
			PortDirection portDirection= port->getDirection();

			if(comType == eCompositionTypeVectorBit){
				VectorPortBit* vectorPort = (VectorPortBit*)(&*port);
				//	std::cout<<"\tVPort:  "<<vectorPort->getName()<<"\t";
				//	std::cout<<"DIR: "<<vectorPort->getParentCollection()->getDirection()<<"\n";

				portName = vectorPort->getName();
				portDirection = vectorPort->getParentCollection()->getDirection();

				if(portDirection == ePortDirectionOut){
					int index = 0;
					if(vectorPortCountMap.find(portName) == vectorPortCountMap.end()){
						vectorPortCountMap[portName] = 1;
					}
					else{
						index = vectorPortCountMap[portName];
						vectorPortCountMap[portName]++;
					}
					std::stringstream ss;
					ss<<portName<<"_"<<index;
					portName = ss.str();
					//	printf("NEW PORT NAME: %s\n", portName.c_str());
				}
			}

			if(portDirection == ePortDirectionOut){
				ckt->addOutput(portName, source->getID());
				port2OutPortMap[portName] = sourcePortName;
			}
		}
	}



	//fs<<"\nEND";
}





void writetoNLV(std::string filePath, std::map<std::string, std::string>& primMap){
	std::string nlv = "";

	//Module name
	nlv += "module new " + ckt->getName() + "\n\n";
	std::map<std::string, std::string>::iterator iMapS;
	for(iMapS = primMap.begin(); iMapS != primMap.end(); iMapS++){
		nlv+=iMapS->second + "\n";
	}

	//Get the ports
	std::map<std::string, int> input;
	std::map<std::string, int> output;
	std::map<std::string, int>::iterator iMap;
	ckt->getInputs(input);
	for(iMap = input.begin(); iMap != input.end(); iMap++)
		nlv += "load port " + iMap->first + " in\n";
	
	ckt->getOutputs(output);
	for(iMap = output.begin(); iMap != output.end(); iMap++)
		nlv += "load port " + iMap->first + " out\n";

	
	std::string inst = "";
	std::string net = "";
	
	std::map<int, Vertex*>::const_iterator it;
	for(it = ckt->begin(); it != ckt->end(); it++){
		if(it->second->getType() != "IN" &&
				it->second->getType() != "VCC" &&
				it->second->getType() != "GND")
			inst += "load inst " + it->second->getName() + " " + it->second->getType() + " v1\n";

		std::map<std::string, std::vector<Vertex*> > out;
		std::map<std::string, std::vector<Vertex*> >::iterator iOut;
		it->second->getOutput(out);

		//Check to see if it is connected to output Port
		if(out.size() == 0){
			std::string onode = ckt->findOutPortName(it->first);
			if(onode != ""){
				net+= "load net " + it->second->getName() + " ";
				net+= "-port " + onode + " " ;
				net+="\\\n\t";

				if(it->second->getType() == "IN") 
					net+= "-port " + it->second->getName() + " " ;
				else if(it->second->getType() == "VCC") 
					net+= "-power ";
				else if(it->second->getType() == "GND")
					net+= "-ground ";
				else{
					net+= "-pin ";
					net+= it->second->getName() + " " + port2OutPortMap[onode] + " " ;
				}
				net+="\n";
				continue;
			}
			else
				continue;
		}

		for(iOut = out.begin(); iOut != out.end(); iOut++){
			net+= "load net " + it->second->getName() + " ";
			if(it->second->getType() == "VCC") 
				net+= "-power ";
			else if(it->second->getType() == "GND")
				net+= "-ground ";
			else if(it->second->getType() == "IN") 
				net+= "-port " + it->second->getName() + " " ;
			else{
				net+= "-pin ";
				net+= it->second->getName() + " " + iOut->first + " " ;
			}

			net+="\\\n\t";

			//Check to see if the output of this port goes to the output of the circuit
			std::string onode = ckt->findOutPortName(it->first);
			if(onode != "")
				net+= "-port " + onode + " " ;

			for(unsigned int i = 0; i < iOut->second.size(); i++){
					net+= "-pin ";
					net+= iOut->second[i]->getName() + " " + iOut->second[i]->getInputPortName(it->first) + " " ;
			}
			net+="\n";
		}

	}

	std::ofstream ofs(filePath.c_str(), std::ofstream::out);
	ofs<<nlv;
	ofs<<inst;
	ofs<<net;
	ofs<<"\nshow\nfullfit\n";

	ofs.close();

}



