CC      = g++
CPPFLAGS= -O3 -c -std=c++11
LIBS = -lrt
LDFLAGS = -static -O3 -m64
SOURCES = main.cpp graph.cpp sort.cpp 
OBJECTS = $(SOURCES:.cpp=.o)

all: $(SOURCES) a.out

a.out : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f a.out *.o

