COMPILER=gcc
CPPFLAGS=-std=c++11
OPENCVLIBS=-lopencv_core -lopencv_imgcodecs

all: example.o opencv_pfm.o 
	$(COMPILER) $(CPPFLAGS) -o example example.o opencv_pfm.o $(OPENCVLIBS)
	rm -f example.o opencv_pfm.o

example.o: example.cpp src/opencv_pfm.hpp
	$(COMPILER) $(CPPFLAGS) -c example.cpp

opencv_pfm.o: src/opencv_pfm.cpp src/opencv_pfm.hpp
	$(COMPILER) $(CPPFLAGS) -c src/opencv_pfm.cpp

clean:
	rm -f example example.o opencv_pfm.o