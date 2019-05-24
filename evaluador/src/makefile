all: initMemSem delMemSem productor consumidor evaluador

LDFLAGS=-lrt -pthread

evaluador: evaluador.o
	$(CXX) -o $@ $^ $(LDFLAGS)

initMemSem: initMemSem.o
	$(CXX) -o $@ $^ $(LDFLAGS)

delMemSem: delMemSem.o
	$(CXX) -o $@ $^ $(LDFLAGS)

productor: productor.o
	$(CXX) -o $@ $^ $(LDFLAGS)

consumidor: consumidor.o
	$(CXX) -o $@ $^ $(LDFLAGS)

evaluador.o: initMemSem.cpp elementos.h

initMemSem.o: initMemSem.cpp elementos.h

delMemSem.o: delMemSem.cpp elementos.h

productor.o: productor.cpp elementos.h

consumidor.o: consumidor.cpp elementos.h

clean:
	rm -f *.o
	rm -f *.*~ *~
	rm -f productor consumidor initMemSem delMemSem evaluador
