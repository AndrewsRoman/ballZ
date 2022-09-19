CFLAGS = -Wall -std=c99 
ALLEGRO = -lallegro_image  -lallegro_primitives  -lallegro_dialog  -lallegro_ttf -lallegro_font  -lallegro_audio -lallegro_acodec -lallegro 
OFLAGS = -lm
all: ballZ

ballZ: ballZ.o menu.o fase.o structs.o
		gcc $(CFLAGS) ballZ.o menu.o fase.o structs.o -o ballZ $(ALLEGRO) $(OFLAGS)
		
ballZ.o: ballZ.c
		gcc -c $(CFLAGS) ballZ.c
		
menu.o: menu.c menu.h
		gcc -c $(CFLAGS) menu.c
		
fase.o: fase.c fase.h
		gcc -c $(CFLAGS) fase.c

structs.o: structs.c structs.h
		gcc -c $(CFLAGS) structs.c
		
run:
		./ballZ 
		
valgrind:
		valgrind ./ballZ

gdb:
	gdb ./ballZ
		

clean:
		-rm -f *.o

purge: clean
		-rm -f ballZ 
