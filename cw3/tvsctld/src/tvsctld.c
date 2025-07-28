#include <systemd/sd-daemon.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/wait.h>

#define START_CODE 2
#define STOP_CODE 3
#define STOP_DB_CODE 4
#define STATUS_CODE 5
#define RESET_CODE 6
#define INC_CODE 7
#define DEC_CODE 8

#define START_FILE_PATH "./tvsapp-start.sh"
#define STOP_FILE_PATH "./tvsapp-stop.sh"
#define STATUS_FILE_PATH "./tvsapp-status.sh"
#define RESET_FILE_PATH "./tvsapp-reset.sh"
#define INC_FILE_PATH "./tvsapp-inc.sh"
#define DEC_FILE_PATH "./tvsapp-dec.sh"

void terror(char *msg) {
	perror(msg);
	pthread_exit((void *)(intptr_t)1);
}


void execute_script(const char *script, const char *arg1, const char *arg2) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        if (arg1 && arg2) {
            execlp("/bin/bash", "bash", script, arg1, arg2, NULL);
        } else if (arg1) {
            execlp("/bin/bash", "bash", script, arg1, NULL);
        } else {
            execlp("/bin/bash", "bash", script, NULL);
        }
        perror("Error executing script");
        exit(1);
    }
    int ret = 0;
    waitpid(pid, &ret, 0);
}

void * process_connection(void * ptr) {
	int conn_fd = (int)(intptr_t)ptr;
	unsigned char command;

	if (read(conn_fd, &command, 1) <= 0) {
		perror("ERROR reading from socket");
		close(conn_fd);
		return NULL;
	}
    
	printf("Received command: %d\n", command);
    
	switch (command) {
        case START_CODE:
            printf("Received START command\n");
            execute_script(START_FILE_PATH, NULL, NULL);
            break;

        case STOP_CODE:
            printf("Received STOP command\n");
            execute_script(STOP_FILE_PATH, NULL, NULL);
            break;

        case STOP_DB_CODE:
            printf("Received STOP with -db command\n");
            execute_script(STOP_FILE_PATH, "-db", NULL);
            break;

        case STATUS_CODE: {
            printf("Received STATUS command\n");
            execute_script(STATUS_FILE_PATH, NULL, NULL);
            break;
        }

        case RESET_CODE: {
            size_t scale = 0, base = 0;
            if (read(conn_fd, &scale, sizeof scale) > 0) {
                char scale_str[16];
                sprintf(scale_str, "%ld", scale);
                if(read(conn_fd, &base, sizeof base) > 0){
                    printf("Received RESET command (scale=%ld, base=%ld)\n", scale, base);
                    char base_str[16];
                    sprintf(base_str, "%ld", base);
                    execute_script(RESET_CODE, scale_str, base_str);
                } else {
                    printf("Received RESET command (scale=%ld\n", scale);
                    execute_script(RESET_CODE, scale_str, NULL);
                }
                
            } else {
                execute_script(RESET_CODE, NULL, NULL);
            }
            break;
        }

        case INC_CODE: {
            size_t delta = 0;
            if (read(conn_fd, &delta, sizeof delta) > 0) {
                char delta_str[16];
                sprintf(delta_str, "%ld", delta);
                printf("Received INC command (delta=%ld)\n", delta);
                execute_script(INC_FILE_PATH, delta_str, NULL);
            } else {
                printf("Received INC command (no delta)\n");
                execute_script(INC_FILE_PATH, NULL, NULL);
            }
            break;
        }

        case DEC_CODE: {
            size_t delta = 0;
            if (read(conn_fd, &delta, 1) > 0) {
                char delta_str[16];
                sprintf(delta_str, "%ld", delta);
                printf("Received DEC command (delta=%ld)\n", delta);
                execute_script(DEC_FILE_PATH, delta_str, NULL);
            } else {
                printf("Received DEC command (no delta)\n");
                execute_script(DEC_FILE_PATH, NULL, NULL);
            }
            break;
        }
        
        default:
            printf("Unknown command: %d\n", command);
            break;
    }
	close(conn_fd);
	pthread_exit(NULL);
}

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[]) {	
	puts(":: START ::");

	int nfds = sd_listen_fds(0);
	if (nfds != 1) {
		fprintf(stderr, "Zero or more than one fds! Panic.\n");
		exit(1);
	}

	int main_fd = SD_LISTEN_FDS_START;

	puts(":: LISTENING ::");

	for (;;) {

		struct sockaddr_un cli_addr;
		unsigned int cli_addr_len = sizeof cli_addr;

		int conn_fd = accept(main_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
		if (conn_fd < 0) {
			close(main_fd);
			error("ERROR on accept");
		}

		printf("server established connection with client\n");

		pthread_t thread;
		pthread_create(&thread, NULL, process_connection, (void *)(intptr_t)conn_fd);
		pthread_detach(thread);
	}

	close(main_fd);

	puts(":: END ::");
	return 0;
}
