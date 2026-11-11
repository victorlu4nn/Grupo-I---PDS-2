CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include

EXECUTABLE	:= main

SOURCES := $(wildcard $(SRC)/*.cpp $(SRC)/base/*.cpp $(SRC)/games/*.cpp)

all: $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SOURCES)
	-mkdir $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE)/base -I$(INCLUDE)/games $^ -o $@

clean:
	-rm $(BIN)/*
