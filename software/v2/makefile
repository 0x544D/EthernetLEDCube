
main: main.c cube.c	
	gcc -c -o cube.o cube.c -lm
	gcc -c -o draw.o draw.c -lm
	gcc -c -o snake.o pattern_snake.c
	gcc -c -o fade3d.o pattern_fade3d.c
	gcc -c -o coloredge.o pattern_coloredge.c
	gcc -c -o drop.o pattern_drop.c
	gcc -c -o pattern_flash.o pattern_flash.c
	gcc -o cubecontrol main.c cube.o draw.o fade3d.o snake.o coloredge.o drop.o pattern_flash.o -Wall -O3 `pkg-config --cflags --libs gtk+-3.0` -rdynamic -lm
	rm -f *.o
	./cubecontrol

raweth: sendRawEth.c
	gcc -o raweth sendRawEth.c -Wall -pthread -lm
	./raweth

raweth_single: sendRawEthSingle.c
	gcc -o raweth_single sendRawEthSingle.c -Wall
	./raweth_single
