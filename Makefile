# Variables
ODIR=obj
CC = gcc
CXX = g++
CXXFLAGS = -g -Wall
OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

SRCS = detect1.cpp
PROG = detect1 

DisplayImage: DisplayImage.cpp
	$(CXX) $(CXXFLAGS) DisplayImage.cpp $(LIBS) -o DisplayImage 

detect: detect1.cpp
	$(CXX) $(CXXFLAGS) detect1.cpp $(LIBS) -o detect 
	
# $(PROG): $(SRCS)
# 	$(CXX) $(CXXFLAGS) -o $(PROG) $(SRCS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(PROG) *.o