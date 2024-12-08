CC = gcc
CFLAGS = -Wall
SERVER = server
CLIENT = client

.PHONY:
.SILENT:

all: $(SERVER) $(CLIENT)

$(SERVER): server.c
	$(CC) $(CFLAGS) -o $(SERVER) server.c

$(CLIENT): client.c
	$(CC) $(CFLAGS) -o $(CLIENT) client.c

run_server:
	gnome-terminal -- ./$(SERVER)

run_client:
	gnome-terminal -- ./$(CLIENT)

run: all run_server run_client

clean:
	rm -f $(SERVER) $(CLIENT)
