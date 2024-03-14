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
	printf("%s\033[1m(Syntax):%s\n./%s\b\b -e <ENUMERATE> -s <STEALTH>\n\n",YELLOW,NORMAL,__FILE__);
	printf("%s\033[1m(Example):%s\n ./%s\b\b -e 1 -s 1\n\n",YELLOW,NORMAL,__FILE__);
	printf("%s\033[1m(Enumeration Options):\n%s",YELLOW,NORMAL);
	printf("Enumeration 1: Everything.\n");
	printf("Enumeration 2: Packages.\n");
	printf("Enumeration 3: Privilege Escalation.\n");
	printf("Enumeration 4: Sensitive Information.\n");
	printf("\n%s\033[1m(Stealth Options):\n%s",YELLOW,NORMAL);
	printf("Stealth 1: No Stealth.\n");
	printf("Stealth 2: Semi-Stealthy.\n");
	printf("Stealth 3: Stealthy.\n");
	printf("Stealth 4: Very Stealthy.\n");
	printf("%s\n",NORMAL);
}
void error(char* errmsg){
	fprintf(stderr,"\033[1m%sERROR:%s\033[0m\n",RED,errmsg);
	help();
	abort();
}
int main(int argc, char **argv) {
	if(argc < 3){error("Not Enough Flags Provided!");}
	else{
		int opt; int e = 0; int s = 0;
		while ((opt = getopt(argc, argv, "e:s:")) != -1){
			switch (opt){
				case 'e':
					if(atoi(optarg) > 0 && atoi(optarg) <= 4){
						e = atoi(optarg);
					}
					else{error("Invalid Enumeration Option!");}
					break;
				case 's': 
					if(atoi(optarg) > 0 && atoi(optarg) <= 4){
						s = atoi(optarg);
					}
					else{error("Invalid Stealth Option!");}
					break;
				case '?': error("Invalid Flag!");break;
				default:error("You broke something!");break;
			}
		}
		if(e == 0 || s == 0){error("Invalid Flags!");}
		else{
			switch(e){
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				case'?':break;
				default:break;
			}
			switch(s){
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				case'?':break;
				default:break;
			}
			printf("e:%d,s:%d",e,s);
		}
	}
  	return 0;
}
