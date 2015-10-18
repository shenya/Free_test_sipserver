#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "sip_message.h"
#include "sip_socket.h"

#define FILE_NAME "./200-register-response.txt"
#define STR_LEN 1024
#define RX_MESSAGE_LEN 65536

#define DEFAULT_IP "192.168.40.131"
#define PORT 5060

#define TEMP_FILE "temp.txt"

int sock_fd;
int temp_fd;


int main(int argc, char *argv[])
{
    FILE *fp_tx = NULL;
    FILE *fp_rx = NULL;

    char buf[STR_LEN];
    char message[65536];
    int len = -1;
    int ret = -1;

    rx_message_t *rx_msg = NULL;
    tx_message_t *tx_msg = NULL;

   //Important
    ret = message_manager_init(&tx_manager, &rx_manager);
    if (ret < 0) {
        printf("manager init failed\n");
        exit(1);
    }
   //end

    temp_fd = open(TEMP_FILE, O_RDWR);
    if (temp_fd < 0) {
        perror("Open temp file failed");
        exit(1);
    }


    sock_fd = udp_socket_create(DEFAULT_IP, PORT, 1);
    if (sock_fd < 0) {
        printf("Fail to create socket\n");
        exit(1);
    }
#if 0
    len = read(sock_fd, message, RX_MESSAGE_LEN);
    if (len < 0) {
        perror("Fail to receive message");
    } else 
#endif

{
#if 0
        printf("%s", message);
        write(temp_fd, message, len);
        close(temp_fd);
#endif
        fp_rx = fopen(TEMP_FILE, "r");
        if (!fp_rx) {
            perror("error to open file");
            exit(1);
        }

        rx_msg = get_rx_message();
        if (rx_msg == NULL) {
            printf("%s, rx_msg is NULL\n", __FUNCTION__);
            exit(1);
        }

        while (fgets(buf, STR_LEN, fp_rx)) {
            printf("%s", buf);
            rx_message_parse(rx_msg, buf, strlen(buf));
        }
        rx_manager->cur_index++;
    }

#if 0
    ret = rx_manager_display(1);
    if (ret < 0) {
        printf("error occured\n");
    }
#endif

    /* get tx message from file */
    fp_tx = fopen(FILE_NAME, "r");
    if (!fp_tx) {
        perror("error to open file");
        exit(1);
    }
    
    tx_msg = get_tx_message();
    if (tx_msg == NULL) {
        printf("%s, tx_msg is NULL\n", __FUNCTION__);
        exit(1);
    }

    while (fgets(buf, STR_LEN, fp_tx)) {
        printf("%s", buf);
        tx_message_parse(tx_msg, buf, strlen(buf));
    }

    fclose(fp_tx);
    return 0;
}
