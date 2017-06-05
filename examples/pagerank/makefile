CC      = g++
CPPFLAGS= -O3 -c -std=c++11
LIBS = -lrt -lpthread
LDFLAGS = -static -O3 -m64
SOURCES_ST = prST.cpp graph.cpp sort.cpp 
OBJECTS_ST = $(SOURCES_ST:.cpp=.o)

SOURCES_MT = prMT.cpp graph.cpp sort.cpp 
OBJECTS_MT = $(SOURCES_MT:.cpp=.o)

all: ST MT

ST: $(SOURCES_ST) prST

prST : $(OBJECTS_ST)
	$(CC) $(LDFLAGS) $(OBJECTS_ST) -o $@ $(LIBS)

MT: $(SOURCES_MT) prMT

prMT : $(OBJECTS_MT)
	$(CC) $(LDFLAGS) $(OBJECTS_MT) -o $@ $(LIBS)

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f prST prMT *.o

