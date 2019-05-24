BINDIR=bin
.phony: all clean ${BINDIR}

all: src/initMemSem src/delMemSem src/productor src/consumidor src/main

LDFLAGS=-lrt -pthread

src/main: main.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/initMemSem: initMemSem.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/delMemSem: delMemSem.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/productor: productor.o
	$(CXX) -o $terminal0 $make
terminal0 $./initMemSem
  terminal1    $./consumidor
terminal0 $./productor@ $^ $(LDFLAGS)

src/consumidor: consumidor.o
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: initMemSem.cpp elementos.h

initMemSem.o: initMemSem.cpp elementos.h

delMemSem.o: delMemSem.cpp elementos.h

productor.o: productor.cpp elementos.h

consumidor.o: consumidor.cpp elementos.h

clean:
	rm -f *.o
	rm -f *.*~ *~
	rm -f productor consumidor initMemSem delMemSem
