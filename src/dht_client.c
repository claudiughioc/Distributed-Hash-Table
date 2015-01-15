#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dht_client.h"


static char *usage = "DHT. Usage:\n> put key value;\n\
> get key;\n> exit;";

static void execute_command(char cmds[CMD_MAX_WORDS][CMD_SIZE])
{

}

static int parse_command(char *line)
{
	int count = 0;
	char cmds[CMD_MAX_WORDS][CMD_SIZE];
	char *word = strtok(line, " ");

	/* Parse the command and find the arguments */
	while (word) {
		if (count >= CMD_MAX_WORDS)
			return 0;

		strcpy(cmds[count], word);
		
		word = strtok(NULL, " ");
		count++;
	}

	execute_command(cmds);

	return 0;
}

int main(int argc, char **argv) {
	char line[CMD_SIZE];


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
