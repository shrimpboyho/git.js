#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#if EMSCRIPTEN
#include <emscripten.h>
#endif

void main_loop(void *arg) {
}

int main() {
  struct sockaddr_in addr;
  int res;
  int serverfd;

  // create the socket
  serverfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverfd == -1) {
    perror("cannot create socket");
    exit(EXIT_FAILURE);
  }
  fcntl(serverfd, F_SETFL, O_NONBLOCK);

  // bind to the supplied port
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(SOCKK);
  if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) {
    perror("inet_pton failed");
    exit(EXIT_FAILURE);
  }

  res = bind(serverfd, (struct sockaddr *)&addr, sizeof(addr));
  if (res == -1) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  close(serverfd);

#if EMSCRIPTEN
  emscripten_set_main_loop(main_loop, 60, 0);
#else
  while (1) main_loop(NULL); sleep(1);
#endif

  return EXIT_SUCCESS;
}
