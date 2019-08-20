#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define GMT (+9)

void getTime(char * time_buf) {
	time_t rawtime;
	struct tm* tm;
	char temp[10];

	time(&rawtime);
	tm = gmtime(&rawtime);

	memset(time_buf, 0x00, 50);
	memset(temp, 0x00, 10);

	sprintf(temp, "%d", tm->tm_year+1900);
	strcat(time_buf, temp);
	memset(temp, 0x00, 10);

	sprintf(temp, "%02d", tm->tm_mon+1);
	strcat(time_buf, temp);
	memset(temp, 0x00, 10);

	sprintf(temp, "%02d", tm->tm_mday);
	strcat(time_buf, temp);
	memset(temp, 0x00, 10);

	// insert _ for 19700101_010101
	strcat(time_buf, "_");

	sprintf(temp, "%02d", (tm->tm_hour + GMT) %24 );
	strcat(time_buf, temp);
	memset(temp, 0x00, 10);

	sprintf(temp, "%02d", tm->tm_min);
	strcat(time_buf, temp);
	memset(temp, 0x00, 10);

	sprintf(temp, "%02d", tm->tm_sec);
	strcat(time_buf, temp);
}

void takePic(char * file_name) {
	char time_buf[50];
	char buf[672];

	memset(buf, 0x00, 672);
	strcat(buf, "raspistill -o ");

	getTime(time_buf);
	strcat(file_name, time_buf);
	strcat(file_name, ".jpg");
	strcat(buf, time_buf);

	strcat(buf, ".jpg -t 1 -w 640 -h 480");
	system(buf);
	//system("raspistill -o Pub.jpg -t 1 -w 640 -h 480");
}

