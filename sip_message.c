/*
 *Copyright (C) 2015 shengy<shengybest@gmail.com>
 *
 *This library is free software; you can redistribute it and/or
 *modify it under the terms of the GNU Lesser General Public
 *License as published by the Free Software Foundation; either
 *version 2.1 of the License, or (at your option) any later version.
 *
 *This library is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *Lesser General Public License for more details.
 *
 *You should have received a copy of the GNU Lesser General Public
 *License along with this library; if not, write to the Free Software
 *Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sip_message.h"

tx_manager_t *tx_manager;
rx_manager_t *rx_manager;

int message_manager_init(tx_manager_t **tx_manager, rx_manager_t **rx_manager)
{
   if (!tx_manager || !rx_manager) {
       printf("%s: Invalid parameter\n", __FUNCTION__);
       return -1;
   }

   *tx_manager = (tx_manager_t *)malloc(sizeof(tx_manager_t));
   if (*tx_manager == NULL) {
       printf("%s: malloc failed\n", __FUNCTION__);
       return -1;
   }

   *rx_manager = (rx_manager_t *)malloc(sizeof(rx_manager_t));
   if (*rx_manager == NULL) {
       printf("%s: malloc failed\n", __FUNCTION__);
       return -1;
   }

   memset(*tx_manager, 0, sizeof(tx_manager_t));
   memset(*rx_manager, 0, sizeof(rx_manager_t));

   return 0;
}

int rx_manager_display(int detail_flag)
{
    int i = 0, j = 0;
    rx_message_t *rx_msg;

    if (!rx_manager) {
       printf("%s: rx_manager is NULL\n", __FUNCTION__);
       return -1;
    }

    printf("********Rx message start**********\n");
    for (i = 0; i < rx_manager->cur_index; i++) {
        rx_msg = &(rx_manager->rx_msg[i]);
        if (!rx_msg) {
            printf("%s: rx_msg is NULL\n", __FUNCTION__);
            return -1;
        }
    printf("********Header start************\n");
        for (j = 0; j < rx_msg->cur_index; j++) {
            if (rx_msg->header[j].used_flag) {
                if (detail_flag) {
                    printf("header name: %s\n", rx_msg->header[j].name);
                }
                printf("%s", rx_msg->header[j].header_value);
            }
        }
    printf("********Header end*****************\n");
    }
    printf("********Rx message end*************\n");

    return 0;   
}

int msg_hdr_name_parse(char *dest, char *src)
{
    if (!src || !dest) {
        printf("%s, Src is NULL\n", __FUNCTION__);
        return -1;
    }

    while (*src != '\0' && *src != ' ') {
        *dest++ = *src++;
    }

    if (*src == '\0') {
        printf("%s, header(%s) is error\n", __FUNCTION__, src);
        return -1;
    }

    *dest = '\0';
    return 0;
}

int rx_msg_hdr_strcpy(rx_message_t *rx_msg, char *buf)
{
    char temp[HEADER_NAME_LEN];
    int ret = -1;
    int index = rx_msg->cur_index;
    if (index < 0 || index > HEADER_MAX_NUM) {
        printf("%s, Invalid cur_index\n", __FUNCTION__);
        return -1;
    }

    if (!buf) {
        printf("%s, buf is NULL\n", __FUNCTION__);
        assert(buf);
        return -1;
    }

    if (buf[0] != 0x0a && buf[0] != 0x0d) {
        ret = msg_hdr_name_parse(temp, buf);
        if (ret < 0) {
            printf("%s, header name parse failed\n", __FUNCTION__);
            return -1;
        }

        strcpy(rx_msg->header[index].name, temp);
    }
    strcpy(rx_msg->header[index].header_value, buf);
    rx_msg->cur_index++;
    rx_msg->header[index].used_flag = 1;

    return 0;
}

int tx_msg_hdr_strcpy(tx_message_t *tx_msg, char *buf)
{
    char temp[HEADER_NAME_LEN];
    int ret = -1;
    int index = tx_msg->cur_index;
    if (index < 0 || index > HEADER_MAX_NUM) {
        printf("%s, Invalid cur_index\n", __FUNCTION__);
        return -1;
    }

    if (!buf) {
        printf("%s, buf is NULL\n", __FUNCTION__);
        assert(buf);
        return -1;
    }

    if (buf[0] != 0x0a && buf[0] != 0x0d) {
        ret = msg_hdr_name_parse(temp, buf);
        if (ret < 0) {
            printf("%s, header name parse failed\n", __FUNCTION__);
            return -1;
        }

        strcpy(tx_msg->header[index].name, temp);
    }

    strcpy(tx_msg->header[index].header_value, buf);
    tx_msg->cur_index++;
    tx_msg->header[index].used_flag = 1;

    return 0;
}

int rx_message_parse(rx_message_t *rx_msg, char *buf, int len)
{
    int ret = -1;   
    if (!rx_msg) {
        printf("%s: rx_message is NULL", __FUNCTION__);
        return -1;
    }

    if (!buf) {
        printf("%s: buf is NULL", __FUNCTION__);
        return -1;
    }

    printf("parse buf: %s, len: %d\n", buf, len);
    ret = rx_msg_hdr_strcpy(rx_msg, buf);
    if (ret < 0) {
        printf("%s: msg_hdr_strcpy failed\n", __FUNCTION__);
    }

    return ret;
}

int tx_message_parse(tx_message_t *tx_msg, char *buf, int len)
{
    int ret = -1;
    if (!tx_msg) {
        printf("%s: tx_message is NULL", __FUNCTION__);
        return -1;
    }

    if (!buf) {
        printf("%s: buf is NULL", __FUNCTION__);
        return -1;
    }

    printf("parse buf: %s, len: %d\n", buf, len);
    ret = tx_msg_hdr_strcpy(tx_msg, buf);
    if (ret < 0) {
        printf("%s: msg_hdr_strcpy failed\n", __FUNCTION__);
    }

    return ret;
}

rx_message_t *get_rx_message(void)
{
    rx_message_t *rx_msg = NULL;

    if (!rx_manager) {
        printf("%s, Invalid index\n", __FUNCTION__);
        return rx_msg;
    }

    if (rx_manager->cur_index < 0 || rx_manager->cur_index >= MESSAGE_MAX_COUNT) {
        printf("%s, Invalid index\n", __FUNCTION__);
        return rx_msg;
    }

    rx_msg = &(rx_manager->rx_msg[rx_manager->cur_index]);
    if (rx_msg == NULL) {
        printf("%s, rx_msg is NULL\n", __FUNCTION__);
    }

    return rx_msg;
}

tx_message_t *get_tx_message(void)
{
    tx_message_t *tx_msg = NULL;

    if (!tx_manager) {
        printf("%s, Invalid index\n", __FUNCTION__);
        return tx_msg;
    }

    if (tx_manager->cur_index < 0 || tx_manager->cur_index >= MESSAGE_MAX_COUNT) {
        printf("%s, Invalid index\n", __FUNCTION__);
        return tx_msg;
    }

    tx_msg = &(tx_manager->tx_msg[tx_manager->cur_index]);
    if (tx_msg == NULL) {
        printf("%s, rx_msg is NULL\n", __FUNCTION__);
    }

    return tx_msg;
}
