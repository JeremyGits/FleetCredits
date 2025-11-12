# Fleet Credits Regtest Miner - C++ Version
# (Future implementation - for now, use Python version)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = -lcurl -ljsoncpp

TARGET = fc_miner
SOURCES = fc_miner.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Note: C++ version not yet implemented
# Use Python version (fc_miner.py) for now

