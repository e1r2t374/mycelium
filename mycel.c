#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#define RED "\033[38;2;230;0;0m"
#define YELLOW "\033[38;2;200;200;0m"
#define GREEN "\033[38;2;0;150;0m"
#define NORMAL "\033[0m"
#define BOLD "\033[1m"
/*
TODO
add windows and mac detection
add options such as steath and option without color
*/
void error(const char *err_msg) {
	fprintf(stderr, "\033[1m%sERROR:%s\033[0m\n", RED, err_msg);
	exit(EXIT_FAILURE);
}
/*Executes system commands like System() but returns output */
int sys_cmd(const char *command, char *output) {
	int pipefd[2];
	pid_t pid;
	int status;
	if (pipe(pipefd) == -1) {
		error("Error creating pipe.");
	}
	pid = fork();
	if (pid < 0) {
		error("Fork failed.");
	}
	if (pid == 0) {
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
			error("Failed to duplicate file descriptor.");
		}
		close(pipefd[1]);
		if (execl("/bin/sh", "sh", "-c", command, NULL) == -1) {
			error("Error executing command.");
		}
	} 
	else {
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
	    		ssize_t n = read(pipefd[0], output, 4096);
	    		if (n > 0) {
				output[n] = '\0';
			}
	    		close(pipefd[0]);
	    		return 0;
		} 
		else {
	    		close(pipefd[0]);
	    		return 1;
		}
    	}
 	return -1;
}
/*Prints header then runs command (Probably soon to be combined with sys_exec*/
void *exec_cmd(void *cmd) {
	char **cmds = (char **)cmd;
    	char output[4096] = {0};
    	if (sys_cmd(cmds[0], output) == 0) {
		switch (cmds[1][0]) {
	    		case '0':printf("%s\n[Operating System/Kernal Info]:\033[0m\n", GREEN);break;
	    		case '1':printf("%s\n[IDs and Groups]:\033[0m\n", GREEN);break;
	    		default:printf("%s\n[No Title Available]:\033[0m\n", GREEN);break;
	}
	printf("%s\n%s\n",output, NORMAL);
   	} 
	else {
		switch (cmds[1][0]) {
	    		case '0':printf("%s\n[Operating System Info]:\033[0m\n", RED);break;
	    		case '1':printf("%s\n[IDs and Groups]:\033[0m\n", RED);break;
	    		default:printf("%s\n[No Title Available]:\033[0m\n", RED);break;
		}
    	}
	return 0;
}
int main(void) {
	char *commands[] = {
	"cat /etc/*-release && cat /proc/version 2>/dev/null; sleep 1", //OS and Kernel
	"echo ID=$(id) && for i in $(cut -d':' -f1 /etc/passwd);do id $i;done 2>/dev/null; sleep 1", //IDs and Groups
    	};
	/*Multithreading*/
	pthread_t threads[sizeof(commands) / sizeof(commands[0])];
	size_t i;
	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		char **cmds = malloc(2 * sizeof(char *));
		cmds[0] = commands[i];
		cmds[1] = malloc(2 * sizeof(char));
		sprintf(cmds[1], "%ld", i);
		pthread_create(&threads[i], NULL, exec_cmd, cmds);
	}
	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
