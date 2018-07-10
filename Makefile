# Variables
ODIR=obj
CC = gcc
CXX = g++
CXXFLAGS = -g -Wall
OPENCV = `pkg-config --cflags --libs opencv`
LIBS = $(OPENCV)

SRCS = detect1.cpp
PROG = detect1 

DisplayImage: DisplayImage.cpp
	$(CXX) $(CXXFLAGS) DisplayImage.cpp $(LIBS) -o ./output/DisplayImage 

detect: detect.cpp
	$(CXX) $(CXXFLAGS) detect.cpp $(LIBS) -o ./output/detect 

drawCircle: drawCircle.cpp
	$(CXX) $(CXXFLAGS) drawCircle.cpp $(LIBS) -o ./output/drawCircle 
# $(PROG): $(SRCS)
# 	$(CXX) $(CXXFLAGS) -o $(PROG) $(SRCS) $(LIBS)
homography: homography.cpp
	$(CXX) $(CXXFLAGS) homography.cpp $(LIBS) -o ./output/homography 

virtual-billboard: virtual-billboard.cpp
	$(CXX) $(CXXFLAGS) virtual-billboard.cpp $(LIBS) -o ./output/virtual-billboard 

facedectect: facedectect.cpp
	$(CXX) $(CXXFLAGS) facedectect.cpp $(LIBS) -o ./output/facedectect

handwrite: handwrite.cpp
	$(CXX) $(CXXFLAGS) handwrite.cpp $(LIBS) -o ./output/handwrite

ocr: ./OCR/simple.cpp
	$(CXX) $(CXXFLAGS) ./OCR/simple.cpp `pkg-config --cflags --libs opencv tesseract ` -o ./output/ocr-simple

eigenface: ./eigenface.cpp
	$(CXX) $(CXXFLAGS) ./eigenface.cpp `pkg-config --cflags --libs opencv` -o ./output/eigenface

.PHONY: clean

clean:
	rm -f $(PROG) *.o