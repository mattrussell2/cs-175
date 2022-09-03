#Makefile for lab2 
CXX           = $(shell fltk-config --cxx)
CXXFLAGS      = $(shell fltk-config --cxxflags)
LINKFLTK_FLGS = $(shell fltk-config --ldflags --use-gl --use-images)
POSTBUILD     = fltk-config --post #build .app for osx. (does nothing on pc)

all: l2

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -lglut -lGLU -lGL 

l2:  main.o ply.o 
	$(CXX) -o $@ main.o ply.o $(LINKFLTK_FLGS) -lglut -lGLU -lGL 

clean:
	@rm -rf l2.app l2 *.o *~ 
