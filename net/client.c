#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 7
#define M 2

char charBuf[N+1] = "client!";
long longBuf[M];

int main(void){
  srand((unsigned) time(NULL));
  for (int i = 0; i < M; i++) {
    longBuf[i] = rand();
    longBuf[i] = (longBuf[i] << 32) | rand();
  }

  struct sockaddr_in peer;
  int s;
  int rc;

  peer.sin_family = AF_INET;
  peer.sin_port = htons(9999);
  peer.sin_addr.s_addr = inet_addr(/*10.12.40.99"*/"127.0.0.1");

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0){
    perror("ошибка вызова socket");
    //exit(1);
    return 1;
  }

  rc = connect(s, (struct sockaddr *)&peer, sizeof(peer));
  if (rc)
  {
    perror("ошибка вызова connect");
    //exit(1);
    return 1;
  }

  printf("%s %ld %ld\n", charBuf, longBuf[0], longBuf[1]);

  rc = send(s, charBuf, N, 0);
  if (rc <= 0)
  {
    perror("ошибка вызова send");
    //exit(1);
    return 1;
  }

  rc = send(s, longBuf, sizeof(long)*M, 0);
  if (rc <= 0)
  {
    perror("ошибка вызова send");
    //exit(1);
    return 1;
  }

  /*rc = recv(s, charBuf, 1, 0);
  if (rc <= 0)
    perror("oшибка вызова recv");
  else
    printf("%s\n", charBuf);
  //exit(0)*/
  return 0;
}
