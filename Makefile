CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD -O2 
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = main.o config.o printer.o parent.o bank.o watcardoffice.o watcard.o vendingmachine.o nameserver.o bottlingplant.o truck.o  # list of object files
EXECS = soda

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

${EXECS} : ${OBJECTS}
	${CXX} ${CXXFLAGS} -o soda ${OBJECTS}

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} ${EXECS}
