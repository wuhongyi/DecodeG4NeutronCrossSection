# error
OBJ = decode

sourcefile = main.cc wuString.hh wuString.cc  

CFLAGS = -Wall -O2 -lz

GXX = g++ -std=c++11

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJ)
