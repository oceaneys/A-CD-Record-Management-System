#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *menu[] = {
	"a - add new record",
	"b - delete record",
	"q - quit",
	NULL,
};

int getchoice(char *greet, char *menu[], FILE *in, FILE *out);

int main()
{
	int choice = 0;
	FILE *in, *out;

	

	if(!isatty(fileno(stdout))){
		fprintf(stderr,"You are not a terminal, OK.\n");
	}

	in = fopen("/dev/tty","r");
	out = fopen("/dev/tty","w");

	if(!in || !out){
		fprintf(stderr,"Unable to open /dev/tty\n");
		exit(1);
	}

	do{
		choice = getchoice("Please select an action",menu, in, out);
		printf("You have chosen %c\n",choice);
	}while(choice != 'q');

	exit(0);
}


int getchoice(char *greet, char *choices[], FILE *in, FILE *out)
{
	int chosen = 0;
	int selected;
	char **option;

	do{
		fprintf(out,"Choice: %s\n",greet);
		option = choices;
		while(*option){
			fprintf(out,"%s\n",*option);
			option++;
		}
		
		do{
			selected = fgetc(in);
		}while(selected == '\n');

		option = choices;

		while(*option){
			if(selected == *option[0]){
				chosen = 1;
				break;
			}
			option++;
		
		}
		if(!chosen){
			fprintf(out,"Incorrect choice, select again\n");
		}

	}while(!chosen);

	return selected;
}
