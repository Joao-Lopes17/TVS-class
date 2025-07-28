#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCKET_PATH "/run/isel/tvsctld/request/tvsctld.socket"

#define START_CODE 2
#define STOP_CODE 3
#define STOP_DB_CODE 4
#define STATUS_CODE 5
#define RESET_CODE 6
#define INC_CODE 7
#define DEC_CODE 8


void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, const char * argv[]) {
	// These must always be the first instructions. DON'T EDIT
	close(0); open("/dev/null", O_RDONLY);       // DON'T EDIT

	if (argc < 2) {
		fprintf(stderr, "not enough arguments\n");
		exit(1);
	}


	puts(":: START ::");
	
	int conn_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (conn_fd < 0) {
		error("ERROR creating socket");
	}

	struct sockaddr_un srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	strncpy(srv_addr.sun_path, SOCKET_PATH, sizeof(srv_addr.sun_path) - 1);

	if (connect(conn_fd, (struct sockaddr *)&srv_addr, sizeof (srv_addr)) < 0) {
		error("ERROR connecting socket");
	} 
	
	puts(":: CONNECTED ::");
	unsigned char command;
	if (strcmp(argv[1], "start") == 0) {
        command = START_CODE;
    } else if (strcmp(argv[1], "stop") == 0) {
        command = (argc == 3 && strcmp(argv[2], "-db") == 0) ? STOP_DB_CODE : STOP_CODE;
    } else if (strcmp(argv[1], "status") == 0) {
        command = STATUS_CODE;
    } else if (strcmp(argv[1], "reset") == 0) {
        command = RESET_CODE;
/*         if (argc >= 3) options[0] = (unsigned char)atoi(argv[2]);
        if (argc == 4) options[1] = (unsigned char)atoi(argv[3]);
        options_len = (argc - 2); */
    } else if (strcmp(argv[1], "inc") == 0) {
        command = INC_CODE;
/*         if (argc == 3) {
            options[0] = (unsigned char)atoi(argv[2]);
            options_len = 1;
        } */
    } else if (strcmp(argv[1], "dec") == 0) {
        command = DEC_CODE;
/*         if (argc == 3) {
            options[0] = (unsigned char)atoi(argv[2]);
            options_len = 1;
        } */
    } else {
        fprintf(stderr, "Invalid command: %s.\n", argv[1]);
        exit(1);
    }
	if (write(conn_fd, &command, 1) < 0) {
		error("ERROR writing to socket");
	}
	switch (command){
		case INC_CODE:
		case DEC_CODE:
			if (argc == 3) {
				size_t delta = (size_t)atoi(argv[2]);
				if (write(conn_fd, &delta, sizeof delta) < 0){ 
					error("ERROR writing data to socket");
				}
			}
			break;
		case RESET_CODE:
			if (argc >= 3) {
				size_t scale = (size_t)atoi(argv[2]);
				if (write(conn_fd, &scale, sizeof scale) < 0){ 
					error("ERROR writing data to socket");
				}
				if (argc >= 4) {
					size_t base = (size_t)atoi(argv[3]);
					if (write(conn_fd, &base, sizeof base) < 0){ 
						error("ERROR writing data to socket");
					}
				}
			}
			break;
		default:
			break;
	}
	
	close(conn_fd);
	puts(":: END ::");
	return 0;
}
