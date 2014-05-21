/*****************************************************************
**************		VerilogExporter.cpp		**********************
*
*				Parses through EDIF and Converts
*				to Verilog
*
*				@Author: 	kaiwen
*				@Last Edit:	2012-04-10
*				@Started:	2011-09-02
*
*****************************************************************/


#include "torc/generic/VerilogExporter.hpp"

//FPGA TYPE
#define ALTERA 'a'
#define XILINX 'x'

namespace torc {
namespace generic {

//****************************************************************
//	Default Constructor
//****************************************************************
VerilogExporter::VerilogExporter(std::string name, char fpga) {
	mStream.open(name.c_str(), std::ios::out);
	mFPGA = fpga;
	mFileName = name;
}




//****************************************************************
//	Destructor
//****************************************************************
VerilogExporter::~VerilogExporter() {
	// TODO Auto-generated destructor stub
}




//****************************************************************
//	verilogExport:
//		Main function that does the parsing and conversion from 
//		EDIF TO VERILOG
//****************************************************************
bool VerilogExporter::verilogExport(torc::generic::RootSharedPtr &rootPtr) {
	//Initial Declarations
	std::vector<torc::generic::LibrarySharedPtr, std::allocator<LibrarySharedPtr> > vLibrary;
	std::string module = "";
	std::string moduleHeader = "";
	std::string portInOut = "";
	std::string wire = "";
	std::string instance = "";
	std::string assign = "";
	std::string portAssign = "";
	std::string componentAssign = "";
	std::string defParam = "";
	std::string verilogHeader = "";
	std::string vHeaderFileName = "";

	try {
		std::cout<<"\n*********************************\n";
		std::cout<<  "Begin EDIF to Verilog Translation\n\n";

		//Get library of interest
		rootPtr->getLibraries(vLibrary);
		Library* lib = vLibrary.at(vLibrary.size() - 1).get();
		std::vector<torc::generic::CellSharedPtr> vCell;
		lib->getCells(vCell);

		//Get cell of interest
		Cell* cell = vCell.at(vCell.size() - 1).get();
		std::vector<torc::generic::ViewSharedPtr> vViews;
		cell->getViews(vViews);

		//Get the module name from cell
		moduleHeader += "module " + cell->getName() + " ( ";
		
		View* view = vViews.at(0).get(); 
		std::vector<torc::generic::PortSharedPtr> vPort;
		view->getPorts(vPort);
			
		//GENERATE VERILOG HEADER FILE***********
		//Output header file to file		
        	vHeaderFileName = "aztop_inst.vh";
		verilogHeader += "`define AZIDOMODULENAME " + cell->getName() + "\n\n"; 
		verilogHeader += cell->getName() + " ( ";

		//Get Directory		
		std::cout<<"FULL DIR: "<<mFileName<<std::endl;
		unsigned int directoryIndex = mFileName.rfind("/");
		if(mFileName.rfind("/") == std::string::npos)
			directoryIndex = mFileName.rfind("\\");
			
		std::string directory = mFileName.substr(0, directoryIndex+1);
		
		std::cout<<"\nOUTPUT HEADER FILE: "<<directory<<vHeaderFileName<<std::endl;
		std::ofstream headerStream;
		vHeaderFileName = directory + vHeaderFileName;

		//Go through each port of the cell and determine if it's input or output
		//std::cout << "TRAVERSING INPUT OUTPUT PORTS....." << std::endl;
		for (unsigned int l = 0; l < vPort.size(); l++) {
			Port* port = vPort.at(l).get();

			if(vHeaderFileName != "")
				verilogHeader += "\n\t." + port->getName() + "(" + port->getName() + "),";
			
			//Sets the size of array for wire
			int portSize = port->getSize() - 1;
			std::string psize = "";

			//Only make it a wire array if more than one bit
			if (portSize > 0)
				psize = "[" + boost::lexical_cast<std::string>(portSize) + ":0] ";

			//DEFINE INPUT OUTPUT PORTS
			switch (port->getDirection()) {
			case ePortDirectionIn:
				portInOut += "\tinput " + psize + port->getName() + ";\n";
				moduleHeader+= port->getName() + ", ";
				break;
			case ePortDirectionOut:
				portInOut += "\toutput " + psize + port->getName() + ";\n";
				moduleHeader+= port->getName() + ", ";
				break;
			/*case ePortDirectionInOut:
				portInOut += "\tinout " + psize + port->getName() + ";\n";
				moduleHeader+= port->getName() + ", ";
				break;*/
			default:
				std::cout << "ERROR: Undefined port exists" << std::endl;
				return false;
			}
		}

		
		//Get nets
		std::vector<torc::generic::NetSharedPtr> vNet;
		view->getNets(vNet);

		//ASSIGNMENTS FOR NETS (WIRES)
		//Go through each net and get wire instantiations and assignments
		for (unsigned int l = 0; l < vNet.size(); l++) {
			Net* net = vNet.at(l).get();
			
			//Wire Declarations for port assignments
			wire += getWires(vPort, net);

			//Get the assigns that are stuck at VCC or GND (FOR ALTERA)
			if(mFPGA == ALTERA)
				assign += getAssignConstant(net);

			//Assignments of wires to the ports
			portAssign += getAssignPorts(vPort,net);
		}

		//Combine assignment strings
		assign += "\n" + portAssign;

		//Get instances
		std::vector<torc::generic::InstanceSharedPtr> vInst;
		view->getInstances(vInst);

		//std::cout << "SEARCHING FOR INSTANCES.........." << std::endl;
		//Go through each Instance and write out the instantiation of each module
		for (unsigned int l = 0; l < vInst.size(); l++) {
			Instance* inst = (vInst.at(l).get());

			//Get the primitive name for the instance (Cellref)
			std::string instPrimitive = inst->getMaster()->getParent()->getName();
			std::string originalPrimitive = inst->getMaster()->getParent()->getName();

			//Skip instance if it is VCC. There is no VCC/GND Primitive
			//(USED FOR LOGIC 0/1 WIRE ASSIGNMENTS)
			if(mFPGA == ALTERA)
				if (instPrimitive == "VCC" || instPrimitive == "GND")
					continue;

			//Gets the instance name
			std::string instName = inst->getName();
			std::string instParameter = " ";
			std::string pinAssignment = "";

			//CHECK TO SEE IF INSTANCE IS A BOARD COMPONENT
			bool isComponent = checkComponent(instPrimitive,  pinAssignment, verilogHeader, lib, inst);
			
			//Add port to Module header
			if(isComponent)
				moduleHeader+= "\n\t" + instPrimitive + ", ";

			//CHECK TO SEE IF INSTANCE IS TYPE LPM (ALTERA PARAMETERIZED MODULES)
			else if(instPrimitive.find("__", 0) != std::string::npos){
				instPrimitive = instPrimitive.substr(0, instPrimitive.find("__",0));
			}

			//Check for parameter settings in the property fields
			if(!isComponent)
				checkProperties(inst, instName, defParam);

			//Form instance line with primitive parameters and name if not component.
			if(isComponent)
				portInOut += pinAssignment;
			else
				instance += "\t" + instPrimitive + instParameter + instName + "( ";

			//Get the port reference of the instance
			std::vector<torc::generic::PortReferenceSharedPtr> vRef;
			inst->getPortReferences(vRef);
			//std::cout<<"\n\nINST: "<<instPrimitive<<std::endl;
			
			// Go through each port of the instance and 
			// find the net that connects to that port
			for (unsigned int m = 0; m < vRef.size(); m++) {
				//Get the name of the port reference
				PortReference* ref = vRef.at(m).get();
				std::string refName = ref->getName();
				//std::cout<<"\tPORTREF: "<<refName<<std::endl;
				
				//Hard coded fix for azido for altera primitive gates 
				if(mFPGA == ALTERA){
					if (refName.c_str()[0] == 'I' && (refName.length() < 4))
						refName.replace(0, 1, "IN");
					else if (refName.c_str()[0] == 'O' && (refName.length() < 4))
						refName.replace(0, 1, "OUT");
				}
				
				//Get nets connect to the port
				std::vector<torc::generic::NetSharedPtr> connectedNet;
				ref->getConnectedNets(connectedNet);
				
				//Skip if port is floating
				if(connectedNet.size() == 0){
					//std::string shortName = instName.substr(0, 20);
					//std::cout<<"WARNING: Port " + refName + " of instance " + shortName + " (" + instPrimitive + ") is unconnected\n";
					continue;	
				}
				
				//If port is IO, don't instance. 
				if(!isComponent)
					instance += "\n\t\t." + refName + "(";
		
				// Find the net that connects to the port
				// If It is a multibit port, find the nets
				// that connects to the portbits
				for(unsigned int p = 0; p < connectedNet.size(); p++){
				
					//Go through the ports the net connects to
					Net* net = connectedNet.at(p).get();
					std::vector<torc::generic::PortReferenceSharedPtr> refTemp;
					net->getConnectedPortRefs(refTemp);
					//std::cout<<"NET NAME: "<<net->getName()<<"\t"<<net->getSize()<<std::endl;
					
					//Find the port that is connected to the instance
					//Need this step because now we're in the net's view
					for(unsigned q = 0; q < refTemp.size(); q++){
						PortReference* refnet = refTemp.at(q).get();
						torc::generic::CompositionType comType = refnet->getCompositionType(); 
						std::string refNetName = refnet->getName();
						
						//Hard coded fix for azido for altera primitive gates
						//MAY CAUSE ISSUES IF OTHER PORT NAME MATCHES
						if(mFPGA == ALTERA){
							if (refNetName.c_str()[0] == 'I' && (refNetName.length() < 4))
								refNetName.replace(0, 1, "IN");
							else if (refNetName.c_str()[0] == 'O' && (refNetName.length() < 4))
								refNetName.replace(0, 1, "OUT");
						}
						
						//std::cout<<"1st REF: "<<refName<<"\t2nd REF: "<<refNetName<<std::endl;

						//If the port reference is a bit of a port, get index
						if (comType == torc::generic::eCompositionTypeVectorBit) {
							torc::generic::VectorPortBitReference* vectorPort =
									(torc::generic::VectorPortBitReference*) &*refnet;	
									
							std::vector<std::size_t> index = vectorPort->getIndices();
							std::string instRefName = vectorPort->getParentCollection()->getParent()->getName();

							//Check to make sure it is the port of the instance we are looking for.
							if( instRefName == instName && refNetName == refName){
								std::string indexSize = boost::lexical_cast<std::string>(ref->getSize()-1);
								std::string indexVal = boost::lexical_cast<std::string>(index[0]);
								std::string wirename;

								std::vector<torc::generic::PortSharedPtr> directPortList;
								bool isOut = false;

								//Check port direction
								getCellPortDirection(lib, originalPrimitive, directPortList);
								for(unsigned int p = 0 ; p < directPortList.size(); p++){
									Port* directPort = directPortList.at(p).get();
									if(vectorPort->getName() == directPort->getName()){
										if(directPort->getDirection() == ePortDirectionOut)
											isOut = true;
									}
								}
								
								//If previous net was found, use net. Otherwise make a new net
								wirename = refNetName + instName;
								//Assignments depending on in/out
								if(!isOut)
									assign += "\tassign " + wirename + "[" + indexVal + "] = "
										+ net->getName() + ";\n";
								else
									assign += "\tassign " + net->getName() + " = "
										+  wirename + "[" + indexVal + "];\n";

								//Declare the wire if not already declared.
								if (wire.find(wirename, 0) == std::string::npos) {
									wire += "\twire [" + indexSize + ":0]" + wirename + ";\n";
									instance += wirename + "), ";
									//netFound = true;
								}
								
							}
						}

						//If the instance name is the same as the instance that the net is going to 
						//and the right ports, write out the name of the net
						else if ((refnet->getParent()->getName() == instName) && (refNetName == refName)) {
							std::string assignment = "";
							
							//If connection is to a port/component, no instance is required
							if(isComponent){
								//Assigns net to port
								assignComponent(lib, instPrimitive, refNetName, net->getName(), assignment);
								componentAssign += assignment;
							}
							else{
								instance += net->getName() + "), ";
								break;
							}
						}
					}
				}
			}
			
			//Finish off the instance line
			if(instance.length() > 0  && !isComponent){
				instance.erase(instance.length() - 2, 1);
				instance += "\n\t);\n";
			}
		}

		//Finish off the module instantiation line.
		if(moduleHeader.at(moduleHeader.size() - 2) == ',')
			moduleHeader.erase(moduleHeader.length() - 2, 1);
		moduleHeader += ");\n";

		//Finish off the defParam line
		if(defParam.length() > 0){
			defParam.erase(defParam.length() - 2, 1);
			defParam = "\tdefparam" + defParam + ";\n";
		}
		
		if(verilogHeader.at(verilogHeader.size() - 2) != '(')
			verilogHeader.erase(verilogHeader.length() - 1, 1);
		verilogHeader += "\n);\n";
		std::cout<<"Verilog Header file generated"<<std::endl;

		//Format output
		if (wire == "")
			module = moduleHeader + "\n" + portInOut + "\n\n" + instance + "\n" + defParam + "\nendmodule\n\n";
		else if (assign == "")
			module = moduleHeader + "\n" + portInOut + "\n\n" + wire + "\n" + instance + "\n" + defParam + "\nendmodule\n\n";
		else
			module = moduleHeader + "\n" + portInOut + "\n\n" + wire + "\n" + assign + "\n" + componentAssign + "\n" + instance +  "\n" + defParam + "\nendmodule\n\n";

		//Output contents of file out to terminal
		//std::cout << module << std::endl;

		//Output contents to file
		mStream << module;
		mStream.close();
		

		//Output verilogHeader to stream
		headerStream.open(vHeaderFileName.c_str(), std::ios::out);
		headerStream << verilogHeader;
		headerStream.close();
		
	
		std::cout << "CONVERSION COMPLETE"<< std::endl;
		std::cout<<  "*******************\n"<<std::endl;

	} catch (std::exception &e) {
		std::cout << "EXCEPTION IN exportAlteraVerilog\n\n" << e.what() << std::endl << std::endl;
		return false;
	}

	return true;
}


//****************************************************************
//	getWires
//		Declares nets as wires as needed. 
//****************************************************************
std::string VerilogExporter::getWires(std::vector<torc::generic::PortSharedPtr> vPort, Net* net){
	//Initial Declarations
	std::string name = net->getName();

	//Compare net name with input output ports. If same, don't declare as wire
	for (unsigned int m = 0; m < vPort.size(); m++) {
		std::string portName = (vPort.at(m).get())->getName();
		if (name == portName) {
			//If it is a port, don't create wire
			return "";
		}
	}
	
	//Create wire because the name is not the same as any of the ports 
	//Make wire array if more than 1 net
	if( net->getSize() > 1){
		//Get the size of the wire array
		std::string indexVal = boost::lexical_cast<std::string>(net->getSize()-1);		
		return "\twire [" + indexVal + ":0] " + name + ";\n";
	}
	else
		return "\twire " + name + ";\n";
}



//****************************************************************
//	getAssignConstant
//		Assigns constant 1 or 0 to any net that is connected to 
//		VCC OR GROUND
//****************************************************************
std::string VerilogExporter::getAssignConstant(Net* net){
	//Initial Declarations
	std::string name = net->getName();
	std::vector<torc::generic::PortReferenceSharedPtr> vcpr;
	
	//Get ports that the net connects to. 
	net->getConnectedPortRefs(vcpr);

	//There is no VCC/GND primitive in Altera.
	//Assign the net to a Logic 1 for VCC and Logic 0 for GND
	for (unsigned int m = 0; m < vcpr.size(); m++) {
		PortReference* r = vcpr.at(m).get();
		if (r->getName() == "VCC")
			return "\tassign " + name + " = 1'b1;\n";
		else if (r->getName() == "GROUND")
			return "\tassign " + name + " = 1'b0;\n";
	}

	return "";
}

//****************************************************************
//	checkComponent
//		Checks to see if the instance is a board component or not
//****************************************************************
bool VerilogExporter::checkComponent(std::string instance, std::string& pinAssignment, std::string& verilogHeader, Library* lib, Instance* inst){

	PropertySharedPtr initPropertyPtr = inst->getProperty("PIN");

	if(initPropertyPtr != NULL){
		std::string property = initPropertyPtr->getValue().get<Value::String>();

		std::vector<torc::generic::PortSharedPtr> directPortList;
		getCellPortDirection(lib, instance, directPortList);
		Port* directPort = directPortList.at(0).get();
		//std::cout<<instance<<std::endl;
		std::string direction = "\toutput ";
		if(directPort->getDirection() == ePortDirectionOut)
			direction = "\tinput ";
		/*else if(directPort->getDirection() == ePortDirectionInOut)
			direction = "\tinput ";*/

		std::string size = "";
		if(directPortList.size() > 1){
			std::string indexVal = boost::lexical_cast<std::string>(directPortList.size()-1);
			size = "[" + indexVal + ":0] ";
		}
		pinAssignment = direction + size + instance +  " /* synthesis altera_chip_pin_lc=\"" + property + "\"*/ ;\n";
		verilogHeader += + "\n\t." + instance + "(" + instance + "),";
	}
	else
		return false;

	return true;

}



//****************************************************************
//	assignComponent
//		Assigns the nets that are connected to the component pins
//****************************************************************
bool VerilogExporter::assignComponent(Library* lib, std::string instance, std::string index, std::string net,  std::string& assignment){
	//Assigns nets that are connected to the components
	std::vector<torc::generic::PortSharedPtr> directPortList;
	getCellPortDirection(lib, instance, directPortList);
	Port* directPort = directPortList.at(0).get();

	std::string size = "";
	
	//If it is a vectorport, Get the index of the port where the net is connected to
	if(directPortList.size() > 1)
		for(unsigned int p = 0 ; p < directPortList.size(); p++){
			Port* directPort = directPortList.at(p).get();

			//Makes sure that the ports are the same before retreiving the index
			if(index == directPort->getName()){
				std::string indexVal = boost::lexical_cast<std::string>(directPortList.size()-1-p);
				size = "[" + indexVal + "] ";
				break;
			}
		}

	//Determine the direction of the port
	bool isOut = false;
	if(directPort->getDirection() == ePortDirectionOut)
		isOut = true;

	if(isOut)
		assignment = "\tassign " + net + "= " + instance + size +";\n";
	else
		assignment = "\tassign "+instance + size + " = " + net + ";\n";

	return true;
}



//****************************************************************
//	getAssignPorts
//		Assigns nets to the outgoing ports
//****************************************************************
std::string VerilogExporter::getAssignPorts(std::vector<torc::generic::PortSharedPtr> vPort, Net* net){
	//Initial Declarations
	std::vector<torc::generic::PortSharedPtr> portTemp;
	net->getConnectedPorts(portTemp);
	std::string name = net->getName();
	
	//Need to store assign because there could be a net that connects to more than one port
	std::string assign = "";

	
	//std::cout<<"NET NAME: "<<name<<"\tSIZE: "<<portTemp.size()<<std::endl;	

	//Check to see if There are any vector bits that are connected directly to output.
	//If there is assign the output port with the respective net
	for (unsigned int o = 0; o < portTemp.size(); o++) {
		Port* r = portTemp.at(o).get();

		//Check to see if it is a Vectorport
		torc::generic::CompositionType comType = r->getCompositionType();
					
		if (comType == torc::generic::eCompositionTypeVectorBit) {
			//Type cast to vector port so we can extract the member (index)
			torc::generic::VectorPortBit* vectorPort = (torc::generic::VectorPortBit*) &*r;

			std::vector<std::size_t> index = vectorPort->getIndices();
			std::string indexVal = boost::lexical_cast<std::string>(index[0]);

			//Make sure that every bit in the port is connected if the port names are the same
			for(unsigned int i = 0; i < vPort.size(); i++){
				Port* ptemp = vPort.at(i).get();

				if(r->getName()== ptemp->getName()){
					if(ptemp->getDirection() == ePortDirectionOut)
						assign += "\tassign " + r->getName() + "[" + indexVal + "] = "
							+ name+ ";\n";
					else if (ptemp->getDirection() == ePortDirectionIn)
						assign += "\tassign " + name+  " = "+ r->getName() +
							"[" + indexVal + "];\n";
					else
						std::cout<<"UNKNOWN PORT DIRECTION"<<std::endl;
				}
			}
		}
		else{
			//Make sure the port name is not the same as the net name
			if(r->getName() == name)
				continue;			
				
			std::cout<<"P1: "<<r->getName()<<std::endl;
			std::cout<<"NET NAME: "<<name<<std::endl;
			
			//Assign net to the port
			if(r->getDirection() == ePortDirectionOut)
				assign += "\tassign " + r->getName() + " = " + name + ";\n";
			else if(r->getDirection() == ePortDirectionIn)
				assign += "\tassign " + name + " = " + r->getName() + ";\n";
		}
	}

	return assign;
}


//****************************************************************
//	getCellPortDirection
//		Get the ports and their direction given a cell name
//****************************************************************
void VerilogExporter::getCellPortDirection(Library* lib, std::string name, std::vector<torc::generic::PortSharedPtr> &directPortList){
	//Need to get direction of the port of the instance
	CellSharedPtr cptr = lib->findCell(name);
	std::vector<torc::generic::ViewSharedPtr> viewTemp;
	Cell* cell = cptr.get();

	cell->getViews(viewTemp);
	viewTemp.at(0).get()->getPorts(directPortList);
}

//****************************************************************
//	checkProperties
//		Gets all the properties tied to the instance as 
//		parameterized settings for the instance
//****************************************************************
void VerilogExporter::checkProperties(Instance* instance, std::string instName, std::string &defParam){
	//Map Iterator
	std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,
		PropertySharedPtr,
		std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,
		std::allocator<std::pair <const std::basic_string<char,std::char_traits<char>, std::allocator<char> >,
		PropertySharedPtr> > >::iterator it;

	//Map
	std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,
		PropertySharedPtr,
		std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,
		std::allocator<std::pair <const std::basic_string<char,std::char_traits<char>, std::allocator<char> >,
		PropertySharedPtr> > > propertyMap;

	//Get Property maps from the instance
	instance->getProperties(propertyMap);

	//If Map is empty, there are no properties to check
	if(propertyMap.size() == 0){
		return;
	}

	std::cout<<"PRIM: "<<instance->getMaster()->getParent()->getName() << std::endl;
	
	//For each property, get the parameters associated with the cell.
	for(it = propertyMap.begin(); it != propertyMap.end(); it++){
		std::string propName = it->first;
		std::string value = it->second->getValue().get<Value::String>();

		//Check to see if parameter is a string or integer
		if( value[0]=='|' && value.at(value.length()-1) == '|')
			value = "\"" + value.substr(1, value.length()-2) + "\"";

		std::cout<<"\t\tPROP NAME: "<< propName <<" VAL: "<< value<<std::endl;
		defParam += "\n\t\t" + instName + "." + propName + " = " + value + ", ";
	}
	
}


} //namespace
} //namespace
