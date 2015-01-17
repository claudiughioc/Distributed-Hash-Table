#ifndef __DHT_CLIENT_H__
#define __DHT_CLIENT_H__


#define CMD_SIZE		128
#define CMD_MAX_WORDS		3
#define PUT_CMD			"put"
#define GET_CMD			"get"
#define EXIT_CMD		"exit"

#define CONFIG_FILE		"servers.cfg"

#define pr_console()		printf("> ")
#define pr_msg(message)		printf("> %s\n", message)


#endif
