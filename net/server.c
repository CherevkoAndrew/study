#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define N 8
#define M 4

char charBuf[N+1] = "server!";
double doubleBuf[M];

int main(void){
  struct sockaddr_in local;
  int s;
  int s1;
  int rc;
  char buf[1];

  local.sin_family = AF_INET;
  local.sin_port = htons(7777);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0){
    perror("ошибка вызова socket");
    return 1;
  }

  rc = bind(s, (struct sockaddr *)&local, sizeof(local));
  if (rc < 0){
    perror("ошибка вызова bind");
    return 1;
  }

  rc = listen(s, 5);
  if (rc){
    perror("ошибка вызова listen");
    return 1;
  }

  s1 = accept(s, NULL, NULL);
  if (s1 < 0){
    perror("ошибка вызова accept");
    return 1;
  }

  rc = recv(s1, charBuf, N, 0);
  if (rc <= 0){
    perror("ошибка вызова recv");
    return 1;
  }

  rc = recv(s1, doubleBuf, sizeof(double)*M, 0);
  if (rc <= 0){
    perror("ошибка вызова recv");
    return 1;
  }

  printf("%s %lf %lf %lf %lf\n", charBuf, doubleBuf[0], doubleBuf[1], doubleBuf[2], doubleBuf[3]);

  /*rc = send(s1, "2", 1, 0);
  if (rc <= 0)
    perror("ошибка вызова send");*/
  return 0;
}
