#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>

#define RED "\033[38;2;230;0;0m"
#define YELLOW "\033[38;2;200;200;0m"
#define GREEN "\033[38;2;0;150;0m"
#define NORMAL "\033[0m"
#define BOLD "\033[1m"
/*
TODO
-Use array of structss to keep commands and the headers togeather, because it's quite easy to mess up and have two arrays with different sizes.
- Allocate memory dynamically for commands' output, and add coordination between threads to keep their outputs separate when printing.
-Print command with header
-Options such as steath, no color, no multithreading, etc
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

void *exec_cmd(void *cmd) {
	char **cmds = (char **)cmd;
	char output[4096] = {0};
	if (sys_cmd(cmds[0], output) == 0) {
		printf("%s%s:\033[0m\n", GREEN, cmds[1]);
		printf("%s\n%s\n", output, NORMAL);
	} 
	else {
		printf("%s%s:\033[0m\n", RED, cmds[1]);
		printf("%s\n%s\n", output, NORMAL);
	}
	return 0;
}
int main(void){
	char *commands[] = { /*Add your commands to the array (Don't forget to add a matching header text)*/
		"uname -a ||: && (cat /etc/*-release ||: && (cat /proc/version; sleep 1 ||:)) 2>/dev/null",
		"echo ID=$(id) && for i in $(cut -d':' -f1 /etc/passwd);do id $i;done; 2>/dev/null sleep 1  2>/dev/null",
		"env 2>/dev/null | grep -v 'LS_COLORS' 2>/dev/null",
		"echo $(whoami) 2>/dev/null; sleep 1",
		"cat /etc/shells 2>/dev/null",
		"cat /etc/passwd 2>/dev/null",
		"cat /etc/master.passwd 2>/dev/null",
		"cat /etc/group 2>/dev/null",
		"cat /etc/shadow 2>/dev/null",
		"cat /etc/gshadow 2>/dev/null",
		"cat /etc/sudoers 2>/dev/null",
		"cat /etc/profile 2>/dev/null",
		"cat /etc/bashrc 2>/dev/null",
		"cat ~/.bash_profile 2>/dev/null",
		"cat ~/.bashrc 2>/dev/null",
		"cat ~/.bash_logout 2>/dev/null",
		"cat /etc/services 2>/dev/null",
		"ls -alh /usr/bin/ 2>/dev/null && ls -alh /sbin/ 2>/dev/null",
		"dpkg --get-selections||:&&(rpm -qa||:&&(pacman -Q||:&&(ls -alh /var/cache/yum/||:&&(ls -alh /var/cache/apt/archivesO||:)))) 2>/dev/null",
	};
	char *headers[] = { /*The titles of the commands*/
		"Operating System/Kernel Info",
		"IDs and Groups",
		"Env",
		"Current User",
		"Shells",
		"Passwd Contents",
		"Master.passwd Contents",
		"Group Contents",
		"Shadow Contents",
		"Gshadow Contents",
		"Sudoers Contents",
		"etc/profile Contents",
		"etc/bashrc Contents",
		".bash_profile Contents",
		".bashrc Contents",
		".bash_logout Contents",
		"etc/services Contents",
		"/usr/bin and /sbin Contents",
		"Installed Packages",
	};
	if(sizeof(commands) != sizeof(headers)){
		error("Command and header arrays do not have the same number of elements.");
	}
	pthread_t threads[sizeof(commands)/sizeof(commands[0])];
	size_t i;
	for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
		char **cmds = malloc(2 * sizeof(char *));
		cmds[0] = commands[i];
		cmds[1] = headers[i];
		pthread_create(&threads[i], NULL, exec_cmd, cmds);
	}
	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
