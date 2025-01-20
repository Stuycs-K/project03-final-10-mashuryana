.PHONY: clean run compile

run: game
	@./game
clean: 
	@rm -f *.o game
compile: game
game: server.o grid.o 
	@gcc -o game server.o grid.o 
server.o: server.c grid.h
	@gcc -c server.c 
grid.o: grid.c grid.h
	@gcc -c grid.c
