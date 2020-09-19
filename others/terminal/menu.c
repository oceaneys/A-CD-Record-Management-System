#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


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
	struct termios initial_settings, new_settings;

	if(!isatty(fileno(stdout))){
		fprintf(stderr,"You are not a terminal, OK.\n");
	}

	in = fopen("/dev/tty","r");
	out = fopen("/dev/tty","w");

	if(!in || !out){
		fprintf(stderr,"Unable to open /dev/tty\n");
		exit(1);
	}

	tcgetattr(fileno(out),&initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_lflag &= ~ISIG;
	if(tcsetattr(fileno(out),TCSANOW, &new_settings) != 0){
		fprintf(stderr, "Could not set attributes\n");
	}

	do{
		choice = getchoice("Please select an action",menu, in, out);
		printf("You have chosen %c\n",choice);
	}while(choice != 'q');
	
	tcsetattr(fileno(out),TCSANOW, &initial_settings);

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
