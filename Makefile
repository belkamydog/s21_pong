WWW=
# -Wall -Werror -Wextra
CC=gcc

pong: pong.c clean
	$(CC) $(WWW) pong.c -o pong
	./pong

pong_interactive: clean
	$(CC) $(WWW) pong_interactive.c -o pong_interactive -lncurses
	./pong_interactive

clean:
	rm -rf *.o pong pong_interactive