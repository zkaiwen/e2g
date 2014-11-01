# Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
# $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/generic/Makefile $
# $Id: Makefile 446 2011-04-15 19:15:30Z nsteiner $

# This program is free software: you can redistribute it and/or modify it under the terms of the 
# GNU General Public License as published by the Free Software Foundation, either version 3 of the 
# License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
# the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with this program.  If 
# not, see <http://www.gnu.org/licenses/>.

THIS_DIR = torc/generic
SRC_DIR = ../..
BOOST_INCLUDE_DIR = /usr/local/include/boost-1_42

# C++ flags
CCFLAGS = \
				-x c++ \
				-g \
				-Wall \
				-fopenmp \
				-Werror \
				-I$(SRC_DIR) \
				-I$(BOOST_INCLUDE_DIR) \
				$

# object files
OBJS = \
				VerilogExporter.o \
				GraphExporter.o \
				graph.o \
				vertex.o \
				$

# dependencies
DEPS = $(OBJS:.o=.d)

# generate object file lists for regular and test code
REAL_OBJS = $(filter-out %UnitTest.o %RegressionTest.o, $(OBJS))
TEST_OBJS = $(filter %UnitTest.o %RegressionTest.o, $(OBJS))

# not all targets are real
.PHONY: all code decompiler om parser util clean

# make everything
all: code

# clean everything
clean:
	rm -f $(OBJS) $(DEPS)
	rm -f ObjectFileList RealObjectFileList TestObjectFileList
	make -C decompiler clean
	make -C om clean
	make -C parser clean
	make -C util clean

# Include automatically generated dependencies or silently make them.
-include $(DEPS)

# build the code
code: decompiler om parser util $(OBJS) RealObjectFileList TestObjectFileList

# build the subdirectories
om:
	make -C om
parser:
	make -C parser
util:
	make -C util
decompiler:
	make -C decompiler

# build the real object list
RealObjectFileList: $(OBJS)
	@echo $(REAL_OBJS:%=$(THIS_DIR)/%) > RealObjectFileList

# build the test object list
TestObjectFileList: $(OBJS)
	@echo $(TEST_OBJS:%=$(THIS_DIR)/%) > TestObjectFileList

# compile c++ sources 
%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

# build dependency files
%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< |  \
	sed -e '/boost/d' -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;
