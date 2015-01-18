#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dht_client.h"
#include "dht.h"

static char *usage = "DHT. Usage:\n> put key value;\n\
> get key;\n> exit;";

static void generate_data(char **data)
{
	data = malloc(MEGA * sizeof(char));
	if (!data) {
		printf("Unable to allocate data\n");
		return;
	}

	printf("Initializing data\n");

}


static void test_throughput(void)
{
	char *data;
	printf("Testing the throughput\n");

	generate_data(&data);

}


static void execute_command(char cmds[CMD_MAX_WORDS][CMD_SIZE], int len)
{
	char *value;
	int ret, i;
	size_t value_length;

	if (len == 0)
		return;

	/* Execute the PUT command */
	if (!strncmp(cmds[0], PUT_CMD, strlen(PUT_CMD))) {

		/* Check arguments */
		if (len < 3) {
			pr_msg("Unknown argument");
			printf("%s\n", usage);
			return;
		}

		if ((ret = put_cmd(cmds[1], cmds[2])))
			pr_msg("Unable to put value\n");
		return;
	}

	/* Execute the GET command */
	if (!strncmp(cmds[0], GET_CMD, strlen(GET_CMD))) {

		/* Check arguments */
		if (len < 2) {
			pr_msg("Unknown argument");
			printf("%s\n", usage);
			return;
		}

		if ((ret = get_cmd(cmds[1], &value, &value_length))) {
			pr_msg("Unable to get the value");
			return;
		}
		successful_gets++;
		pr_msg(value);
		return;
	}

	/* Execute the STAT command */
	if (!strncmp(cmds[0], STAT_CMD, strlen(STAT_CMD))) {
		printf("\n");
		for (i = 0; i < servers_no; i++)
			printf("Server %d, objects %lu\n", i, servers[i].objects);
		printf("Successful gets: %d\n", successful_gets);

		return;
	}

	/* Execute the EXIT command */
	if (!strncmp(cmds[0], EXIT_CMD, strlen(EXIT_CMD)))
		exit(0);
}


static int parse_command(char *line)
{
	int count = 0;
	char cmds[CMD_MAX_WORDS][CMD_SIZE];
	char *word = strtok(line, " \n");

	/* Parse the command and find the arguments */
	while (word) {
		if (count >= CMD_MAX_WORDS)
			break;

		strcpy(cmds[count], word);
		
		word = strtok(NULL, " \n");
		count++;
	}

	execute_command(cmds, count);

	return 0;
}


/* Read configuration file */
static int read_cfg_file(int *servers_no)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int lines = 0;

	if ((fp = fopen(CONFIG_FILE, "r")) == NULL) {
		printf("Error opening config file %s\n", CONFIG_FILE);
		return 1;
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		line[strlen(line) - 1] = 0;

		strncpy(servers[lines++].ip, line, strlen(line));
	}

	fclose(fp);
	if (line)
		free(line);
	*servers_no = lines;
	return 0;
}


/* Read servers' IPs, compute hashes, connect to servers */
static int init_servers(void)
{
	int servers_no;

	/* Read IP addresses of all the servers */
	if (read_cfg_file(&servers_no)) {
		printf("Error initializing servers\n");
		return 1;
	}
	printf("Read %d servers\n", servers_no);

	compute_servers_hashes(servers_no);

	return 0;
}

int main(int argc, char **argv) {
	char line[CMD_SIZE];

	if (init_servers())
		return 1;

	if (argc == 2 && !strcmp(argv[1], THROUGHPUT))
		test_throughput();

	printf("%s\n", usage);
	pr_console();

	/* Read commands from stdin */
	while (fgets(line, sizeof(line), stdin)) {

		/* Execute the given command */
		if (parse_command(line))
			break;

		pr_console();
	}

	return 0;
}
