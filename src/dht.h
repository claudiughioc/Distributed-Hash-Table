#ifndef __DHT_H__
#define __DHT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/md5.h>
#include <openssl/md5.h>
#include <libmemcached/memcached.h>

#define MAX_SERVERS_NO		10
#define MAX_IP_LEN		33

#define MD5_SIZE		16
#define SHA1_SIZE		20
#define MD5_HASH		0
#define SHA1_HASH		1

#define MEMCACHED_PUT		0
#define MEMCACHED_GET		1
#define ERR_NO_CONN		-1
#define ERR_NOT_FOUND		-2

struct server {
	unsigned char ip[MAX_IP_LEN];
	unsigned char md5[MD5_SIZE];
	unsigned char sha1[SHA1_SIZE];
	unsigned long objects;
	int enabled;
};

extern struct server servers[MAX_SERVERS_NO];
extern int servers_no, successful_gets;

int put_cmd(char *key, char *val);
int get_cmd(char *key, char **val, size_t *val_len);
void compute_servers_hashes(int servers_no);

#endif
