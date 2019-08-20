#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <fcntl.h>
#include <unistd.h>
#include "PiCamera.h"
#include "ServoCtl.h"

#define BUF_SIZE 672
#define PAN 12
#define TILT 13

int main(int argc, char **argv)
{
	struct sockaddr_l2 addr = { 0 };
	char buf[BUF_SIZE];
	char file_name[19];
	int sourse_fd;
	int file_name_len, read_len;
	int s, status;
	char *ptr;
	char dest[18] = "B8:27:EB:75:79:7A";

	// allocate a socket
	s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

	// set the connection parameters (who to connect to)
	addr.l2_family = AF_BLUETOOTH;
	addr.l2_psm = htobs(0x1001);
	str2ba(dest, &addr.l2_bdaddr);

	// connect to server
	// status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	while(1) {
		memset(buf, 0x00, BUF_SIZE);
		memset(file_name, 0x00, 19);

		// read Angle
		printf("Reading....\n");
		read_len = read(s, buf, BUF_SIZE);
		// if (read_len == 0)	continue;
		// printf("%s\n", buf);

		// send Angle
		setAngle(buf);

		// take Picture
		takePic(file_name);

		memset(buf, 0x00, BUF_SIZE);
		printf("File Name : %s\n", file_name);
		file_name_len = strlen(file_name);

		status = write(s, file_name, file_name_len);

		sourse_fd = open(file_name, O_RDONLY);
		if (!sourse_fd) {
			perror("Error : ");
			return 1;
		}

		while (1) {
			memset(buf, 0x00, BUF_SIZE);
			read_len = read(sourse_fd, buf, BUF_SIZE);
			status = write(s, buf, BUF_SIZE);
			if (status < 0) perror("uh oh");
			if (read_len == 0)	break;
		}
		memset(buf, 0x00, 0);
		status = write(s, buf, 0);
	}

	close(s);
}
