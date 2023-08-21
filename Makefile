CXX = g++

CXXFLAGS = -Wall -std=c++11

LIBS =  -lcrypto++

myprogram: main.cpp
	$(CXX) $(CXXFLAGS) -o myprogram main.cpp $(LIBS)


clean:
	rm -f *.o myprogram
