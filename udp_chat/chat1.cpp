#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  char *ipAddr = (char *)malloc(256 * sizeof(char));
  int recv_port;
  int port;
  int client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  strcpy(ipAddr, argv[1]);
  sscanf(argv[2], "%d", &port);
  sscanf(argv[3], "%d", &recv_port);
  int ret;
  // khai báo địa chỉ cuả client
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ipAddr);
  addr.sin_port = htons(port);
  if (bind(client, (struct sockaddr *)&addr, sizeof(addr))) {
    perror("bind() failed.");
  }
  // khai báo địa chỉ đối phương
  struct sockaddr_in sender_addr;
  addr.sin_family = AF_INET;
  sender_addr.sin_addr.s_addr = inet_addr(ipAddr);
  sender_addr.sin_port = htons(recv_port);
  socklen_t sender_addr_len = sizeof(sender_addr);
  socklen_t addr_len = sizeof(addr);
  struct pollfd fds[2];

  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  fds[1].fd = client;
  fds[1].events = POLLIN;

  char buf[256];
  while (1) {
    int ret = poll(fds, 2, -1);
    if (ret < 0) {
      perror("poll() failed");
      break;
    }

    else if (ret == 0) {
      printf("Timed out.\n");
      continue;
    } else {
      printf("ret = %d\n", ret);
    }

    // printf("Send message:");
    if (fds[0].revents & POLLIN) {
      // printf("Send message: \n");
      fgets(buf, sizeof(buf), stdin);
      ret = sendto(client, buf, strlen(buf), 0, (struct sockaddr *)&sender_addr,
                   sizeof(sender_addr));
      if (ret <= 0)
        printf("Send fail.");
    }

    if (fds[1].revents & POLLIN) {
      ret = recvfrom(client, buf, sizeof(buf), 0,
                     (struct sockaddr *)&sender_addr, &sender_addr_len);
      if (ret <= 0) {
        printf("Fail.\n");
        break;
      }
      buf[ret] = 0;
      printf("Received: %s\n", buf);
    }
    // printf("send message:\n");
  }

  // Ket thuc, dong socket
  close(client);

  return 0;
}