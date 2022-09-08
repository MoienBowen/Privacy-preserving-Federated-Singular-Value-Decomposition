CC = gcc
CFLAGS = -Wall -c -pthread -DPAILLIER_THREAD -fpic
DEPS = ./../include/paillier.h tools.h
OBJ_LIB = tools.o paillier.o paillier_manage_keys.o paillier_io.o
OBJ_INTERPRETER = main.o

#standaloine command interpreter executable recipe	
paillier_exe: main.o $(OBJ_LIB)
	$(CC) -Wall -o $@ $^ -lgmp

#clean project
.PHONY: clean
clean:
	rm -fr ./*.o

all: paillier_exe 

