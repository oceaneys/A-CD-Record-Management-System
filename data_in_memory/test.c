#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	
	init_record_list();
	display_all_records();

    return 0;
}
