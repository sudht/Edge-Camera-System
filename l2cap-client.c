#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 672

int main(int argc, char **argv)
{
        struct sockaddr_l2 addr = { 0 };
        char buf[BUF_SIZE];
        int sourse_fd;
        int file_name_len, read_len;
        int s, status;
        //    char *message = "hello!";
        char dest[18] = "B8:27:EB:75:79:7A";

        /*
                if(argc < 2)
                {
                        fprintf(stderr, "usage: %s <bt_addr>\n", argv[0]);
                        exit(2);
                }

                strncpy(dest, argv[1], 18);
        */

        // allocate a socket
        s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

        // set the connection parameters (who to connect to)
        addr.l2_family = AF_BLUETOOTH;
        addr.l2_psm = htobs(0x1001);
        str2ba(dest, &addr.l2_bdaddr);

        // connect to server
        status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

        memset(buf, 0x00, BUF_SIZE);
        printf("File Name : Pub.jpg");
        strcpy(buf, "Pub.jpg");
        file_name_len = strlen(buf);

        status = write(s, buf, file_name_len);
        sourse_fd = open(buf, O_RDONLY);
        if (!sourse_fd) {
                perror("Error : ");
                return 1;
        }
        
         while (1) {
                memset(buf, 0x00, BUF_SIZE);
                read_len = read(sourse_fd, buf, BUF_SIZE);
                status = write(s, buf, BUF_SIZE);
                if (status < 0) perror("uh oh");
                if (read_len == 0)      break;
        }
        // send a message
        /*
        if (status == 0) {
                status = write(s, "hello!", 6);
        }

        if (status < 0) perror("uh oh");
        */

        close(s);
}
