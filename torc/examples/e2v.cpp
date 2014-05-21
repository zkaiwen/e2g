/*****************************************************************
**************		VerilogExporter.hpp		**********************
*
*				Parses through EDIF and Converts
*				to Verilog
*
*				@Author: 	kaiwen
*				@Last Edit:	2012-04-11
*				@Started:	2011-09-02
*
*****************************************************************/

#include "torc/Generic.hpp"
#include "torc/generic/VerilogExporter.hpp"
#include <fstream>
#include <map>
#include <boost/regex.hpp>

using namespace std;
using namespace torc::generic;

int main(int argc, char* argv[]) {
	std::cout<<"\n#########################################################################*\n"<<std::endl;
	if(argc != 3){
		std::cout<<"INVALID ARGUMENTS\n./e2v <FPGA> <EDN File>"<<std::endl;
		std::cout<<"FPGA:"<<std::endl;
		
		// TEMPORARY FIX FOR AZIDO HARDCODED PORT NAMES FOR PRIMITIVE LOGIC GATES
		std::cout<<"\t-a\tALTERA PLATFORM"<<std::endl;
		std::cout<<"\t-x\tXILINX PLATFORM"<<std::endl;
		
		return 0;
	}

	//Check second argument to see which fpga platform we are trying 
	//	retreive the Verilog file for. 
	char fpga;
	try{
		fpga = argv[1][1];
		if(fpga == 'a' || fpga == 'A')
			std::cout<<"ALTERA FPGA SELECTED"<<endl;
		else if(fpga == 'x' || fpga == 'X')
			std::cout<<"XILINX FPGA SELECTED"<<endl;
		else{
			std::cout<<"UNKNOWN FPGA SELECTED. EXITING..."<<std::endl;
			std::cout<<"\t-a\tALTERA PLATFORM"<<std::endl;
			std::cout<<"\t-x\tXILINX PLATFORM"<<std::endl;
			return 0;
		}
	}
	catch(std::exception &e){
		std::cout<<"UNKNOWN ARGUMENT. EXITING...\n./e2v <OPTION> <EDN File>"<<std::endl;
		std::cout<<"Options:"<<std::endl;
		
		// TEMPORARY FIX FOR AZIDO HARDCODED PORT NAMES FOR PRIMITIVE LOGIC GATES
		std::cout<<"\t-a\tALTERA PLATFORM"<<std::endl;
		std::cout<<"\t-x\tXILINX PLATFORM"<<std::endl;
	}
	
	//Get the file name and extension from the argument list
	std::string fileFullName = argv[2];
	std::string fileName = fileFullName;
	
	//Get the file name without extension
	unsigned int index = fileName.find_first_of(".");
	if(index != std::string::npos)
		fileName = fileName.substr(0, index);
	
	//Set input and output files
	std::string modifiedEDIFPath = fileFullName;
	std::string generatedVerilogPath = fileName + ".v";

	//OUTPUT FILES WE ARE WORKING WITH
	std::cout<<"INPUT ADJUSTED EDIF FILE: "<<modifiedEDIFPath<<endl;
	std::cout<<"OUTPUT V FILE: "<<generatedVerilogPath<<endl;

	try{
		// import the EDIF design
		string inFileName = modifiedEDIFPath;
		fstream fileStream(inFileName.c_str());
		if(!fileStream.is_open()){
			std::cout<<"Cannot open file. EDIF File may be missing"<<std::endl;
			fileStream.close();
			return 0;
		}	
			
		ObjectFactorySharedPtr factoryPtr(new ObjectFactory());

		cout<<"Begin EDIF Import......................."<<endl;
		EdifImporter importer(factoryPtr);
		importer(fileStream, inFileName);
		cout<<"EDIF Imported"<<endl;

		// look up an instance of interest
		RootSharedPtr rootPtr = importer.getRootPtr();

		//Instantiate verilog exporter object and pass in file name
		VerilogExporter* vExport = new VerilogExporter(generatedVerilogPath, fpga);

		//Export design by passing rootPtr of the imported EDIF File
		vExport->verilogExport( rootPtr);
		
		cout<<"Verilog File Exported"<<endl;
	}
	catch(exception& e){
		cout<<"EXCEPTION ON MAIN"<<endl;
	}

	std::cout<<"\n##########################################################################\n\n\n"<<std::endl;
	return 0;
}
