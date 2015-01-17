#include "dht.h"

struct server *md5_servers[MAX_SERVERS_NO],
	      *sha1_servers[MAX_SERVERS_NO];
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


/* Lookup a hash in a sorted list of servers
 * Determine the server whose ID is greater
 * than the hash but closest to id */
static int binary_search(struct server **servers, unsigned char *hash, int type)
{
	int result, middle, x, y;
	int start = 0, end = servers_no - 1;

	/* Compare to the limits */
	if (type == MD5_HASH) {
		x = compare_hash(hash, servers[0]->md5, MD5_SIZE);
		y = compare_hash(hash, servers[servers_no - 1]->md5, MD5_SIZE);
	}
	if (type == SHA1_HASH) {
		x = compare_hash(hash, servers[0]->sha1, SHA1_SIZE);
		y = compare_hash(hash, servers[servers_no - 1]->sha1, SHA1_SIZE);
	}
	if (x < 0 || y > 0)
		return 0;

	/* Perform binary search */
	printf("Search between %d and %d\n", start, end);
	while (end - start > 1) {
		middle = (end + start) / 2;
		if (type == MD5_HASH)
			x = compare_hash(hash, servers[middle]->md5, MD5_SIZE);
		if (type == SHA1_HASH)
			x = compare_hash(hash, servers[middle]->sha1, SHA1_SIZE);

		if (x < 0)
			end = middle;
		else
			start = middle;
		printf("Search between %d and %d\n", start, end);
	}

	return end;
}


/* Get the index of a server using MD5 as the hash function */
static void get_md5_index(char *key, int *index)
{
	int i, x;
	unsigned char md5[MD5_SIZE];
	unsigned char *md5_successor = NULL,
		      *md5_smallest = NULL;


	/* Compute MD5 hash of the key */
	MD5(key, strlen(key), md5);
	print_hash(md5, MD5_SIZE);

	*index = binary_search(md5_servers, md5, MD5_HASH);
}


/* Get the index of a server using SHA1 as the hash function */
static void get_sha1_index(char *key, int *index)
{
	int i, x;
	unsigned char sha1[SHA1_SIZE];
	unsigned char *sha1_successor = NULL,
		      *sha1_smallest = NULL;

	/* Compute SHA1 hash of the key */
	SHA1(key, strlen(key), sha1);
	print_hash(sha1, SHA1_SIZE);

	*index = binary_search(sha1_servers, sha1, SHA1_HASH);
}


/* Execute the PUT command */
void put_cmd(char *key, char *val)
{
	int i, idx_md5, idx_sha1;

	printf("Put command, key %s, len %d, val %s.\n",
			key, (int)strlen(key), val);

	get_md5_index(key, &idx_md5);
	get_sha1_index(key, &idx_sha1);
	printf("Got server %d for MD5 hashing\n", idx_md5);
	printf("Got server %d for SHA1 hashing\n", idx_sha1);
}


/* Execute the PUT command */
void get_cmd(char *key)
{
	int i, idx_md5, idx_sha1;
	printf("Get command, key %s, len %d\n", key, (int)strlen(key));

	get_md5_index(key, &idx_md5);
	get_sha1_index(key, &idx_sha1);
	printf("Got server %d for MD5 hashing\n", idx_md5);
	printf("Got server %d for SHA1 hashing\n", idx_sha1);
}


/* Sort the servers based on their hash values */
static void sort_servers(struct server **vector, int type)
{
	int i, j, x;
	struct server *aux;

	if (type != MD5_HASH && type != SHA1_HASH) {
		printf("Unknown hash type\n");
		return;
	}

	for (i = 0; i < servers_no - 1; i++)
		for (j = i + 1; j < servers_no; j++) {

			if (type == MD5_HASH)
				x = compare_hash(vector[i]->md5,
					vector[j]->md5, MD5_SIZE);

			if (type == SHA1_HASH)
				x = compare_hash(vector[i]->sha1,
					vector[j]->sha1, SHA1_SIZE);

			if (x > 0) {
				aux = vector[i];
				vector[i] = vector[j];
				vector[j] = aux;
			}
		}
}


/* Compute IP MD5 and SHA1 hashes for each server */
void compute_servers_hashes(int nr_servers)
{
	int i;
	unsigned char md5[MD5_SIZE];
	unsigned char sha1[SHA1_SIZE];

	servers_no = nr_servers;
	for (i = 0; i < servers_no; i++) {
		MD5(servers[i].ip, strlen(servers[i].ip), md5);
		SHA1(servers[i].ip, strlen(servers[i].ip), sha1);

		memcpy(servers[i].md5, md5, MD5_SIZE);
		memcpy(servers[i].sha1, sha1, SHA1_SIZE);

		md5_servers[i] = &servers[i];
		sha1_servers[i] = &servers[i];
	}


	/* Sort the servers based on their hash values
	 * We keep two separate list of pointers to the servers
	 * based on the hashing function used */
	sort_servers(md5_servers, MD5_HASH);
	sort_servers(sha1_servers, SHA1_HASH);


	/* Print the sorted list of servers, to remove this */
	printf("\n");
	for (i = 0; i < servers_no; i++) {
		printf("\n%d MD5 server %s, SHA1 server %s\n",
				i, md5_servers[i]->ip, sha1_servers[i]->ip);
		print_hash(md5_servers[i]->md5, MD5_SIZE);
		print_hash(sha1_servers[i]->sha1, SHA1_SIZE);
	}
}
