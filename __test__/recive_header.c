#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include<arpa/inet.h>

void DieWithUserMessage(const char *msg, const char *detail){

	fputs(msg,stderr);

	fputs(":",stderr);
	fputs(detail,stderr);

	fputc('\n', stderr);

	exit(1);
}


void DieWithSystemMessage(const char *msg){

	perror(msg);
	exit(1);
}


void PrintSocketAddress(const struct sockaddr *address, FILE *stream){

  if(address ==NULL||stream ==NULL){ /*  */
    fprintf(stderr,"address or stream cannot be NULL!");
    return;
  }

  //pointer to binary address
  void *numericAddress;
  //storage for printable address
  char addrBuffer[INET6_ADDRSTRLEN];

  in_port_t  port;

  switch(address->sa_family){

  case AF_INET:
    numericAddress = &((struct sockaddr_in *) address)->sin_addr;
    port = ntohs(((struct sockaddr_in *) address)->sin_port);
    break;

  case AF_INET6:
    numericAddress = &((struct sockaddr_in6*)address)->sin6_addr;
    port =ntohs(((struct sockaddr_in6*)address)->sin6_port);
    break;

  default:
    fputs("[Unknown type]",stream);
    return;
  }


  //Convert Binary or numeric to printable address
  if(inet_ntop(address->sa_family,numericAddress,addrBuffer,sizeof(addrBuffer))==NULL){

    fputs("Invalid address",stream);
  }else{

    fprintf(stream,"\nAddress:%s\n",addrBuffer);
    if(port!=0){
      fprintf(stream,"port:%u\n",port);
    }
  }

}

static const int MAXPENDING = 5;
int SetupTCPServerSocket(const char *service) {

  //Construct the server address structure
  struct addrinfo addreCriteria;
  memset(&addreCriteria, 0, sizeof(addreCriteria));

  addreCriteria.ai_family = AF_UNSPEC;

  // Accept on any address/port
  addreCriteria.ai_flags = AI_PASSIVE;

  addreCriteria.ai_socktype = SOCK_STREAM;
  addreCriteria.ai_protocol = IPPROTO_TCP;

  struct addrinfo *servAddr; // List of server address available

  int rtnVal = getaddrinfo(NULL, service, &addreCriteria, &servAddr);

  if (rtnVal != 0) {
    DieWithUserMessage("getaddrinfo() unable to work", gai_strerror(rtnVal));
  }

  int servSock = -1;

  for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Create TCP socket
    servSock = socket(addr->ai_family, addr->ai_socktype,
                      addr->ai_protocol);
    if (servSock < 0) {
      continue;
    }

    // Bind to the local address and set socket to list
    if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
        (listen(servSock, MAXPENDING) == 0)) {
      //Print local address of socket

      struct sockaddr_storage localAddr;

      socklen_t addrSize = sizeof(localAddr);

      if(getsockname(servSock,(struct sockaddr *)&localAddr,&addrSize)<0){

        DieWithSystemMessage("getSockName() unable to work!");
      }
      fputs("Bind to:",stdout);

      PrintSocketAddress((struct sockaddr *)&localAddr,stdout);
      fputc('\n',stdout);
      break; //Bind and list successful
    }
    close(servSock);
    servSock = -1;
  }
  //Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);
  return servSock;
}
int AcceptTCPConnection(int servSock){

  struct sockaddr_storage clntAddr;

  //Set length of client address structure (in-out parameter)
  socklen_t  clntAddrLen =sizeof(clntAddr);

  //Wait for client to connect
  int clntSock = accept(servSock,(struct sockaddr *)&clntAddr,&clntAddrLen);

  if(clntSock<0){
    DieWithSystemMessage("accept() unable to work!");
  }
  //clntSock connected to a client
  fputs("Handle client::",stdout);

  PrintSocketAddress((struct sockaddr *)&clntAddr,stdout);

  fputc('\n',stdout);

  return clntSock;
}


int main(void){
     	int servSock = SetupTCPServerSocket("3000");
      char ch;
        for(;;){
          int clntsock = AcceptTCPConnection(servSock);
           FILE *file = fdopen(clntsock,"rb+");
           while(1){
             fprintf(stderr,"%c",fgetc(file));
           }  
        }
    return 0;
}
