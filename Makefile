all: compila

compila:
	gcc -c main.c
	gcc main.c -o executavel

executa:
	./executavel < teste