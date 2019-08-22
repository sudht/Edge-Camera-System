#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXBUF 672

int main(int argc, char **argv)
{
	struct sockaddr_l2 addr = { 0 };
	char buf[MAXBUF] = { 0 };
	char file_name[MAXBUF];
	int s, client, bytes_read, status;
	int read_len, des_fd, file_read_len;
	float angle;
	int counter = 0;
	int slave = 0;
	char dest[2][18] = { "B8:27:EB:66:87:00", "B8:27:EB:66:A2:37"};

	printf("Input slave num (0 or 1): ");
	scanf("%d", &slave);
	getchar();
	printf("%d\n", slave);

	// bind socket to port 0x1001 of the first available
	// bluetooth adapter
	addr.l2_family = AF_BLUETOOTH;
	str2ba(dest[slave], &addr.l2_bdaddr);
	addr.l2_psm = htobs(0x1001);

	// accept one connection
	while(1) {
		s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

		// set the connection parameters (who to connect to)
		status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
		if(status == -1) {
			printf("Connection Error!\n");
			continue;
		}

		printf("(1)Pan 5~95, (2)Tilt 30~95  ex)1=50 2=50\nInput: ");
		scanf("%[^\n]s", &buf);
		getchar();

		if(write(s, buf, MAXBUF) == -1) {
			perror("Error : ");
		}

		memset(buf, 0, MAXBUF);

		read_len = read(s, buf, MAXBUF);
		if (read_len > 0) {
			strcpy(file_name, buf);
			printf("%s\n",  file_name);
		}
		else {
			perror("Error : ");
			break;
		}

		FILE_OPEN:
		des_fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL, 0700);
		if (!des_fd) {
			perror("file open error : ");
			break;
		}
		if (errno == EEXIST) {
			close(des_fd);
			printf("Go to FILE_OPEN\n");
			goto FILE_OPEN;
		}

		while (1) {
			memset(buf, 0x00, MAXBUF);
			file_read_len = read(s, buf, MAXBUF);
			write(des_fd, buf, file_read_len);
			if (file_read_len == EOF | file_read_len == 0) {
				printf("finish file\n");
				break;
			}
		}

		close(s);
		break;
	}
}
