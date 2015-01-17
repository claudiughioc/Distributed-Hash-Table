#ifndef __DHT_H__
#define __DHT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/md5.h>

#define MAX_SERVERS_NO		10
#define MAX_IP_LEN		33

#define MD5_SIZE		16
#define SHA1_SIZE		20
#define MD5_HASH		0
#define SHA1_HASH		1

struct server {
	unsigned char ip[MAX_IP_LEN];
	unsigned char md5[MD5_SIZE];
	unsigned char sha1[SHA1_SIZE];
	int enabled;
};

extern struct server servers[MAX_SERVERS_NO];

void put_cmd(char *key, char *val);
void get_cmd(char *key);
void compute_servers_hashes(int servers_no);

#endif
