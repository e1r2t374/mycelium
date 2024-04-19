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
	char *commands[] = {
		"uname -a 2>/dev/null && cat /etc/*-release && cat /proc/version 2>/dev/null; sleep 1", /*0*/
		"echo ID=$(id) && for i in $(cut -d':' -f1 /etc/passwd);do id $i;done 2>/dev/null; sleep 1", /*1*/
		"env 2>/dev/null | grep -v 'LS_COLORS' 2>/dev/null", /*2*/
		"echo $(whoami) 2>/dev/null; sleep 1", /*3*/
		"cat /etc/shells 2>/dev/null", /*4*/
		"cat /etc/passwd 2>/dev/null", /*5*/
		"cat /etc/master.passwd 2>/dev/null", /*6*/
		"cat /etc/group 2>/dev/null", /*7*/
		"cat /etc/shadow 2>/dev/null", /*8*/
		"cat /etc/gshadow 2>/dev/null", /*9*/
		"cat /etc/sudoers 2>/dev/null", /*10*/
		"cat /etc/sudoers.d/* 2>/dev/null", /*11*/
		"lastlog 2>/dev/null |grep -v \"Never\" 2>/dev/null", /*12*/
		"w 2>/dev/null", /*13*/
		"grep -v -E \"^#\" /etc/passwd 2>/dev/null| awk -F: '$3 == 0 { print $1}' 2>/dev/null", /*14*/
		"grep -v -e '^$' /etc/sudoers 2>/dev/null |grep -v \"#\" 2>/dev/null", /*15*/
		"echo '' | sudo -S -l -k 2>/dev/null", /*16*/
		"sudo -S -l -k 2>/dev/null", /*17*/
	};
	char *headers[] = {
		"Operating System/Kernel Info",/*0*/
		"IDs and Groups",/*1*/
		"Env",/*2*/
		"Current User",/*3*/
		"Shells",/*4*/
		"Passwd Contents",/*5*/
		"Master.passwd Contents",/*6*/
		"Group Contents",/*7*/
		"Shadow Contents",/*8*/
		"Gshadow Contents",/*9*/
		"Sudoers Contents"/*10*/
		"Sudoers.d Contents",/*11*/
		"Last Login",/*12*/
		"Currently Logged In",/*13*/
		"Admin Users",/*14*/
		"Vital Sudoers Info",/*15*/
		"Sudo Pass?",/*16*/
		"Sudo Permissions",/*17*/
	};
	/*ADD length check to ensure headers and commands arrays are same size*/
	pthread_t threads[sizeof(commands) / sizeof(commands[0])];
	int i;
	for (i = 0; i < (int)(sizeof(commands) / sizeof(commands[0])); i++) {
		char **cmds = malloc(2 * sizeof(char *));
		cmds[0] = commands[i];
		cmds[1] = headers[i];
		pthread_create(&threads[i], NULL, exec_cmd, cmds);
	}
	for (i = 0; i < (int)(sizeof(commands) / sizeof(commands[0])); i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
