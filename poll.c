#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define MAX_CLIENTS 256
#define PORT 8080
#define BUFF_SIZE 4096

typedef enum {
  STATE_NEW,
  STATE_CONNECTED,
  STATE_DISCONNECTED,
} state_e;

typedef struct {
  int fd;
  state_e state;
  char buffer[BUFF_SIZE];
} clientstate_t;

clientstate_t clientStates[MAX_CLIENTS];

void init_clients() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    clientStates[i].fd = -1;
    clientStates[i].state = STATE_NEW;
    memset(&clientStates[i].buffer, '\0', BUFF_SIZE);
  }
}

int find_free_slot() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clientStates[i].fd == -1) {
      return i;
    }
  }

  return -1;
}

int find_slot_by_fd(int fd) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clientStates[i].fd == fd) {
      return i;
    }
  }

  return -1;
}

int main() {
  int listen_fd, conn_fd, freeSlot;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);

  struct pollfd fds[MAX_CLIENTS + 1];
  int nfds = 1;
  int opt = 1;

  init_clients();

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0) == -1)) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsocketopt");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(listen_fd, 10) == -1) {
    perror("listne");
    exit(EXIT_FAILURE);
  }

  printf("server listening on port %d\n", PORT);

  memset(fds, 0, sizeof(fds));
  fds[0].fd = listen_fd;
  fds[0].events = POLLIN;
  nfds = 1;

  while (1) {
    int ii = 1;
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (clientStates[i].fd != -1) {
        fds[ii].fd = clientStates[i].fd;
        fds[ii].events = POLLIN;
        ii++;
      }
    }

    int n_events = poll(fds, nfds, -1);
    if (n_events == -1) {
      perror("poll");
      exit(EXIT_FAILURE);
    }

    if (fds[0].events & POLLIN) {
      if ((conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, sizeof(client_addr))) == -1) {
        perror("accept");
        continue;
      }

      printf("new connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

      freeSlot = find_free_slot();
      if (freeSlot == -1) {
        printf("server full, closing new conn");
        close(conn_fd);
      } else {
        clientStates[freeSlot].fd = conn_fd;
        clientStates[freeSlot].state = STATE_CONNECTED;
        nfds++;

        printf("slot %d has fd %d\n", freeSlot, clientStates[freeSlot].fd);
      }

      n_events--;
    }

    for (int i = 1; i <= nfds && n_events > 0; i++) {
      if (fds[i].revents & POLLIN) {
        n_events--;

        int fd = fds[i].fd;
        int slot = find_slot_by_fd(fd);
        ssize_t bytes_read = read(fd, &clientStates[slot].buffer, sizeof(clientStates[slot].buffer));

        if (bytes_read <= 0) {
          close(fd);
          if (slot == -1) {
            printf("tried to close fd that doesn't exist?\n");
          } else {
            clientStates[slot].fd = -1;
            clientStates[slot].state = STATE_DISCONNECTED;
            printf("client disconnected or error\n");
            nfds--;
          }
        } else {
          printf("got data from client : %s\n", clientStates[slot].buffer);
        }
      }
    }
  }

  return 0;
}
