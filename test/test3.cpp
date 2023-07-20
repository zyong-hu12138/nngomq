#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <iostream>
#define MULTICAST_GROUP "239.100.0.1"
#define INTERFACE_ADDR "172.29.1.0"
#define PORT 52000
#define MAX_BUFFER_SIZE 1024
using namespace std;
int main() {
    // int sockfd;
    // struct sockaddr_in local_addr;
    // struct ip_mreq mreq;
    // char buffer[MAX_BUFFER_SIZE];

    // // 创建UDP套接字
    // if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    //     perror("socket");
    //     exit(EXIT_FAILURE);
    // }

    // // 设置SO_REUSEADDR选项，允许重新绑定到同一端口
    // int reuse = 1;
    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    // {
    //     perror("setsockopt");
    //     close(sockfd);
    //     exit(EXIT_FAILURE);
    // }

    // // 设置本地地址和端口
    // memset(&local_addr, 0, sizeof(local_addr));
    // local_addr.sin_family = AF_INET;
    // local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // local_addr.sin_port = htons(PORT);

    // // 绑定本地地址和端口到套接字
    // if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    //     perror("bind");
    //     close(sockfd);
    //     exit(EXIT_FAILURE);
    // }

    // // 设置组播接收网卡接口
    // struct in_addr if_addr;
    // if_addr.s_addr = inet_addr(INTERFACE_ADDR);
    // if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &if_addr, sizeof(if_addr)) < 0) {
    //     perror("setsockopt");
    //     close(sockfd);
    //     exit(EXIT_FAILURE);
    // }

    // // 加入组播组
    // mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
    // mreq.imr_interface.s_addr = inet_addr(INTERFACE_ADDR);
    // if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    //     perror("setsockopt");
    //     close(sockfd);
    //     exit(EXIT_FAILURE);
    // }

    // printf("UDP multicast listener started...\n");

    // while (1) {
    //     // 接收组播消息
    //     memset(buffer, 0, sizeof(buffer));
    //     ssize_t num_bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    //     if (num_bytes < 0) {
    //         perror("recv");
    //         close(sockfd);
    //         exit(EXIT_FAILURE);
    //     } else if (num_bytes == 0) {
    //         // 连接关闭（不太可能在UDP中发生）
    //         printf("Connection closed\n");
    //         break;
    //     } else {
    //         // 打印接收到的组播消息
    //         buffer[num_bytes] = '\0';
    //         printf("Received: %s\n", buffer);
    //     }
    // }

    // // 关闭套接字
    // close(sockfd);
    char *udp_ip = "333";
    char *ip=udp_ip;
    ip="444";
    cout<<udp_ip<<endl;
    return 0;
}
