CFLAGS = -O1 -g -W -Wall -I$(INC)

SRC = ./src/
INC = ./include/
OBJ = ./obj/
BIN = ./bin/

TARGETS = $(BIN)greedy

default : $(TARGETS)

$(OBJ)functions.o : $(SRC)functions.cpp $(INC)functions.h
	g++ -c $(CFLAGS) $(SRC)functions.cpp -o $(OBJ)functions.o

$(BIN)greedy : $(OBJ)functions.o $(SRC)greedy.cpp
	g++ $(CFLAGS) $(OBJ)functions.o $(SRC)greedy.cpp -o $(BIN)greedy

clean:
	rm -f $(OBJ)*.o
	rm -f $(BIN)*

redo:
	touch $(SRC)*.cpp
	make
