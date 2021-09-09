#include<iostream>
#include "../includes/cppNode.hpp"
#include <string.h>
#include <unistd.h>

void home_route(Request *req)
{
  Response *_response = (Response *)malloc(sizeof(Response));

  char *body = "<h1>Cookies are set.</h2>";
  char len[16];

  _response->sockfd = req->clnt_sock;

  int_to_str(len,strlen(body));
  
  set_responseHeader(_response,CONTENT_TYPE,"text/html");
  set_responseHeader(_response,COOKIES,"mpm_key=123;");
  set_responseHeader(_response,CONTENT_LENGTH,len);
  set_responseHeader(_response,CONNECTION,"close");
  set_responseHeader(_response,STATUS,"200");
  set_responseHeader(_response,ACCEPT_RANGES,"bytes");


  response(_response,body,strlen(body),sizeof(char));

  free(_response);
}

int main(int argc, char **argv){

  cpp_node::Http_Server *http_server;

	if(argc!=3){
		fprintf(stderr,"%s: <port> <current_directory>\n",argv[0]);
		return 1;
	}

  http_server = cpp_node::allocate_server(atoi(argv[1]),argv[2]);

  cpp_node::CPP_NODE_ROUTER *home = new cpp_node::CPP_NODE_ROUTER();
  
  home->set_route("/home","GET");
  
  home->set_route_handle(home_route);

  http_server->routes->add_route(home);

  http_server->start();
  
	return 0;
}