CXX = g++
CXXFLAGS = -Wall -std=c++17

SRC = code/add-binary.cpp code/Number.cpp
HDR = code/Number.h code/conf.h
BIN = build/add-binary

$(BIN): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC)

.PHONY: clean install

clean:
	rm -f $(BIN)

install: $(BIN)
	cp $(BIN) /usr/local/bin/
