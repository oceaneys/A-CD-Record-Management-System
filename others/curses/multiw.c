#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main()
{
	WINDOW *new_window_ptr;
	WINDOW *popup_window_ptr;
	int x_loop,y_loop;
	char a_letter = 'a';

	initscr();
	
	move(5,5);
	printw("%s", "Testing multiple windows");
	wrefresh(stdscr);

	for(y_loop = 0; y_loop < LINES - 1; y_loop++){
		for(x_loop = 0; x_loop < COLS - 1; x_loop++){
			mvwaddch(stdscr, y_loop, x_loop, a_letter);
			a_letter++;
			if(a_letter > 'z'){
				a_letter = 'a';
			}
		}
	}

	wrefresh(stdscr);
	sleep(2);

	new_window_ptr = newwin(10,20,5,5);
	box(new_window_ptr, '|','-');
	mvwprintw(new_window_ptr, 2, 2, "%s", "Hello World");
	mvwprintw(new_window_ptr, 5, 2, "%s","numbers begin");
	wrefresh(new_window_ptr);
	sleep(2);

	a_letter = '0';
	for(y_loop = 0; y_loop < LINES - 1; y_loop++){
		for(x_loop = 0; x_loop < COLS - 1; x_loop++){
			mvwaddch(stdscr, y_loop, x_loop, a_letter);
			a_letter++;
			if(a_letter > '9'){
				a_letter = '0';
			}
		}
	}

	wrefresh(stdscr);
	sleep(2);
	
	wrefresh(new_window_ptr);
	sleep(2);

//	touchwin(new_window_ptr);
	wrefresh(new_window_ptr);
	sleep(2);

	popup_window_ptr = newwin(10,20,8,8);
	box(popup_window_ptr, '|', '-');
	mvwprintw(popup_window_ptr, 5, 2, "%s", "Pop Up Window!");
	wrefresh(popup_window_ptr);
	sleep(2);

	touchwin(new_window_ptr);
	wrefresh(new_window_ptr);
	sleep(2);

	touchwin(popup_window_ptr);
	wrefresh(popup_window_ptr);
	sleep(2);

	
	wclear(popup_window_ptr);
	wclear(new_window_ptr);
	wrefresh(popup_window_ptr);
	wrefresh(new_window_ptr);
	sleep(2);
	
	delwin(new_window_ptr);
	delwin(popup_window_ptr);
	touchwin(stdscr);

	wrefresh(stdscr);
	sleep(2);

	endwin();
	exit(EXIT_SUCCESS); 
	

}
