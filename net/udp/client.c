#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 7
#define M 5

char charBuf[N+1] = "client!";
int longBuf[M];

int main(void){
  srand((unsigned) time(NULL));
  for (int i = 0.; i < M; i++) {
    longBuf[i] = i;
  // longBuf[i] = (longBuf[i] << 32) | rand();

  }

  struct sockaddr_in remote;
  int s, slen=sizeof(remote);
  int rc;

  remote.sin_family = AF_INET;
  remote.sin_port = htons(8888);
  //remote.sin_addr.s_addr = inet_addr("127.0.0.1");

  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s < 0){
    perror("ошибка вызова socket");
    //exit(1);
    return 1;
  }

  if (inet_aton("10.12.40.147", &remote.sin_addr)==0) {
    perror("inet_aton err");
    return 1;
  }

  printf("%s %ld %ld\n", charBuf, longBuf[0], longBuf[1]);


  rc = sendto(s, longBuf, sizeof(int)*M, 0, (sockaddr*)&remote, (socklen_t)slen);
  if (rc <= 0)
  {
    perror("ошибка вызова send");
    //exit(1);
    return 1;
  }


  return 0;
}
