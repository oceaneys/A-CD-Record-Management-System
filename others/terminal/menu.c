#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <term.h>
#include <curses.h>

static FILE *output_stream = (FILE *)0;

char *menu[] = {
	"a - add new record",
	"b - delete record",
	"q - quit",
	NULL,
};

int getchoice(char *greet, char *menu[], FILE *in, FILE *out);
int char_to_terminal(int char_to_write);

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
		fprintf(out,"You have chosen %c\n",choice);
		usleep(400000); // sleep for the 'fprintf' above to display info
	}while(choice != 'q');
	
	tcsetattr(fileno(out),TCSANOW, &initial_settings);

	exit(0);
}


int getchoice(char *greet, char *choices[], FILE *in, FILE *out)
{
	int chosen = 0;
	int selected;
	int screenrow, screencol = 10;
	char **option;
	char *cursor, *clear;

	output_stream = out;

	setupterm(NULL, fileno(out), (int *)0);
	cursor = tigetstr("cup");
	clear = tigetstr("clear");

	
	do{
		screenrow = 4;
		tputs(clear, 1, char_to_terminal);
		tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
		fprintf(out,"Choice: %s\n",greet);
		screenrow += 2;
		option = choices;
		while(*option){
			tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
			fprintf(out,"%s\n",*option);
			screenrow++;
			option++;
		}

		fprintf(out,"\n");

		fflush(out);
		tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
		selected = fgetc(in);
		option = choices;

		while(*option){
			if(selected == *option[0]){
				chosen = 1;
				break;
			}
			option++;
		
		}
		if(!chosen){
			tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
			fprintf(out,"Incorrect choice, select again\n");
			usleep(400000); // sleep for the 'fprintf' above to display info
		}

	}while(!chosen);

	return selected;
}


int char_to_terminal(int char_to_terminal)
{
	if(output_stream)
		putc(char_to_terminal, output_stream);
	return 0;
}
