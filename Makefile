
CXX = g++
CXXFLAGS = -Wall -std=c++11
LIBS = -lcrypto++

SOURCES = main.cpp AESStringEncryption.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = myprogram

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)