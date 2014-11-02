# Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
# $HeadURL: https://svn.east.isi.edu/torc/trunk/src/Makefile $
# $Id: Makefile 703 2011-08-12 22:31:34Z nsteiner $

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

ifneq ($(TORC_X), 1)
LDFLAGS = \
				-L/usr/local/lib \
				-dead_strip \
				-lboost_filesystem \
				-lboost_regex \
				-lboost_signals \
				-lboost_system \
				-lboost_thread \
				-lstdc++ \
				$
LDTESTFLAGS = \
				-lboost_unit_test_framework \
				$
else
LDFLAGS = \
				-arch x86_64 \
				-L/usr/local/lib \
				-dead_strip \
				-lboost_filesystem \
				-lboost_regex \
				-lboost_signals \
				-lboost_system \
				-lboost_thread \
				-lstdc++ \
				$
LDTESTFLAGS = \
				-lboost_unit_test_framework \
				$
endif


REAL_OBJECTS = \
				@torc/generic/RealObjectFileList \
				@torc/generic/om/RealObjectFileList \
				@torc/generic/parser/RealObjectFileList \
				@torc/generic/util/RealObjectFileList \
				@torc/generic/decompiler/RealObjectFileList \
				$

TEST_OBJECTS = \
				@torc/generic/TestObjectFileList \
				@torc/generic/om/TestObjectFileList \
				@torc/generic/parser/TestObjectFileList \
				@torc/generic/util/TestObjectFileList \
				@torc/generic/decompiler/TestObjectFileList \
				$


.PHONY: torc clean

# make everything

all: e2v e2g ndf2cnl ndf2nlv



# make the generic example executable
e2v: torc 
	$(CC) \
				$(REAL_OBJECTS) \
				torc/examples/e2v.o \
				$(LDFLAGS) \
				-o $@ \
				$

e2g: torc 
	$(CC) \
				$(REAL_OBJECTS) \
				torc/examples/e2g.o \
				$(LDFLAGS) \
				-o $@ \
				$

ndf2cnl: torc 
	$(CC) \
				$(REAL_OBJECTS) \
				torc/examples/ndf2cnl.o \
				$(LDFLAGS) \
				-o $@ \
				$

ndf2nlv: torc 
	$(CC) \
				$(REAL_OBJECTS) \
				torc/examples/ndf2nlv.o \
				$(LDFLAGS) \
				-o $@ \
				$

# make the torc subdirectory
torc:
	make -C torc


# clean everything
clean:
	make -C torc clean
	rm -f $(OBJS)
	rm -f e2v
	rm -f e2g 
	rm ndf2*
