#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // 创建套接字
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // 设置组播地址和端口
    struct sockaddr_in multicast_addr{};
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr("239.255.0.1");  // 组播 IP 地址
    multicast_addr.sin_port = htons(5000);  // 组播端口号

    // 将套接字与组播地址绑定
    if (bind(sock, reinterpret_cast<struct sockaddr*>(&multicast_addr), sizeof(multicast_addr)) < 0) {
        std::cerr << "Failed to bind socket to multicast address." << std::endl;
        return 1;
    }

    // 设置组播选项
    struct ip_mreq mreq{};
    mreq.imr_multiaddr.s_addr = inet_addr("239.255.0.1");  // 组播 IP 地址
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);  // 本地接口地址（任意）

    // 将套接字加入组播组
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<const char*>(&mreq), sizeof(mreq)) < 0) {
        std::cerr << "Failed to join multicast group." << std::endl;
        return 1;
    }

    // 发送数据
    const char* message = "Hello, multicast!";
    if (sendto(sock, message, strlen(message), 0, reinterpret_cast<struct sockaddr*>(&multicast_addr), sizeof(multicast_addr)) < 0) {
        std::cerr << "Failed to send multicast data." << std::endl;
        return 1;
    }

    // 接收数据
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    socklen_t addr_len = sizeof(multicast_addr);
    ssize_t recv_size = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<struct sockaddr*>(&multicast_addr), &addr_len);
    if (recv_size < 0) {
        std::cerr << "Failed to receive multicast data." << std::endl;
        return 1;
    }

    // 显示接收到的数据
    std::cout << "Received data: " << buffer << std::endl;

    // 关闭套接字
    // close(sock);

    return 0;
}
