all: ejecutable

ejecutable: main.o Planificador.o
	$(CC) -g -o ejecutable *.o Lista.c Lista.h Nodo.c Nodo.h Variables.h `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lm -lquadmath
run:
	./ejecutable
main.o:
	$(CC) -c main.c Variables.h `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lm -lquadmath
Planificador.o:
	$(CC) -c Planificador.h Planificador.c Variables.h `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lm -lquadmath 

debug: 
	valgrind --leak-check=full --track-origins=yes ./ejecutable

clean:
	rm -rf *.o ejecutable *.h.gch
	clear