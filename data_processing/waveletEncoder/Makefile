.SUFFIXES: .cpp

MAKE = make
CXXFLAGS += -std=c++11 -D _LITTLE_ENDIAN

all : pdb 

SRCS =  wave.cpp waveAnthony.cpp block.cpp zeroblocktable.cpp zeroblocktableassist.cpp zerobit.cpp error.cpp mainassist.cpp encode.cpp main.cpp scale.cpp wavedata.cpp

OBJS =  wave.o waveAnthony.o block.o zeroblocktable.o zeroblocktableassist.o zerobit.o error.o mainassist.o encode.o main.o scale.o wavedata.o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -c $*.cpp

pdb: $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXLIBPATH) $(OBJS) $(CXXLIBS) -o pdb

clean:
	rm -f *.o pdb

depend:
	mkdepend -c "$(CC) -M $(CXXINCLUDE)" Makefile $(SRCS)
# DO NOT DELETE THIS LINE -- make depend uses it
# DO NOT DELETE THIS 2nd LINE -- make depend uses it
