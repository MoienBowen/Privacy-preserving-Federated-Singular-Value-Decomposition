CC = gcc
CFLAGS = -Wall -O3 -c -pthread -DPAILLIER_THREAD -fpic
DEPS = ./include/paillier.h ./paillier/tools.h ./include/SVD_Power_Method.h ./include/randomvector.h
OBJ_LIB = ./paillier/tools.o ./paillier/paillier.o ./paillier/paillier_manage_keys.o ./paillier/paillier_io.o ./SVD_Power_Method.o ./noise/randomvector.c

OBJ_INTERPRETER = main.o

#standaloine command interpreter executable recipe	
federated_SVD_exe: main.o $(OBJ_LIB)
	$(CC) -Wall -o $@ $^ -lgmp -lsodium

#clean project
.PHONY: clean

clean:
	rm -f $(binaries) *.o paillier/*.o federated_SVD_exe

all: federated_SVD_exe 

