#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
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
void error(const char *err_msg){
	fprintf(stderr,"\033[1m%sERROR:%s\033[0m\n",RED,err_msg);
	exit(EXIT_FAILURE);
}
int sys_cmd(const char *command, char *output) {
	int pipefd[2];
    	pid_t pid;
	int status;
	if (pipe(pipefd) == -1) {error("Error creating pipe.");}
	pid = fork();
	if (pid < 0) {error("Fork failed.");}
	if (pid == 0) {
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1) {error("Failed to duplicate file descriptor.");}
		close(pipefd[1]);
		if (execl("/bin/sh", "sh", "-c", command, NULL) == -1) {error("Error executing command.");}
    	} 
	else {
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			ssize_t n = read(pipefd[0], output, 4096);
			if (n > 0) {output[n] = '\0';}
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

int main(void) {
	char *commands[] = {
	"cat /etc/*-release && cat /proc/version 2>/dev/null", //OS and Kernel
	"echo ID=$(id) && for i in $(cut -d':' -f1 /etc/passwd);do id $i;done 2>/dev/null", //IDs and Groups
	"find / -type f -perm -o+w ! -path '/proc/*'", // World-writable files
	"find / -type d -ls 2>/dev/null | grep -E '\\s777\\s|\\s775\\s|\\s711\\s'", // Directories with excessive permissions
	"echo 'test' 2>/dev/null" //Functionality test
	};
	int size = sizeof(commands) / sizeof(commands[0]);
	for (int i = 0; i < size; i++) {
		char output[4096] = {0};
		if (sys_cmd(commands[i], output) == 0) {
			switch (i) {
				case 0:
					printf("%s\n[Operating System/Kernal Info]:\033[0m\n",GREEN);
					printf("%s\n%s\n",output,NORMAL);
					break;
				case 1:
					printf("%s\n[IDs and Groups]:\033[0m\n",GREEN);
					printf("%s\n%s\n",output,NORMAL);
					break;
				case 2:
					printf("%s\n[World Writable Files]:\033[0m\n",GREEN);
					printf("%s\n%s\n",output,NORMAL);
					break;
				case 3:
					printf("%s\n[Priviledged Directories]:\033[0m\n",GREEN);
					printf("%s\n%s\n",output,NORMAL);
					break;
				default:printf("%s\n[No Title Available]:\033[0m\n",GREEN);break;
			}
		} 
		else {
			switch (i) {
				case 0:printf("%s\n[Operating System Info]:\033[0m\n",RED);break;
				case 1:printf("%s\n[IDs and Groups]:\033[0m\n",RED);break;
				case 2:printf("%s\n[World Writable Files]:\033[0m\n",RED);break;
				case 3:printf("%s\n[Priviledged Directories]:\033[0m\n",RED);break;
				default:printf("%s\n[No Title Available]:\033[0m\n",RED);break;
			}
		}
	}
	return 0;
}
