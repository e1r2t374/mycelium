#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#define RED "\033[38;2;230;0;0m"
#define YELLOW "\033[38;2;200;200;0m"
#define GREEN "\033[38;2;0;150;0m"
#define NORMAL "\033[0m"
#define BOLD "\033[1m"
void help(){
	printf("%s%s[Usage]:%s\n\n",BOLD,YELLOW,NORMAL);
	printf("%s%s(Syntax):%s\n./%s\b\b -e <ENUMERATE> -s <STEALTH>\n\n",BOLD,YELLOW,NORMAL,__FILE__);
	printf("%s%s(Example):%s\n ./%s\b\b -e 1 -s 1\n\n",BOLD,YELLOW,NORMAL,__FILE__);
	printf("%s%s(Enumeration Options):\n%s",BOLD,YELLOW,NORMAL);
	printf("Enumeration 1: Everything.\n");
	printf("Enumeration 2: Packages.\n");
	printf("Enumeration 3: Privilege Escalation.\n");
	printf("Enumeration 4: Sensitive Information.\n");
	printf("Enumeration 5: Network.\n");
	printf("\n%s%s(Stealth Options):\n%s",BOLD,YELLOW,NORMAL);
	printf("Stealth 0: (DEFAULT)No Stealth.\n");
	printf("Stealth 1: Stealth.\n");
	printf("%s\n",NORMAL);
}
void error(char* errmsg){
	fprintf(stderr,"\033[1m%sERROR:%s\033[0m\n",RED,errmsg);
	help();
	abort();
}
int main(int argc, char **argv) {
	if(argc < 2){error("Not Enough Flags Provided!");}
	else{
		int opt; int e = 0; int s = 0;
		while ((opt = getopt(argc, argv, "e:s::")) != -1){
			switch (opt){
				case 'e':
					if(atoi(optarg) > 0 && atoi(optarg) <= 4){
						e = atoi(optarg);
					}
					else{error("Invalid Enumeration Option!");}
					break;
				case 's': 
					if(atoi(optarg) == 0 || atoi(optarg) == 1){
						s = atoi(optarg);
					}
					else{error("Invalid Stealth Option!");}
					break;
				case '?': error("Invalid Flag!");break;
				default:error("You broke something!");break;
			}
		}
		if(e == 0){error("Invalid Flag!");}
		else{
			switch(e){
				case 1:
					switch(s){
						case 0:break;
						case 1:break;
						case'?':break;
						default:break;
					}
					break;
				
				case 2:
					switch(s){
						case 0:
							if(system("dpkg --get-selections | wc -l 1>/dev/null 2>/dev/null") == 0){ 
								printf("%s%s[Apt installed]%s\n",BOLD,GREEN,NORMAL);
								if(system("echo \"Packages:\"&& dpkg --get-selections | wc -l 2>/dev/null") != 0){
									error("dpkg package error");
								}
								if(system("echo \"Versions:\"&& dpkg -p 2>/dev/null") != 0){
									error("dpkg version error");
								}
							}
							break;
						case 1:break;
						case'?':break;
						default:break;
					}
					break;
				case 3:
					switch(s){
						case 0:break;
						case 1:break;
						case'?':break;
						default:break;
					}
					break;
				case 4:
					switch(s){
						case 0:break;
						case 1:break;
						case'?':break;
						default:break;
					}
					break;
				case'?':break;
				default:break;
			}
			
			printf("\ne:%d,s:%d\n",e,s);
		}
	}
  	return 0;
}