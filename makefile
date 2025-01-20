.PHONY: clean run run_server run_client

run: run_server run_client

run_server: server
	@./server

run_client: client
	@./client

clean:
	@rm -f *.o server client game_save.dat

server: server.o grid.o
	@gcc -pthread -o server server.o grid.o

client: client.o grid.o
	@gcc -o client client.o grid.o

grid.o: game.c grid.h
	@gcc -c game.c -o grid.o

server.o: server.c grid.h
	@gcc -c server.c

client.o: client.c grid.h
	@gcc -c client.c