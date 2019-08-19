#include <unistd.h>
#include <string.h>

/*
void servoControl(char * angle) {
        char buf[1024];

	memset(buf, 0, 1024);
	// make buf to "echo 1=50% > /dev/servoblaster"
	strcat(buf, "echo ");
	strcat(buf, angle);
	strcat(buf, "% > /dev/servoblaster");

	// on servoblaster
	system("sudo ./servod");
	system(buf);

	// wait servo turn exactly
	sleep(1);

	// off servoblaster (save power)
	system("sudo killall servod");
}
*/

void setAngle(char * angles) {
        // angles example ("1=50 2=30")
	char angle_PAN[10];
	char angle_TILT[10];
	char buf[672];

	memset(angle_PAN, 0x00, 10);
	memset(angle_TILT, 0x00, 10);

        // ptr -> "1=50"
        char *ptr = strtok(angles, " ");
        printf("%s\n", ptr);
	strcat(angle_PAN, ptr);

        ///ptr -> "2=30"
        ptr = strtok(NULL, " ");
        printf("%s\n", ptr);
	strcat(angle_TILT, ptr);


	// on servoblaster
	system("sudo ./servod");

	// set angle_PAN
	memset(buf, 0x00, 672);
	strcat(buf, "echo ");
        strcat(buf, angle_PAN);
        strcat(buf, "% > /dev/servoblaster");
	system(buf);
	printf("angle_PAN %s\n", buf);

	// set angle_TILT
	memset(buf, 0x00, 672);
        strcat(buf, "echo ");
        strcat(buf, angle_TILT);
        strcat(buf, "% > /dev/servoblaster");
	system(buf);
	printf("angle_TILT %s\n", buf);

	// wait servo turn exactly
	sleep(1);

	// off servoblaster (save power)
        system("sudo killall servod");

}
