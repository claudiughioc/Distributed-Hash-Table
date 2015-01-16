#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dht_client.h"

static char servers[MAX_SERVERS_NO][MAX_SERVER_LEN];
static char *usage = "DHT. Usage:\n> put key value;\n\
> get key;\n> exit;";

/* Execute the PUT command */
static void put_cmd(char *key, char *val)
{
	printf("Put command, key %s, val %s.\n", key, val);
}


/* Execute the PUT command */
static void get_cmd(char *key)
{
	printf("Get command, key %s.\n", key);
}


static void execute_command(char cmds[CMD_MAX_WORDS][CMD_SIZE], int len)
{
	/* Execute the PUT command */
	if (!strncmp(cmds[0], PUT_CMD, strlen(PUT_CMD))) {

		/* Check arguments */
		if (len < 3) {
			pr_msg("Unknown argument");
			printf("%s\n", usage);
			return;
		}

		put_cmd(cmds[1], cmds[2]);
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

		get_cmd(cmds[1]);
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
static int read_cfg_file(void)
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

		strncpy(servers[lines++], line, strlen(line));
	}

	fclose(fp);
	if (line)
		free(line);
	return 0;
}

int main(int argc, char **argv) {
	char line[CMD_SIZE];

	if (read_cfg_file())
		return 1;

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
