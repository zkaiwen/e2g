/*****************************************************************
**************		GraphExporter.hpp		**********************
*
*				Parses through EDIF and Converts
*				to Graph
*
*				@Author: 	kaiwen
*				@Last Edit:	2012-02-19
*				@Started:	2012-02-19
*
*****************************************************************/

#ifndef EDIF2GRAPH_HPP1_
#define EDIF2GRAPH_HPP1_
#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/om/Cell.hpp"
#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/View.hpp"
#include "torc/generic/om/Instance.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/VectorPortBit.hpp"
#include "torc/generic/om/VectorPortBitReference.hpp"
#include "torc/generic/om/ObjectFactory.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>


namespace torc { namespace generic { class Root; }  }
namespace torc {
namespace generic {


class GraphExporter {
	public: 
		//*************************************************************************************
		/*****************************************************************
		*	GraphExporter
		*		Constructor. Creates File stream for EDIF
		*
		*		@param	name	Path to EDIF File
		*****************************************************************/
		GraphExporter();
		GraphExporter(std::string option);



		/*****************************************************************
		*	graphExport
		*		Parses EDIF and matches EDIF elements to graph primitives (ALTERA)
		*
		*		@param	rootPtr	Root Pointer to EDIF Object
		*		@return	Returns true if Graph file is sucessfully generated
		*****************************************************************/
		bool graphExport(std::string outDir, torc::generic::RootSharedPtr &rootPtr);


		/*****************************************************************
		*	~GraphExporter
		*		Destructor
		*****************************************************************/
		virtual ~GraphExporter();


	private:
		//*************************************************************************************
		/*****************************************************************
		*	checkComponent
		*		Checks to see if the instance is a board component.
		*			If it is, it will return true
		*
		*		@param	instance		Instance primitive to see if it
		*								is a board component
		*		@return	pinAssignment	Pin Assignment
		*		@return	Returns true if instance is a component
		*****************************************************************/
		bool checkComponent(std::string instance, std::string& pinAssignment, Library* lib,  Instance* inst);



		/*****************************************************************
		*	assignComponent
		*		Checks to see if the instance is a board component.
		*			If it is, it will return true
		*
		*		@param	lib				Library pointer design
		*		@param	name			Name of the cell we are looking for
		*		@param	directPortList	Returns Port list connected to the Cell with directions
		*****************************************************************/
		 PortDirection getCellPortDirection(Library* lib, std::string cell, std::string port);


		/*****************************************************************
		*	checkProperties
		*		Checks to see if the instance is a board component.
		*			If it is, it will return true
		*
		*		@param	instance		Pointer to instance object
		*		@param	instName		name of specific instance
		*		@param	defParam		Returns parameter string
		*****************************************************************/
		void checkProperties(Instance* instance, std::string instName, std::string &defParam);

		std::string getPorts(View* view);
		std::string trim(std::string str);
		std::string getSimpleName(std::string name, std::string simpleName);

	protected://***********************************************************************************

		//File stream for EDIF
		std::ofstream mStream;
		std::map<std::string, int> instNum;
		std::map<std::string, std::string> simpName;
		bool fp;
		std::map<std::string,int> gates;
		std::map<std::string,int> level;
		
	};

}//END TORC
}//END GENERIC

#endif /* EDIF2GRAPH_HPP_ */
