#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 8
#define M 4

char charBuf[N+1] = ">client!";
double doubleBuf[M];

int main(void){
  srand((unsigned) time(NULL));
  for (int i = 0; i < M; i++) {
    doubleBuf[i] = 1.*rand()/rand();
    //longBuf[i] = (longBuf[i] << 32) | rand();

  }

  struct sockaddr_in remote;
  int s, slen=sizeof(remote);
  int rc;

  remote.sin_family = AF_INET;
  remote.sin_port = htons(8888);
  remote.sin_addr.s_addr = inet_addr("127.0.0.1");

  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s < 0){
    perror("ошибка вызова socket");
    //exit(1);
    return 1;
  }

  if (inet_aton("127.0.0.1", &remote.sin_addr)==0) {
    perror("inet_aton err");
    return 1;
  }

  printf("%s %lf %lf %lf %lf\n", charBuf, doubleBuf[0], doubleBuf[1], doubleBuf[2], doubleBuf[3]);

  rc = sendto(s, charBuf, N, 0, (sockaddr*)&remote, (socklen_t)slen);
  if (rc <= 0)
  {
    perror("ошибка вызова send");
    //exit(1);
    return 1;
  }

  rc = sendto(s, doubleBuf, sizeof(double)*M, 0, (sockaddr*)&remote, (socklen_t)slen);
  if (rc <= 0)
  {
    perror("ошибка вызова send");
    //exit(1);
    return 1;
  }


  return 0;
}
