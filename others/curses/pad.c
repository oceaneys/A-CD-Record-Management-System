#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main()
{
	WINDOW *pad_ptr;
	int x, y;
	int pad_lines, pad_cols;
	char disp_char;

	initscr();
	pad_lines = LINES + 50;
	pad_cols = COLS + 50;
	pad_ptr = newpad(pad_lines, pad_cols);
	disp_char = 'a';

	for(x = 0; x < pad_lines; x++){
		for(y = 0; y < pad_cols; y++){
			mvwaddch(pad_ptr, x, y, disp_char);
			if(disp_char == 'z'){

				disp_char = 'a';

			}else{

				disp_char++;

			}
		}
	}

	prefresh(pad_ptr, 5, 7, 2, 2, 9, 9);
	sleep(2);

	prefresh(pad_ptr, LINES+45, COLS+45, 5, 5, 40, 50);
	sleep(2);

	delwin(pad_ptr);
	endwin();
	exit(EXIT_SUCCESS);
}
