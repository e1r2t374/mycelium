#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED "\033[38;2;230;0;0m"
#define YELLOW "\033[38;2;200;200;0m"
#define GREEN "\033[38;2;0;150;0m"
#define NORMAL "\033[0m"
void help(){
	printf("%s\033[1m[Usage]:\033[0m\n\n",YELLOW);
	printf("%s(Enumeration)\n%s\b\b -e <MODE>\n\n",YELLOW,__FILE__);
	printf("%sExample: %s\b\b -e 1\n\n",YELLOW,__FILE__);
	printf("%sMode 1: Everything.\n",YELLOW);
	printf("%sMode 2: Packages\n",YELLOW);
	printf("%sMode 3: Privilege Escalation.\n",YELLOW);
	printf("%sMode 4: Usernames, Emails, Credentials.\n",YELLOW);
	printf("%sMode 5: Users.\n",YELLOW);
	printf("%s\n",NORMAL);
}
int main(int argc, char *argv[]) {
	if(argc < 2){help();}
	if(system("echo hello 2>/dev/null")){printf("failed");};
  	return 0;
}