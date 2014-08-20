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

using namespace std;
using namespace torc::generic;

bool translate(std::string outDir, RootSharedPtr &rootPtr);
void exportCell(std::string outDir, Cell* module);

int main(int argc, char* argv[]) {
	if(argc != 2){
		std::cout<<"\n################################################################\n"<<std::endl;
		printf("    INVALID ARGUMENTS: ./ndf2cnl <EDIF FILE>\n");
		std::cout<<"\n################################################################\n"<<std::endl;
		return 0 ;
	}	

	try{
		std::cout<<"\n============================================================================\n";
		std::cout<<  "[*]\tBegin EDIF to Custom Netlist Translation\n";
		std::cout<<"============================================================================\n";

		std::string fileName = argv[1];

		//OUTPUT FILES WE ARE WORKING WITH
		std::string dir = "";
		size_t index = fileName.find_last_of("/");

		if(index != std::string::npos)
			dir = fileName.substr(0, index+1);

		std::cout<<"[NDF2CNL] -- EDIF FILE: "<<fileName<<endl;

		// import the EDIF design
		fstream fileStream(fileName.c_str());
		ObjectFactorySharedPtr factoryPtr(new ObjectFactory());
		EdifImporter importer(factoryPtr);
		importer(fileStream, fileName);
		RootSharedPtr rootPtr = importer.getRootPtr();

		//Export design by passing rootPtr of the imported EDIF File
		translate(dir, rootPtr);
	}
	catch(exception& e){
		cout<<"[ndf2cnl] -- EXCEPTION ON MAIN"<<endl;
	}

	return 0;
}






bool translate(std::string outDir, torc::generic::RootSharedPtr &rootPtr) {
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

		printf("[NDF2CNL] -- Possible submodules: %d\n", (int)cells.size()-1);
		for(unsigned int i = 0; i < cells.size(); i++){
			//Get the top cell
			Cell* cell = cells[i].get();
			std::string filepath = outDir + cell->getName();

			//Note the submodules
			if(cell->getName() != topDesign->getCellRefName())
				filepath += "_cell";

			filepath+= ".cnl";

			//export each cell
			exportCell(filepath, cell);
		}


		std::cout << "CONVERSION COMPLETE"<< std::endl;

	} catch (std::exception &e) {
		std::cout << "[NDF2CNL] -- EXCEPTION IN exportGraph\n\n" << e.what() << std::endl << std::endl;
		return false;
	}

	return true;
}


