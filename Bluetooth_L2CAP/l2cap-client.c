#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "PiCamera.h"
#include "ServoCtl.h"

#define MAXBUF 672

int main(int argc, char **argv)
{
	struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
	char buf[MAXBUF] = { 0 };
	char file_name[21];
	int s, client, bytes_read, status, sourse_fd;
	int file_name_len, read_len, des_fd, file_read_len;
	float angle;
	int counter = 0;
	int slave = 0;
	socklen_t opt = sizeof(rem_addr);

	// bind socket to port 0x1001 of the first available
	// bluetooth adapter
	loc_addr.l2_family = AF_BLUETOOTH;
	loc_addr.l2_bdaddr  = *BDADDR_ANY;
	loc_addr.l2_psm = htobs(0x1001);
	s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

	// put socket into listening mode
	// listen(s, 1);

	while (1) {
		printf("Listening....\n");
		listen(s, 1);
		// accept one connection
		client = accept(s, (struct sockaddr *)&rem_addr, &opt);
		ba2str(&rem_addr.l2_bdaddr, buf);
		fprintf(stderr, "accepted connection from %s\n", buf);

		memset(buf, 0, MAXBUF);
		memset(file_name, 0x00, 21);

		// read Angle
		printf("Reading....\n");
		read_len = read(client, buf, MAXBUF);

		// set Angle
		setAngle(buf);

		// take Picture
		takePic(file_name);

		memset(buf, 0x00, MAXBUF);
		printf("File Name : %s\n", file_name);
		file_name_len = strlen(file_name);

		status = send(client, file_name, file_name_len, 0);

		sourse_fd = open(file_name, O_RDONLY);
		if (!sourse_fd) {
			perror("Error : ");
			return 1;
		}

		while (1) {
			memset(buf, 0x00, MAXBUF);
			read_len = read(sourse_fd, buf, MAXBUF);
			status = send(client, buf, MAXBUF, 0);
			if (status < 0) perror("uh oh");
			if (read_len == 0)	break;
		}
		memset(buf, 0x00, 0);
		status = write(s, buf, 0);

		close(client);
	}
	close(client);
	close(s);
}
