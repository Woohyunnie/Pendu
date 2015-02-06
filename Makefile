all: client server
	
client:
	gcc Client/client.c -o client

server:
	gcc Server/server_thread.c -o server_thread -pthread

clean:
	@rm -rf client server
