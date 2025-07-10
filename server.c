#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum {
  PROTO_HELLO,
} proto_type_e;

typedef struct {
  proto_type_e type;
  unsigned short len;
} proto_hdr_t;

void handle_client(int fd) {
  char buf[4096] = {0};
  proto_hdr_t *hdr = buf;
  hdr->type = htonl(PROTO_HELLO);
  hdr->len = sizeof(int);

  int reallen = hdr->len;
  hdr->len = htons(hdr->len);

  int *data = (int *)&hdr[1];
  *data = htonl(1);

  write(fd, hdr, sizeof(hdr) + reallen);
}

int main() {
  struct sockaddr_in serverInfo = {0};
  struct sockaddr_in clientInfo = {0};
  int clientSize = 0;

  serverInfo.sin_family = INADDR_ANY;
  serverInfo.sin_addr.s_addr = 0;  // bind to every IP address our computer owns
  serverInfo.sin_port = ntohs(5555);

  int fd = socket(AF_INET, SOCK_STREAM, 0);  // SOCK_STREAM = TCP

  if (fd == -1) {
    perror("socket");
    return -1;
  }

  if (bind(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
    perror("bind");
    close(fd);
    return -1;
  }

  if (listen(fd, 0) == -1) {
    perror("listen");
    close(fd);
    return -1;
  }

  while (1) {
    int cfd = accept(fd, (struct sockaddr *)&clientInfo, &clientSize);
    if (cfd == -1) {
      perror("accept");
      close(fd);
      return -1;
    }

    handle_client(cfd);

    close(fd);
  }

  return 0;
}
