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
	if( argc !=  4){
	std::cout<<"\n##################################################################\n"<<std::endl;
		printf("INVALID ARGUMENTS: ./e2g <fpgenius -fp OPTION> <EDIF FILE> <Output DIR>\n");
			
		std::cout<<"\n##################################################################\n"<<std::endl;
		return 0 ;
	}	
	try{
		std::cout<<"\n================================================================================\n";
		std::cout<<  "[*]\tBegin EDIF to Graph Translation\n";
		std::cout<<"================================================================================\n";
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
	
		std::cout<<"[E2G] -- EDIF FILE: "<<modifiedEDIFPath<<endl;

		//Grab File Name
		size_t startPos = modifiedEDIFPath.find_last_of('/')+1;
		size_t endPos = modifiedEDIFPath.find_last_of('.');
		std::string filename = modifiedEDIFPath.substr(startPos, endPos-startPos);

	
		// import the EDIF design
		string inFileName = modifiedEDIFPath;
		fstream fileStream(inFileName.c_str());
		ObjectFactorySharedPtr factoryPtr(new ObjectFactory());

		EdifImporter importer(factoryPtr);
		importer(fileStream, inFileName);


		// look up an instance of interest
		RootSharedPtr rootPtr = importer.getRootPtr();

		//Export design by passing rootPtr of the imported EDIF File
		gExport->graphExport(argv[3], filename, rootPtr);
	}
	catch(exception& e){
		cout<<"[E2G] -- EXCEPTION ON MAIN"<<endl;
	}

	return 0;
}
