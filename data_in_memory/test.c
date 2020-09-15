#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "data.h"

int main(int argc, char **argv)
{

	char rtitle[20],rartist[20];
	char ttitle[20],tstyle[20];
	char arg = argv[1][0];
	int ret = 0;
	Record *record = (struct Record *)malloc(sizeof(struct Record));
	Track *track = (struct Track *)malloc(sizeof(struct Track));

	switch(arg){
		case 'r':	
					get_record_data(rtitle,rartist);
					if((ret=add_record_wrap(rtitle,rartist,record)) == 0)
						printf("add record, done.\n");
					else
						printf("%s exsits.\n",rtitle);
				  	break;

		case 't': 
					get_record_data(rtitle,rartist);
					add_record_wrap(rtitle,rartist,record);
					printf("add record, done.\n");

					get_track_data(rtitle,ttitle,tstyle);
					if((ret=add_track_wrap(rtitle,ttitle,tstyle,track)) == 0)
						printf("add track, done.\n");
					else
						printf("%s not exsit, go to (1) first.\n",rtitle); 

					display_track_of_record(record);

				  	break;

		default: printf("nothing\n");
				 break;
	}


    return 0;
}
