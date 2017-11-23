all:
	g++ -L./lib64 -Wall -o bin/drpc main.c -ldiscord-rpc -lpthread

