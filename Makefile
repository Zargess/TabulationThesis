CC=g++
CFLAGS ="-std=c++11" "-O3"
SUBDIRS= $(wildcard src/*/*.cpp)
SOURCES = $(wildcard src/*.cpp) $(SUBDIRS)
DIR = $(CURDIR)
OBJECTS=$(SOURCES:.cpp=.o)
OBJECTSWIN=$(OBJECTS:=\)
EXE=main


all: $(SOURCES) $(EXE)
	echo "Done"

$(EXE): $(OBJECTS)	
	$(CC) $(OBJECTS)  -o $@ 

.cpp.o: 
	$(CC) -c -o $@ $< $(CFLAGS)
	
# Use del on windows. rm on linux
clean-win:
	del /s *.o $(EXE).exe

clean-lin: 
	rm -rf $(OBJECTS) $(EXE)

plot:
	python data/plotfiles/plotfile.py $(PLOTARGS) $(NAMEARGS)