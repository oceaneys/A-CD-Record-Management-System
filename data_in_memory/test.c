#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "data.h"

int main(int argc, char **argv)
{
	char *rtitle1 = "rtitle1";
	char *rartist1 = "rartist1";
	Record *record1 = NULL;
	record1 = (struct Record *)malloc(sizeof(struct Record));
	add_record_wrap(rtitle1,rartist1,record1);

	display_all_records();

    return 0;
}
