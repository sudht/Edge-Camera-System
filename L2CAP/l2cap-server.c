#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXBUF 1024

int main(int argc, char **argv)
{
        struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
        char buf[MAXBUF] = { 0 };
        char file_name[MAXBUF];
        int s, client, bytes_read;
        int read_len, des_fd, file_read_len;
        socklen_t opt = sizeof(rem_addr);

        // allocate socket
        s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

        // bind socket to port 0x1001 of the first available
        // bluetooth adapter
        loc_addr.l2_family = AF_BLUETOOTH;
        loc_addr.l2_bdaddr = *BDADDR_ANY;
        loc_addr.l2_psm = htobs(0x1001);

        bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

        // put socket into listening mode
        listen(s, 1);

        while (1) {
                // accept one connection
                client = accept(s, (struct sockaddr *)&rem_addr, &opt);

                ba2str(&rem_addr.l2_bdaddr, buf);
                fprintf(stderr, "accepted connection from %s\n", buf);

                memset(buf, 0, MAXBUF);

                read_len = read(client, buf, MAXBUF);
                if (read_len > 0) {
                        strcpy(file_name, buf);
                        printf("%s\n",  file_name);
                }
                else {
                        close(client);
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
                        goto FILE_OPEN;
                }

                while (1) {
                        memset(buf, 0x00, MAXBUF);
                        file_read_len = read(client, buf, MAXBUF);
                        write(des_fd, buf, file_read_len);
                        if (file_read_len == EOF | file_read_len == 0) {
                                printf("finish file\n");
                                break;
                        }
                }
                close(client);
                close(des_fd);
        }
        close(s);

        /*
        // read data from the client
        bytes_read = read(client, buf, MAXBUF);
        if (bytes_read > 0) {
                printf("received [%s]\n", buf);
        }

        // close connection
        close(client);
        close(s);
        */
}
