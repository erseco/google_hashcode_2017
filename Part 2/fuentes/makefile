CFLAGS = -O1 -g -W -Wall -I$(INC)

SRC = ./src/
INC = ./include/
OBJ = ./obj/
BIN = ./bin/

TARGETS = $(BIN)greedy

default : $(TARGETS)

$(BIN)greedy :
	g++ $(CFLAGS) $(SRC)greedy.cpp -o $(BIN)greedy

clean:
	rm -f $(OBJ)*.o
	rm -f $(BIN)*

redo:
	touch $(SRC)*.cpp
	make
