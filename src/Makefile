CC=gcc
LDFLAGS=-lcrypto -lmemcached
CFLAGS=-Wall

dht:
	$(CC) dht_client.c dht.c -o dht_client $(LDFLAGS)

clean:
	rm dht_client
