all: serial paralela clone fork

serial: serial.c
	chmod +x init.c
	gcc -o init init.c -lm

paralela: paralela.c
	chmod +x paralela.c
	gcc -o paralela paralela.c -lm -pthread

clone: clone.c
	chmod +x clone.c
	gcc -o clone clone.c -lm


fork: fork.c
	chmod +x fork.c
	gcc -o fork fork.c -lm
