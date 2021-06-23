#include<iostream>
#include "../includes/cppNode.h"
#include <string.h>
#include <unistd.h>

#define RESOURCE_DIR "public"

void handleRoute(Request *req){

    Queries *qrs= req->header.qrs!=NULL?req->header.qrs:(req->header.body.form!=NULL?req->header.body.form:NULL);
    
    if(qrs!=NULL)
    for(int i=0;i<qrs->length;i++)
    fprintf(stderr,"%s:%s\n",qrs->queries[i].name,qrs->queries[i].value);
  
	response_msg(req->clnt_sock,"cppnode: Hello");
}

int http_mpm_server_start(char *port, char *root_dir) {

  if (port==NULL||root_dir==NULL) {
    DieWithUserMessage("Parameter(s)", "<Server Port/Service> ,root dir");
  }

 const int MAX_SIZE = (strlen(root_dir)+ strlen(RESOURCE_DIR)+2) * sizeof(char);

   char * const resource_path =
   (char *)malloc(MAX_SIZE);

 if(resource_path==NULL){
   DieWithSystemMessage("Unable to allocate resources");
  }

 if(join_path(resource_path,MAX_SIZE,root_dir, RESOURCE_DIR)<0){
   DieWithUserMessage("join_path:","Unable to work");
 }

  int servSock = SetupTCPServerSocket(port);

  // serveSock is now ready to accept connection
  // Create an input stream from the socket
  for (;;) {
    // Wait for client to connect 
    int clntSock = AcceptTCPConnection(servSock);
    HandleClient(clntSock, resource_path,root_dir,handleRoute);
    close(clntSock);
  } // Each client

  free(resource_path);
  return 0;
}

int main(int argc, char **argv){

	if(argc!=3){
		fprintf(stderr,"%s: <port> <current_directory>\n",argv[0]);
		return 1;
	}

	http_mpm_server_start(argv[1],argv[2]);

	return 0;
}