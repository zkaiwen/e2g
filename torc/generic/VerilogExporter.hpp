/*****************************************************************
**************		VerilogExporter.hpp		**********************
*
*				Parses through EDIF and Converts
*				to Verilog
*
*				@Author: 	kaiwen
*				@Last Edit:	2011-10-28
*				@Started:	2011-09-02
*
*****************************************************************/

#ifndef EDIF2VERILOG_HPP1_
#define EDIF2VERILOG_HPP1_
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
#include <omp.h>


//const std::string  LEDR18 =		"/* synthesis altera_chip_pin_lc=\"@AJ7, @AD8, @AD9, @AC11, @AB12, @AC12, @AB13, @AC13, @AD14, @AH3, @AJ2, @AJ3, @AH4, @AK3, @AJ4, @AJ5, @AK5, @AJ6\"*/;";
//const std::string  LEDG9 =		"/* synthesis altera_chip_pin_lc=\"@AC14, @AA24, @AA27, @Y23, @Y24, @Y27, @W23, @W25, @W27\"*/;";
//const std::string  SWITCH18 =	"/* synthesis altera_chip_pin_lc=\"@L8, @L7, @L4, @L5, @T9, @U9, @V10, @W5, @AE27, @AD24, @AD25, @AC23, @AC24, @AC26, @AC27, @AB25, @AB26, @AA23\"*/;";
//const std::string  PUSHBTN4	=	"/* synthesis altera_chip_pin_lc=\"@U29, @U30, @T28, @T29\" */;";
//const std::string  CLK50	=	"/* synthesis altera_chip_pin_lc=\"@AD15\"*/;";
//const std::string  SEG7[8] = {
//	"/* synthesis altera_chip_pin_lc=\"@AE8, @AF9, @AH9, @AD10, @AF10, @AD11, @AD12, @AF12\" */;",
//	"/* synthesis altera_chip_pin_lc=\"@AG13, @AE16, @AF16, @AG16, @AE17, @AF17, @AD17, @AC17\" */;",
//	"/* synthesis altera_chip_pin_lc=\"@AE7, @AF7, @AH5, @AG4, @AB18, @AB19, @AE19, @AC19\" */;",
//	"/* synthesis altera_chip_pin_lc=\"@P6, @P4, @N10, @N7, @M8, @M7, @M6, @M4\" */;",
//	"/* synthesis altera_chip_pin_lc=\"@P1, @P2, @P3, @N2, @N3, @M1, @M2, @L6\" */;",
//	"/* synthesis altera_chip_bin_lc=\"@M3, @L1, @L2, @L3, @K1, @K4, @K5, @K6\" */;",
//	"/* synthesis altera_chip_pin_lc=\"@H6, @H4, @H7, @H8, @G4, @F4, @E4, @K2\" */;",
//	"/* synthesis altera_chip_pin_lc=\"@K3, @J1, @J2, @H1, @H2, @H3, @G1, @G2\" */;"}
//const std::string VGA = "/* synthesis altera_chip_pin_lc=\"@G20, @E20, @F20, @H20, @G21, @H21, @D22, @E22, @E23, @D23, @A14, @B14, @B13, @C13, @A12, @B12, @C12, @A11, @B11, @A10, @D19, @C19, @A19, @B19, @B18, @C18, @B17, @A17, @C16, @B16, @B15, @H19, @J19, @C15, @D24\" */;"

namespace torc { namespace generic { class Root; }  }
namespace torc {
namespace generic {


class VerilogExporter {
	public: //*************************************************************************************
		/*****************************************************************
		*	VerilogExporter
		*		Constructor. Creates File stream for EDIF
		*
		*		@param	name	Path to EDIF File
		*****************************************************************/
		VerilogExporter(std::string name, char fpga);



		/*****************************************************************
		*	verilogExport
		*		Parses EDIF and matches EDIF elements to verilog primitives (ALTERA)
		*
		*		@param	rootPtr	Root Pointer to EDIF Object
		*		@return	Returns true if Verilog file is sucessfully generated
		*****************************************************************/
		bool verilogExport(torc::generic::RootSharedPtr &rootPtr);


		/*****************************************************************
		*	~VerilogExporter
		*		Destructor
		*****************************************************************/
		virtual ~VerilogExporter();


	private://*************************************************************************************
		/*****************************************************************
		*	getWires
		*		Gets the necessary Wire Definitions from EDIF
		*
		*		@param	vPort	Vector of the Ports to the Cell
		*						(Used to check common Port wire names)
		*		@param	Net		Name of Wire
		*		@return	Returns string of the wire definitions.
		*****************************************************************/
		std::string getWires(std::vector<torc::generic::PortSharedPtr> vPort, Net* net);



		/*****************************************************************
		*	getAssignConstant
		*		Gets the necessary wire assignemnts from EDIF
		*
		*		@param	Net		Name of Wire
		*		@return	Returns string of constant wire assignments(VCC/GND)
		*****************************************************************/
		std::string getAssignConstant(Net* net);



		/*****************************************************************
		*	getAssignPorts
		*		Gets the necessary wire assignments for vector port bits
		*
		*		@param	vPort	Vector of the Ports to the Cell
		*						(Used to check IN/OUT of Port)
		*		@param	Net		Name of Wire
		*		@return	Returns string of the wire assignments(IN/OUT) ports
		*****************************************************************/
		std::string getAssignPorts(std::vector<torc::generic::PortSharedPtr> vPort,Net* net);


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
		bool checkComponent(std::string instance, std::string& pinAssignment, std::string& verilogHeader, Library* lib,  Instance* inst);



		/*****************************************************************
		*	assignComponent
		*		Checks to see if the instance is a board component.
		*			If it is, it will return true
		*
		*		@param	instance		Instance primitive to see if it
		*								is a board component
		*		@param	net				Net connected to component
		*		@param	index			index of the net to the board component
		*		@return	assignment		assignment of net to port
		*		@return	Returns true if instance is a component
		*****************************************************************/
		bool assignComponent(Library* lib, std::string instance, std::string index, std::string net, std::string &assignment);



		/*****************************************************************
		*	assignComponent
		*		Checks to see if the instance is a board component.
		*			If it is, it will return true
		*
		*		@param	lib				Library pointer design
		*		@param	name			Name of the cell we are looking for
		*		@param	directPortList	Returns Port list connected to the Cell with directions
		*****************************************************************/
		void getCellPortDirection(Library* lib, std::string name, std::vector<torc::generic::PortSharedPtr> &directPortList);


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

	protected://***********************************************************************************

		//File stream for EDIF
		std::ofstream mStream;
		char mFPGA;
		std::string mFileName;
	};

}//END TORC
}//END GENERIC

#endif /* EDIF2VERILOG_HPP_ */