void exportCell(std::string filepath, Cell* module){
		std::cout<<"[NDF2CNL] -- Exporting Cell: "<<filepath<<"\n";
		std::vector<torc::generic::ViewSharedPtr> vViews;
		module->getViews(vViews);
		View* view = vViews.at(0).get();
	

		//Get instances
		std::vector<torc::generic::InstanceSharedPtr> instances;
		std::vector<torc::generic::PortSharedPtr> ports;
		std::vector<torc::generic::NetSharedPtr> nets;
		view->getInstances(instances);
		view->getPorts(ports);
		view->getNets(nets);

		//Go through each Instance and write out the instantiation of each module
		std::stringstream cnlInputPort;
		std::stringstream cnlOutputPort;

		//Name, VID
		std::map<std::string, int> instanceIDMap;

		//Instance name, LUT function
		std::map<std::string, std::string> instanceLUTMap;

		std::vector<std::string> instanceNameMap;
		std::vector<std::string> instanceTypeMap;
		std::vector<std::vector<int> > instanceInputMap;
		std::vector<std::vector<std::string> > instanceInputPortMap;
		std::vector<std::vector<int> > instanceOutputMap;
		std::vector<std::vector<std::string> > instanceOutputPortMap;


		int vID = 0;
		int inputPortSize = 0;
		//Parsing ports to the module
		for (unsigned int i = 0; i < ports.size(); i++) {
			Port* port = ports[i].get();
			if(port->getDirection() == ePortDirectionIn){
				cnlInputPort<<vID<<"  "<<port->getName()<<"  ";
				instanceIDMap[port->getName()] = vID;
				instanceNameMap.push_back(port->getName());
				instanceTypeMap.push_back("INPUT");
				vID++;
				inputPortSize++;

				std::cout<<"Port:  "<< port->getName()<< "\tDIR: Input\n";
			}
			/*
			else if(port->getDirection() == ePortDirectionOut){
				cnlOutputPort<<port->getName()<<" "<<vID<<"  ";
				portOutIDMap[port->getName()] = vID;
				vID++;

				std::cout<<"Port:  "<< port->getName()<< "\tDIR: Output\n";
			}
			*/
		}
	
		int sizeCircuit = ports.size() + instances.size();
		instanceInputMap.resize(sizeCircuit);
		instanceInputPortMap.resize(sizeCircuit);
		instanceOutputMap.resize(sizeCircuit);
		instanceOutputPortMap.resize(sizeCircuit);
		
		
		//Parsing the components of the module
		for (unsigned int i = 0; i < instances.size(); i++) {
			Instance* instance= instances[i].get();
			std::string type = instance->getMaster()->getParent()->getName();
			std::string name = instance->getName();
			std::string lutFunction = "";
			instanceIDMap[name] = vID;
			instanceNameMap.push_back(name);
			instanceTypeMap.push_back(type);

			//Record the function of the LUT
			if(type.find("LUT") != std::string::npos){
				PropertySharedPtr property= instance->getProperty("INIT");
	
				if(property != NULL){
					lutFunction = property->getValue().get<Value::String>();
					instanceLUTMap[name] = lutFunction;
				}
			}
			
			vID++;
			std::cout<<"Instance:  "<< name << "\tType: "<< type <<"\n";
		}
		
		//Parsing nets within module
		for (unsigned int i = 0; i < nets.size(); i++) {
			Net* net= nets[i].get();
			std::string name = net->getName();
			
			std::vector<torc::generic::PortReferenceSharedPtr> portRefs;
			std::vector<torc::generic::PortSharedPtr> netports;
			net->getConnectedPortRefs(portRefs);
			net->getConnectedPorts(netports);
					
			std::vector<std::string> sink;
			std::vector<std::string> sinkPort;
			std::string source;
			std::string sourcePort;
			bool sourceFound = false;
			bool isPortSource = false;

			//Find the port that is connected to the instance
			for(unsigned k = 0; k < portRefs.size(); k++){
				PortReference* portref = portRefs.at(k).get();

				//TODO: Check for buses. Ignore buses for now...
				CompositionType comType = portref->getCompositionType();
				assert(comType != eCompositionTypeVectorBit);

				std::cout<<"\tPortRef:  "<<portref->getName()<<" Instance: "<<portref->getParent()->getName()<<"\n";
				std::string instanceRef = portref->getParent()->getName();
				if(portref->getMaster()->getDirection() == ePortDirectionIn){
					//sink<<portref->getName()<<" "<<instanceIDMap[instanceRef]<<"   ";
					sink.push_back(instanceRef);
					sinkPort.push_back(portref->getName());
				}
				else{
					assert(sourceFound == false);
					sourceFound = true;
					source = instanceRef;
					sourcePort = portref->getName();
					//cnlNet<<portref->getName()<<" "<<instanceIDMap[instanceRef]<<"   ";	
				}
			}

			for(unsigned k = 0; k < netports.size(); k++){
				Port* port = netports.at(k).get();

				//TODO: Check for buses. Ignore buses for now...
				CompositionType comType = port->getCompositionType();
				assert(comType != eCompositionTypeVectorBit);

				std::cout<<"\tPort:  "<<port->getName()<<"\n";
				if(port->getDirection() == ePortDirectionIn){
					assert(sourceFound == false);
					sourceFound = true;
					source = port->getName();
					sourcePort = "O";
					isPortSource = true;
					//cnlNet<<port->getName()<<" "<<portInIDMap[port->getName()]<<"   ";
				}
				else if(port->getDirection() == ePortDirectionOut){
					//sink<<port->getName()<<" "<<portOutIDMap[port->getName()]<<"   ";
					//TODO: IF YOU WANT OUTPUT PORTS
				}
			}


		//Add connections to instances
			int sourceID = instanceIDMap[source];
			for(unsigned k = 0; k < sink.size(); k++){
				int sinkID = instanceIDMap[sink[k]];
				//Connect input to output node
				std::cout<<"Adding "<<instanceIDMap[source]<<" to sink in "<<sink[k]<<"\n";
				instanceInputMap[sinkID].push_back(sourceID);
				instanceInputPortMap[sinkID].push_back(sinkPort[k]);

				//Connect output to input node
				std::cout<<"Adding "<<instanceIDMap[source]<<" to source out  "<<instanceIDMap[sink[k]]<<"\n";
				instanceOutputMap[sourceID].push_back(sinkID);
				instanceOutputPortMap[sourceID].push_back(sourcePort);
			}
			printf("done\n");
			

			//cnlNet<<sink.str() << "\n";
		}


		//Export the CNL file from data collected
		std::ofstream fs(filepath.c_str());
		//fs<<instances.size()+ portInIDMap.size() + portOutIDMap.size()<<" "<<nets.size()<<"\n";
		std::cout<<vID<<"\n";

		std::map<std::string, int>::iterator pID_it;
		std::map<std::string, int>::iterator iID_it;
		std::cout<<inputPortSize<<"  ";
		//std::cout<<cnlInputPort.str()<<"\n";

		int i = 0;
		for(i = 0; i < vID; i++){
			if(instanceTypeMap[i].find("INPUT") != std::string::npos){
				std::cout<<i<<" "<<instanceNameMap[i]<<"  ";
			}
			else
				break;

		}
/*
		for(iID_it = instanceIDMap.begin(); iID_it != instanceIDMap.end(); iID_it++){
			if(instanceTypeMap[iID_it->first].find("INPUT") != std::string::npos){
				std::cout<<iID_it->second<<" "<<iID_it->first<<"  ";
			}
		}
		*/

		printf("\n");
		/*
		for(pID_it = portOutIDMap.begin(); pID_it != portOutIDMap.end(); pID_it++){
			fs<<pID_it->second<<"  OUTPUT "<<pID_it->first<<"\n";
		}
		*/
		
		//fs<<cnlInstance.str();
		//fs<<cnlNet.str();
		/*
		for(iID_it = instanceIDMap.begin(); iID_it != instanceIDMap.end(); iID_it++){
			std::cout<<iID_it->second<< "  "<<instanceTypeMap[iID_it->first]<<"  "<<iID_it->first<<"  ";
			std::cout<<instanceInputMap[iID_it->first].size()<<"  ";
			for(unsigned int i = 0; i < instanceInputMap[iID_it->first].size(); i++){
				std::cout<<instanceInputMap[iID_it->first][i]<<" ";
				std::cout<<instanceInputPortMap[iID_it->first][i]<<" ";
			}
			std::cout<<instanceOutputMap[iID_it->first].size()<<"  ";
			for(unsigned int i = 0; i < instanceOutputMap[iID_it->first].size(); i++){
				std::cout<<instanceOutputMap[iID_it->first][i]<<" ";
				std::cout<<instanceOutputPortMap[iID_it->first][i]<<" ";
			}
			if(instanceTypeMap[iID_it->first].find("LUT") != std::string::npos){
				std::cout<<instanceLUTMap[iID_it->first]<<"\n";
			}
			else
				std::cout<<"\n";
		}
		*/
		for(i = 0; i < vID; i++){
			std::cout<<i<<"  "<<instanceTypeMap[i]<<"  "<<instanceNameMap[i]<<"  ";
			std::cout<<instanceInputMap[i].size()<<"  ";
			for(unsigned int k = 0; k < instanceInputMap[i].size(); k++){
				std::cout<<instanceInputMap[i][k]<<" ";
				std::cout<<instanceInputPortMap[i][k]<<" ";
			}
			std::cout<<instanceOutputMap[i].size()<<"  ";
			for(unsigned int k = 0; k < instanceOutputMap[i].size(); k++){
				std::cout<<instanceOutputMap[i][k]<<" ";
				std::cout<<instanceOutputPortMap[i][k]<<" ";
			}
			if(instanceTypeMap[i].find("LUT") != std::string::npos){
				std::cout<<instanceLUTMap[instanceNameMap[i]]<<"\n";
			}
			else
				std::cout<<"\n";

		}

		



		fs<<"\nEND";
}



