all: client server
	
client:
	gcc Client/client.c -o client

server:
	gcc Server/server_thread.c -o server -pthread

clean:
	@rm -rf ./client ./server
