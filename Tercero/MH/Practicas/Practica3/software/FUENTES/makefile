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

$(OBJ)/trayectorias_simples.o: $(BIN)/trayectorias_simples.cpp $(INC)/trayectorias_simples.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(OBJ)/random.o: $(BIN)/random.cpp $(INC)/random.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(OBJ)/trayectorias_multiples.o: $(BIN)/trayectorias_multiples.cpp $(INC)/trayectorias_multiples.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(OBJ)/hibrido.o: $(BIN)/hibrido.cpp $(INC)/hibrido.h
	$(CC) $< -c -o $@ $(CFLAGS)

PAR: obj/utiles.o obj/evaluacion.o obj/trayectorias_simples.o obj/trayectorias_multiples.o obj/hibrido.o obj/random.o bin/main.cpp
	$(CC) bin/main.cpp obj/utiles.o obj/evaluacion.o obj/trayectorias_simples.o obj/trayectorias_multiples.o obj/hibrido.o obj/random.o -o $@ $(CFLAGS)

clean:
	rm obj/*
