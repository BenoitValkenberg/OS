all: osproject

osproject: main.o seqgen.o 
	gcc -o seqgen main.o seqgen.o 

main.o: main.c
	gcc -o main.o -c main.c -W -Wall -pedantic

seqgen.o: seqgen.c
	gcc -o seqgen.o -c seqgen.c -W -Wall -pedantic

