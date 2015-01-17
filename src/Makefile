CC=gcc
LDFLAGS=-lcrypto
CFLAGS=-Wall

dht:
	$(CC) dht_client.c dht.c -o dht_client $(LDFLAGS)

clean:
	rm dht_client
