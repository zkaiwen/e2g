/*****************************************************************
**************		Graph Exporter		**********************
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
#define XILINX "XILINX"
#define ALTERA "ALTERA"

using namespace std;
using namespace torc::generic;
void test(torc::generic::RootSharedPtr &rootPtr);
void test2(torc::generic::RootSharedPtr &rootPtr);

int main(int argc, char* argv[]) {
	std::cout<<"\n##################################################################\n"<<std::endl;
	if(argc < 2 || argc > 3){
		printf("INVALID ARGUMENTS: ./e2g <fpgenius -fp OPTION> <EDIF FILE>\n");
			
		std::cout<<"\n##################################################################\n"<<std::endl;
		return 0 ;
	}	
	try{
		std::string fileName = argv[1];
		GraphExporter* gExport;

		if(fileName == "-fp"){
			fileName = argv[2];
			gExport = new GraphExporter(argv[1]);
		}
		else
		 	gExport = new GraphExporter();
		
		std::string modifiedEDIFPath = fileName;
	
		//OUTPUT FILES WE ARE WORKING WITH
		unsigned int index = fileName.find_last_of("/");
		std::string dir = "";
		
		if(index != std::string::npos)
		 	dir = fileName.substr(0, index+1);
	
		std::cout<<"INPUT ADJUSTED EDIF FILE: "<<modifiedEDIFPath<<endl;
		std::cout<<"OUTPUT DIRECTORY: "<<dir<<std::endl;
	
		// import the EDIF design
		string inFileName = modifiedEDIFPath;
		fstream fileStream(inFileName.c_str());
		ObjectFactorySharedPtr factoryPtr(new ObjectFactory());

		cout<<"\n******************"<<endl;
		cout<<"Begin EDIF Import"<<endl;
		EdifImporter importer(factoryPtr);
		importer(fileStream, inFileName);
		cout<<"EDIF Imported!"<<endl;
		cout<<"******************\n"<<endl;


		// look up an instance of interest
		RootSharedPtr rootPtr = importer.getRootPtr();

		//Export design by passing rootPtr of the imported EDIF File
		gExport->graphExport(dir, rootPtr);

		cout<<"Graph File Exported"<<endl;
	}
	catch(exception& e){
		cout<<"EXCEPTION ON MAIN"<<endl;
	}

	std::cout<<"\n##########################################################################\n\n\n"<<std::endl;
	return 0;
}
