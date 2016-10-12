#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define N 8
#define M 4

char charBuf[N+1];
double doubleBuf[M];

int main(void){
  struct sockaddr_in local, remote;
  int s, slen=sizeof(remote);
  int s1;
  int rc;
  char buf[1];

  local.sin_family = AF_INET;
  local.sin_port = htons(8888);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s < 0){
    perror("ошибка вызова socket");
    return 1;
  }

  rc = bind(s, (struct sockaddr *)&local, sizeof(local));
  if (rc < 0){
    perror("ошибка вызова bind");
    return 1;
  }


  rc = recvfrom(s, charBuf, N, 0, (sockaddr*)&remote, (socklen_t*)&slen);//////
  if (rc <= 0){
    perror("ошибка вызова recv udp");
    return 1;
  }

  rc = recvfrom(s, doubleBuf, M*sizeof(double), 0, (sockaddr*)&remote, (socklen_t*)&slen);
  if (rc <= 0){
    perror("ошибка вызова recv udp");
    return 1;
  }
  printf("%s %lf %lf %lf %lf\n", charBuf, doubleBuf[0], doubleBuf[1], doubleBuf[2], doubleBuf[3] );

  return 0;
}
