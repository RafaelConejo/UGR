INC = include
OBJ = obj
BIN = bin
CC = g++

CFLAGS=-I$(INC)/ -g -O1


all: PAR

$(OBJ)/utiles.o: $(BIN)/utiles.cpp $(INC)/utiles.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(OBJ)/evaluacion.o: $(BIN)/evaluacion.cpp $(INC)/evaluacion.h
		$(CC) $< -c -o $@ $(CFLAGS)

$(OBJ)/AG.o: $(BIN)/AG.cpp $(INC)/AG.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(OBJ)/random.o: $(BIN)/random.cpp $(INC)/random.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(OBJ)/AM.o: $(BIN)/AM.cpp $(INC)/AM.h
	$(CC) $< -c -o $@ $(CFLAGS)

PAR: obj/utiles.o obj/evaluacion.o obj/AG.o obj/AM.o obj/random.o bin/main.cpp
	$(CC) bin/main.cpp obj/utiles.o obj/evaluacion.o obj/AG.o obj/AM.o obj/random.o -o $@ $(CFLAGS)

clean:
	rm obj/*
