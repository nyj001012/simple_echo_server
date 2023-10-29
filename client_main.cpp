#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void printUsage(const std::string &program_name) {
  std::cout << "Usage: " << program_name << " [IP] [PORT]"
            << std::endl;
  std::cout << "Example: " << program_name << " 192.168.0.1 3306" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printUsage(argv[0]);
    return -1;
  }
  struct sockaddr_in addr_server = {};
  std::memset(&addr_server, 0, sizeof(addr_server));
  addr_server.sin_family = AF_INET;
  addr_server.sin_addr.s_addr = inet_addr(argv[1]);
  addr_server.sin_port = htons(std::stoi(argv[2]));

  char w_buff[256];
  char r_buff[256];
  int sock_client = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, 기본 프로토콜
  if (sock_client == -1) {
    std::cout << "socket() error" << std::endl;
    close(sock_client);
    exit(1);
  }
  if (connect(sock_client, (struct sockaddr *) &addr_server, sizeof(addr_server)) == -1) {
    std::cout << "connect() error" << std::endl;
    close(sock_client);
    exit(1);
  }
  while (1) { // 연결되었을 때 실행
    std::memset(r_buff, 0, 256);
    std::cin >> w_buff;
    if (std::strlen(w_buff) > 255) {
      break;
    }
    int write_size = write(sock_client, w_buff, std::strlen(w_buff));
    if (write_size == -1) {
      std::cout << "write() error" << std::endl;
      break;
    }
    int read_size = read(sock_client, r_buff, sizeof(r_buff));
    if (read_size == -1) {
      std::cout << "read() error" << std::endl;
      break;
    } else {
      r_buff[read_size] = '\n';
      std::cout << r_buff;
    }
  }
  close(sock_client);
  return 0;
}
