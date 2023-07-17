#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

int main() {
    // 创建 UDP 套接字
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock == -1) {
        std::cerr << "Failed to create UDP socket." << std::endl;
        return 1;
    }

    // 设置套接字选项 - 允许地址重用
    int reuse = 1;
    if (setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        std::cerr << "Failed to set SO_REUSEADDR option." << std::endl;
        return 1;
    }

    // 设置组播地址和端口
    sockaddr_in multicast_addr{};
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr("239.0.0.1"); // 组播地址
    multicast_addr.sin_port = htons(50001); // 组播端口

    // 绑定到本地地址和端口
    sockaddr_in local_addr{};
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 本地地址
    local_addr.sin_port = htons(50001); // 本地端口
    if (bind(udp_sock, reinterpret_cast<sockaddr*>(&local_addr), sizeof(local_addr)) == -1) {
        std::cerr << "Failed to bind UDP socket." << std::endl;
        return 1;
    }

    // 加入组播组
    ip_mreq multicast_req{};
    multicast_req.imr_multiaddr = multicast_addr.sin_addr;
    multicast_req.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(udp_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<char*>(&multicast_req), sizeof(multicast_req)) == -1) {
        std::cerr << "Failed to join multicast group." << std::endl;
        return 1;
    }

    // 接收组播数据
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // 清空缓冲区
        ssize_t recv_size = recv(udp_sock, buffer, sizeof(buffer) - 1, 0);
        if (recv_size == -1) {
            std::cerr << "Failed to receive data." << std::endl;
            break;
        } else if (recv_size == 0) {
            std::cout << "Connection closed." << std::endl;
            break;
        } else {
            std::cout << "Received data: " << buffer << std::endl;
        }
    }

    // 关闭套接字
    close(udp_sock);

    return 0;
}
