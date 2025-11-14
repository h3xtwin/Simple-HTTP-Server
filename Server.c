#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVER_PORT 8080

int main() {

  //        >> CREATING THE SOCKET <<
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Creating socket failed: ");
    return EXIT_FAILURE;
  }
  printf("socket created!\n");

  //        >> SOCKET SERVER <<
  struct sockaddr_in serverAddr;

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  printf("created socket server!\n");

  //        >> BIND SOCKET <<
  socklen_t serverLen = sizeof(serverAddr);
  int serverBind =
      bind(sockfd, (const struct sockaddr *)&serverAddr, serverLen);
  if (serverBind < 0) {
    perror("Binding socket failed: ");
    return EXIT_FAILURE;
  }

  printf("binded server\n");

  //        >> LISTEN SOCKET <<
  int serverListen = listen(sockfd, 0);
  if (serverListen < 0) {
    perror("Failed to listen socket: ");
    return EXIT_FAILURE;
  }

  printf("socket is listening\n");

  while (1) {
    //        >> ACCEPT SOCKET <<

    int serverAccept =
        accept(sockfd, (struct sockaddr *)&serverAddr, &serverLen);
    if (serverAccept < 0) {
      perror("Failed to accept socket\n");
      return EXIT_FAILURE;
    }

    printf("connected!\n");

    //       >> SEND MESSAGE <<
    const char httpResponse[128] = "HTTP/1.1 200 OK\r\n"
                                   "Content-Length: 23\r\n"
                                   "Content-Type: text/html\r\n"
                                   "\r\n"
                                   "<h3>Hello Client!</h3>\n";
    if (send(serverAccept, httpResponse, strlen(httpResponse), 0) < 0) {
      perror("failed to send: ");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
