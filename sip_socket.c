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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int udp_socket_create(char *ip, int port, int is_server)
{
    int sock = -1;
    int ret = -1;
    struct sockaddr_in addr;

    if (is_server && (!ip || port < 1024)) {
        printf("Fail to create udp server: Invalid ip or port\n");
        return -1;       
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return -1;
    }

    if (is_server) {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);//htonl(INADDR_ANY);
        addr.sin_port = htons(port);

        ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
        if (ret < 0) {
            perror("Fail to bind");
            return -1;
        }
    }

    return sock;
}
