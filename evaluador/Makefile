BINDIR=bin
.phony: all clean ${BINDIR}

all:
SRCDIR=src
OBJECTS=${SRCDIR}/evaluador.o

all: ${BINDIR}/evaluador

LDFLAGS=-lrt -pthread

${BINDIR}/evaluador: ${OBJECTS} ${BINDIR}
	$(CXX) -o $@ ${OBJECTS} $(LDFLAGS)  

${BINDIR}:
	mkdir bin

${SRCDIR}/evaluador.o: ${SRCDIR}/evaluador.cpp ${SRCDIR}/elementos.h

clean:
	rm -R -f ${SRCDIR}/*.o ${SRCDIR}/*.h~ ${SRCDIR}/*.cpp~
	rm -R -f ${BINDIR}/evaluador