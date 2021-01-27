CXXFLAGS = -Wall -std=c++11 -O2 -DNO_FREETYPE -I $(HOME)/libs/include 

all: main.exe

clean:
	rm -f main.exe *.o

main.exe: main.o Point.o ConvexPolygon.o
	$(CXX) $^ -o $@ -L $(HOME)/libs/lib -l PNGwriter -l png

main.o: main.cc Point.hh ConvexPolygon.hh

Point.o: Point.cc Point.hh

ConvexPolygon.o: ConvexPolygon.cc ConvexPolygon.hh

#  -o 