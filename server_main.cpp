#include <iostream>
#include <string>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void printUsage(const std::string &program_name) {
  std::cout << "Usage: " << program_name << " [PORT]"
            << std::endl;
  std::cout << "Example: " << program_name << " 3306" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printUsage(argv[0]);
    return -1;
  }
  char wr_buff[256];
  struct sockaddr_in addr_server = {};
  struct sockaddr_in addr_client = {};
  socklen_t addr_client_len = sizeof(addr_client_len);
  std::memset(&addr_server, 0, sizeof(addr_server));
  addr_server.sin_family = AF_INET;
  addr_server.sin_port = htons(std::atoi(argv[1]));
  addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
  int sock_server = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, 기본 프로토콜
  if (sock_server == -1) {
    std::cerr << "socket() error" << std::endl;
    close(sock_server);
    exit(1);
  }
  if (bind(sock_server, (struct sockaddr *) &addr_server, sizeof(addr_server)) == -1) {
    std::cerr << "bind() error" << std::endl;
    close(sock_server);
    exit(1);
  }
  if (listen(sock_server, 3) == -1) { // 최대 3개의 클라이언트가 대기할 수 있음
    std::cerr << "listen() error" << std::endl;
    close(sock_server);
    exit(1);
  }
  int sock_client = accept(sock_server, (struct sockaddr *) &addr_client, &addr_client_len);
  if (sock_client == -1) {
    std::cerr << "accept() error" << std::endl;
    close(sock_server);
    exit(1);
  }
  while (1) {
    std::memset(wr_buff, 0, 256);
    int read_size = read(sock_client, wr_buff, sizeof(wr_buff) - 1);
    if (read_size == -1) {
      std::cerr << "read() error" << std::endl;
      break;
    }
    wr_buff[std::strlen(wr_buff)] = '\n';
    std::cout << "Client: " << wr_buff;
    int write_size = write(sock_client, wr_buff, std::strlen(wr_buff));
    if (write_size == -1) {
      std::cerr << "write() error" << std::endl;
      break;
    }
  }
  close(sock_server);
  return 0;
}