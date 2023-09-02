
CXX = g++
CXXFLAGS = -Wall -std=c++11
LIBS = -lcrypto++ -lncurses -lmenu 

SOURCES = AESFileEncryption.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = AESFileEncryption

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)