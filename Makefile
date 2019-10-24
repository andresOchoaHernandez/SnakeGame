all: main

main: 
	@g++ -Wall  main.cpp -o game -lncurses
	@gnome-terminal -- ./game;
	@clear;
	
clean:
	@rm game