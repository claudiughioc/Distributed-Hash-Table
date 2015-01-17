#include "dht.h"

struct server servers[MAX_SERVERS_NO];
int servers_no, md5_first, sha1_first;

static void print_hash(unsigned char *hash, int len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		printf("%02x", hash[i]);
	}
	printf(".\n");
}


static int compare_hash(unsigned char *hash1, unsigned char *hash2, int len)
{
	int result, i;

	for (i = 0; i < len; i++) {
		if (hash1[i] < hash2[i])
			return -1;

		if (hash1[i] > hash2[i])
			return 1;
	}

	return 0;
}


static void get_md5_index(char *key, int *index)
{
	int i, x;
	unsigned char md5[MD5_SIZE];
	unsigned char *md5_successor = NULL,
		      *md5_smallest = NULL;

	/* Compute MD5 hash of the key */
	MD5(key, strlen(key), md5);
	print_hash(md5, MD5_SIZE);

	for (i = 0; i < servers_no; i++) {
		x = compare_hash(md5, servers[i].md5, MD5_SIZE);

		if (x >= 0)
			continue;

		/* Server has ID greater than key */
		if (!md5_successor) {
			md5_successor = servers[i].md5;
			*index = i;
		} else {
			x = compare_hash(servers[i].md5,
					md5_successor, MD5_SIZE);
			if (x < 0) {
				md5_successor = servers[i].md5;
				*index = i;
			}
		}
	}

	if (!md5_successor)
		*index = md5_first;
}


static void get_sha1_index(char *key, int *index)
{
	int i, x;
	unsigned char sha1[SHA1_SIZE];
	unsigned char *sha1_successor = NULL,
		      *sha1_smallest = NULL;

	/* Compute SHA1 hash of the key */
	SHA1(key, strlen(key), sha1);

	for (i = 0; i < servers_no; i++) {
		x = compare_hash(sha1, servers[i].sha1, SHA1_SIZE);

		if (x >= 0)
			continue;

		/* Server has ID greater than key */
		if (!sha1_successor) {
			sha1_successor = servers[i].sha1;
			*index = i;
		} else {
			x = compare_hash(servers[i].sha1,
					sha1_successor, SHA1_SIZE);
			if (x < 0) {
				sha1_successor = servers[i].sha1;
				*index = i;
			}
		}
	}

	if (!sha1_successor)
		*index = sha1_first;
}


/* Execute the PUT command */
void put_cmd(char *key, char *val)
{
	int i, idx_md5, idx_sha1;

	printf("Put command, key %s, len %d, val %s.\n",
			key, (int)strlen(key), val);

	get_md5_index(key, &idx_md5);
	printf("Got server %d for MD5 hashing\n", idx_md5);
}


/* Execute the PUT command */
void get_cmd(char *key)
{
	int i, idx_md5, idx_sha1;
	printf("Get command, key %s, len %d\n", key, (int)strlen(key));

}


/* Compute IP MD5 and SHA1 hashes for each server */
void compute_servers_hashes(int nr_servers)
{
	int i, x;
	unsigned char md5[MD5_SIZE];
	unsigned char sha1[SHA1_SIZE];
	unsigned char *md5_smallest = NULL,
		      *sha1_smallest = NULL;

	servers_no = nr_servers;
	for (i = 0; i < servers_no; i++) {
		x = strlen(servers[i].ip);
		MD5(servers[i].ip, strlen(servers[i].ip), md5);
		SHA1(servers[i].ip, strlen(servers[i].ip), sha1);

		memcpy(servers[i].md5, md5, MD5_SIZE);
		memcpy(servers[i].sha1, sha1, SHA1_SIZE);

		/* Compute the first MD5 server */
		if (md5_smallest) {
			x = compare_hash(md5, md5_smallest, MD5_SIZE);
			if (x < 0) {
				md5_smallest = servers[i].md5;
				md5_first = i;
			}
		} else {
			md5_smallest = servers[i].md5;
			md5_first = i;
		}

		/* Compute the first MD5 server */
		if (sha1_smallest) {
			x = compare_hash(sha1, sha1_smallest, SHA1_SIZE);
			if (x < 0) {
				sha1_smallest = servers[i].sha1;
				sha1_first = i;
			}
		} else {
			sha1_smallest = servers[i].sha1;
			sha1_first = i;
		}


		printf("Server %d:\n", i);
		print_hash(md5, MD5_SIZE);
		print_hash(sha1, SHA1_SIZE);
	}
	printf("First MD5 is %d\n", md5_first);
	printf("First SHA1 is %d\n", sha1_first);
}
