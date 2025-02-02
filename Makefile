WWW=-Wall -Werror -Wextra
CC=gcc

pong: pong.c clean
	$(CC) $(WWW) pong.c -o pong
	./pong

pong_interactive: clean
	$(CC) $(WWW) pong_interactive.c -o pong_interactive -lncurses
	./pong_interactive

pong_threads: clean
	$(CC) $(WWW) pong_multi_threads.c -o pong_threads -lncurses
	./pong_threads

clean:
	rm -rf *.o *.out pong pong_interactive pong_threads