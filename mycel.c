#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#define RED "\033[38;2;230;0;0m"
#define YELLOW "\033[38;2;200;200;0m"
#define GREEN "\033[38;2;0;150;0m"
#define NORMAL "\033[0m"
void help(){
	printf("%s\033[1m[Usage]:\033[0m\n\n",YELLOW);
	printf("%s(Syntax)\n./%s\b\b -e<MODE> -s<STEALTH>\n\n",YELLOW,__FILE__);
	printf("%sExample: ./%s\b\b -e 1 -s 1\n\n",YELLOW,__FILE__);
	printf("%s\033[1m(Mode Options):\n%s",YELLOW,NORMAL);
	printf("%sMode 1: Everything.\n",YELLOW);
	printf("%sMode 2: Packages.\n",YELLOW);
	printf("%sMode 3: Privilege Escalation.\n",YELLOW);
	printf("%sMode 4: Sensitive Information(Credentials).\n",YELLOW);
	printf("%sMode 5: Users.\n",YELLOW);
	printf("\n%s\033[1m(Stealth Options):\n%s",YELLOW,NORMAL);
	printf("%sStealth 1: No Stealth.\n",YELLOW);
	printf("%sStealth 2: Semi-Stealthy.\n",YELLOW);
	printf("%sStealth 3: Stealthy.\n",YELLOW);
	printf("%sStealth 4: Very Stealthy.\n",YELLOW);
	printf("%s\n",NORMAL);
	abort();
}
int main(int argc, char **argv) {
  	return 0;
}
