
#include <arpa/inet.h>
#include <string.h>  // memset()
#include <stdio.h>   // perror()
#include <stdlib.h>  // exit()


int main() {
  struct sockaddr_in me;

  memset(&me, 0, sizeof(me));
  
  me.sin_family = AF_INET;
  me.sin_port   = htons(9991);

  if (inet_aton("127.0.0.1", &me.sin_addr) == 0) {
    perror("inet_aton");
    exit(1);
  }

  int fd = socket(PF_INET, SOCK_DGRAM, 0);

  if (bind(fd, (struct sockaddr*)&me, sizeof(me)) != 0) {
    perror("bind");
    exit(1);
  }


  for (;;) {
    char               buffer[65507];  // Max UDP packet size ((2^16 - 1) - (8 byte UDP header) - (20 byte IP header)).
    struct sockaddr_in other;
    socklen_t          other_len = sizeof(other);

    ssize_t n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&other, &other_len);

    if (n == -1) {
      perror("recvfrom");
      exit(1);
    }

    if (sendto(fd, buffer, n, 0, (struct sockaddr*)&other, other_len) != n) {
      perror("sendto");
      exit(1);
    }
  }
}
