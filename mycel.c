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
Windows and mac detection
Options such as steath, no color, no multithreading, etc
Python and bash equivalents
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
		switch (atoi(cmds[1])) {
			case 0:printf("%s\n[Operating System/Kernal Info]:\033[0m\n",GREEN);break;
			case 1:printf("%s\n[IDs and Groups]:\033[0m\n",GREEN);break;
			case 2:printf("%s\n[Env]:\033[0m\n",GREEN);break;
			case 3:printf("%s\n[Current User]:\033[0m\n",GREEN);break;
			case 4:printf("%s\n[Shells]:\033[0m\n",GREEN);break;
			case 5:printf("%s\n[Passwd Contents]:\033[0m\n",GREEN);break;
			case 6:printf("%s\n[Master.passwd Contents]:\033[0m\n",GREEN);break;
			case 7:printf("%s\n[Group Contents]:\033[0m\n",GREEN);break;
			case 8:printf("%s\n[Shadow Contents]:\033[0m\n",GREEN);break;
			case 9:printf("%s\n[Gshadow Contents]:\033[0m\n",GREEN);break;
			case 10:printf("%s\n[Sudoers Contents]:\033[0m\n",GREEN);break;
			case 11:printf("%s\n[Sudoers.d Contents]:\033[0m\n",GREEN);break;
			case 12:printf("%s\n[Last Login]:\033[0m\n",GREEN);break;
			case 13:printf("%s\n[Currently Logged In]:\033[0m\n",GREEN);break;
			case 14:printf("%s\n[Admin Users]:\033[0m\n",GREEN);break;
			case 15:printf("%s\n[Vital Sudoers Info]:\033[0m\n",GREEN);break;
			case 16:printf("%s\n[Sudo Pass?]:\033[0m\n", GREEN);break;
			case 17:printf("%s\n[Sudo Permissions]:\033[0m\n",GREEN);break;
			//case 18:printf("%s\n[Used Sudo]:\033[0m\n",GREEN);break;
			default:printf("%s\n[No Title Available]:\033[0m\n",GREEN);break;
		}
		printf("%s\n%s\n",output, NORMAL);
	} 
	else {
		switch (atoi(cmds[1])) {
			case 0:printf("%s\n[Operating System Info]:\033[0m\n",RED);break;
			case 1:printf("%s\n[IDs and Groups]:\033[0m\n",RED);break;
			case 2:printf("%s\n[Env]:\033[0m\n",RED);break;
			case 3:printf("%s\n[Current User]:\033[0m\n",RED);break;
			case 4:printf("%s\n[Shells]:\033[0m\n",RED);break;
			case 5:printf("%s\n[Passwd Contents]:\033[0m\n",RED);break;
			case 6:printf("%s\n[Master.passwd Contents]:\033[0m\n",RED);break;
			case 7:printf("%s\n[Group Contents]:\033[0m\n",RED);break;
			case 8:printf("%s\n[Shadow Contents]:\033[0m\n",RED);break;
			case 9:printf("%s\n[Gshadow Contents]:\033[0m\n",RED);break;
			case 10:printf("%s\n[Sudoers Contents]:\033[0m\n",RED);break;
			case 11:printf("%s\n[Sudoers.d Contents]:\033[0m\n",RED);break;
			case 12:printf("%s\n[Last Login]:\033[0m\n",RED);break;
			case 13:printf("%s\n[Currently Logged In]:\033[0m\n",RED);break;
			case 14:printf("%s\n[Admin Users]:\033[0m\n",RED);break;
			case 15:printf("%s\n[Vital Sudoers Info]:\033[0m\n",RED);break;
			case 16:printf("%s\n[Sudo Pass?]:\033[0m\n",RED);break;
			case 17:printf("%s\n[Sudo Permissions]:\033[0m\n",RED);break;
			//case 18:printf("%s\n[Used Sudo]:\033[0m\n",RED);break;
			default:printf("%s\n[No Title Available]:\033[0m\n",RED);break;
		}
	}
	return 0;
}
int main(void) {
	char *commands[] = {
	"uname -a 2>/dev/null && cat /etc/*-release && cat /proc/version 2>/dev/null; sleep 1", //OS and Kernel 0
	"echo ID=$(id) && for i in $(cut -d':' -f1 /etc/passwd);do id $i;done 2>/dev/null; sleep 1", //IDs and Groups 1
	"env 2>/dev/null | grep -v 'LS_COLORS' 2>/dev/null", //Env info 2
	"echo $(whoami) 2>/dev/null; sleep 1", //Current User 3
	"cat /etc/shells 2>/dev/null", //Shells 4
	"cat /etc/passwd 2>/dev/null", //Passwd contents 5
	"cat /etc/master.passwd 2>/dev/null", //Master.passwd contents 6
	"cat /etc/group 2>/dev/null", //Group contents 7
	"cat /etc/shadow 2>/dev/null", //Shadow contents 8
	"cat /etc/gshadow 2>/dev/null", //Gshadow contents 9
	"cat /etc/sudoers 2>/dev/null", //Sudoers contents 10
	"cat /etc/sudoers.d/* 2>/dev/null", //Sudoers.d contents 11
	"lastlog 2>/dev/null |grep -v \"Never\" 2>/dev/null", //Last user to login 12
	"w 2>/dev/null", //Who is logged in 13
	"grep -v -E \"^#\" /etc/passwd 2>/dev/null| awk -F: '$3 == 0 { print $1}' 2>/dev/null", //Admin users 14
	"grep -v -e '^$' /etc/sudoers 2>/dev/null |grep -v \"#\" 2>/dev/null", //Vital sudoers info 15
	"echo '' | sudo -S -l -k 2>/dev/null", //sudo without password? 16
	"sudo -S -l -k 2>/dev/null", //Sudo perms 17
	//"find /home -name .sudo_as_admin_successful 2>/dev/null" //Who has used sudo before (fix) 18
	};
	/*Multithreading*/
	pthread_t threads[sizeof(commands) / sizeof(commands[0])];
	int i;
	for (i = 0; i < (int)(sizeof(commands) / sizeof(commands[0])); i++) {
		char **cmds = malloc((int)(sizeof(commands)/sizeof(commands[0])) * sizeof(char *));
		cmds[0] = commands[i];
		cmds[1] = malloc((int)(sizeof(commands)/sizeof(commands[0])) * sizeof(char));
		sprintf(cmds[1], "%d", i);
		pthread_create(&threads[i], NULL, exec_cmd, cmds);
	}
	for (i = 0; i < (int)(sizeof(commands)/sizeof(commands[0])); i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
