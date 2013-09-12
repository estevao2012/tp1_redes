all: compila executa

compila:
	gcc -c main.c
	gcc main.c -o executavel
server:
	gcc -c simple_server.c
	gcc simple_server.o -o servidor
	./servidor
executa:
	./executavel 

