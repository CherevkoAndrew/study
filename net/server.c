#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define N 7
#define M 2

char charBuf[N+1] = "server!";
long longBuf[M];

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

  rc = recv(s1, longBuf, sizeof(long)*M, 0);
  if (rc <= 0){
    perror("ошибка вызова recv");
    return 1;
  }

  printf("%s %ld %ld\n", charBuf, longBuf[0], longBuf[1]);

  /*rc = send(s1, "2", 1, 0);
  if (rc <= 0)
    perror("ошибка вызова send");*/
  return 0;
}
