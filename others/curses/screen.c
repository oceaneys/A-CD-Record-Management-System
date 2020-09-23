#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main()
{
	initscr();

	move(5,15);
	printw("%s","Hello world");
	refresh();

	sleep(2);

	endwin();
	exit(EXIT_SUCCESS);
}
