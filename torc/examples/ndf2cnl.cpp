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
		std::stringstream cnlInstance;
		std::stringstream cnlNet;

		//Name, VID
		std::map<std::string, int> portInIDMap;
		std::map<std::string, int> portOutIDMap;
		std::map<std::string, int> instanceIDMap;

		int vID = 0;
		//Parsing ports to the module
		for (unsigned int i = 0; i < ports.size(); i++) {
			Port* port = ports[i].get();
			if(port->getDirection() == ePortDirectionIn){
				cnlInputPort<<port->getName()<<" "<<vID<<"  ";
				portInIDMap[port->getName()] = vID;
				vID++;

				//std::cout<<"Port:  "<< port->getName()<< "\tDIR: Input\n";
			}
			else if(port->getDirection() == ePortDirectionOut){
				cnlOutputPort<<port->getName()<<" "<<vID<<"  ";
				portOutIDMap[port->getName()] = vID;
				vID++;

				//std::cout<<"Port:  "<< port->getName()<< "\tDIR: Output\n";
			}
		}
		
		
		//Parsing the components of the module
		for (unsigned int i = 0; i < instances.size(); i++) {
			Instance* instance= instances[i].get();
			std::string type = instance->getMaster()->getParent()->getName();
			std::string name = instance->getName();
			std::string lutFunction = "";
			instanceIDMap[name] = vID;

			//Record the function of the LUT
			if(type.find("LUT") != std::string::npos){
				PropertySharedPtr property= instance->getProperty("INIT");
	
				if(property != NULL){
					lutFunction = property->getValue().get<Value::String>();
				}
			}
			
			cnlInstance<<vID<<"  "<<type<<"  "<<name<<"  "<<lutFunction<<"\n";
			vID++;

			//std::cout<<"Instance:  "<< name << "\tType: "<< type <<"\n";
		}
		
		//Parsing nets within module
		for (unsigned int i = 0; i < nets.size(); i++) {
			Net* net= nets[i].get();
			std::string name = net->getName();
			
			std::vector<torc::generic::PortReferenceSharedPtr> portRefs;
			std::vector<torc::generic::PortSharedPtr> netports;
			net->getConnectedPortRefs(portRefs);
			net->getConnectedPorts(netports);
					
			cnlNet<<name<<"  ";
			std::stringstream sink;
			bool sourceFound = false;

			//Find the port that is connected to the instance
			for(unsigned k = 0; k < portRefs.size(); k++){
				PortReference* portref = portRefs.at(k).get();

				//TODO: Check for buses. Ignore buses for now...
				CompositionType comType = portref->getCompositionType();
				assert(comType != eCompositionTypeVectorBit);

				//std::cout<<"\tPortRef:  "<<portref->getName()<<" Instance: "<<portref->getParent()->getName()<<"\n";
				std::string instanceRef = portref->getParent()->getName();
				if(portref->getMaster()->getDirection() == ePortDirectionIn)
					sink<<portref->getName()<<" "<<instanceIDMap[instanceRef]<<"   ";
				else{
					assert(sourceFound == false);
					sourceFound = true;
					cnlNet<<portref->getName()<<" "<<instanceIDMap[instanceRef]<<"   ";	
				}
			}

			for(unsigned k = 0; k < netports.size(); k++){
				Port* port = netports.at(k).get();

				//TODO: Check for buses. Ignore buses for now...
				CompositionType comType = port->getCompositionType();
				assert(comType != eCompositionTypeVectorBit);

				//std::cout<<"\tPort:  "<<port->getName()<<"\n";
				if(port->getDirection() == ePortDirectionIn){
					assert(sourceFound == false);
					sourceFound = true;
					cnlNet<<port->getName()<<" "<<portInIDMap[port->getName()]<<"   ";
				}
				else if(port->getDirection() == ePortDirectionOut){
					sink<<port->getName()<<" "<<portOutIDMap[port->getName()]<<"   ";
				}
			}
			

			cnlNet<<sink.str() << "\n";


		}


		//Export the CNL file from data collected
		std::ofstream fs(filepath.c_str());
		fs<<instances.size()+ portInIDMap.size() + portOutIDMap.size()<<" "<<nets.size()<<"\n";

		std::map<std::string, int>::iterator pID_it;
		std::map<std::string, int>::iterator iID_it;
		fs<<portInIDMap.size()<<"  ";
		fs<<cnlInputPort.str()<<"\n";
		fs<<portOutIDMap.size()<<"  ";
		fs<<cnlOutputPort.str()<<"\n\n";
		
		for(pID_it = portInIDMap.begin(); pID_it != portInIDMap.end(); pID_it++){
			fs<<pID_it->second<<"  INPUT "<<pID_it->first<<"\n";
		}
		for(pID_it = portOutIDMap.begin(); pID_it != portOutIDMap.end(); pID_it++){
			fs<<pID_it->second<<"  OUTPUT "<<pID_it->first<<"\n";
		}
		
		fs<<cnlInstance.str();
		fs<<cnlNet.str();
		fs<<"\nEND";
}
