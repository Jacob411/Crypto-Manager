CXX = g++
CXXFLAGS = -Wall -std=c++11
LIBS = -lcrypto++ 

SOURCES = AESFileEncryption.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = crypto-manager

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
