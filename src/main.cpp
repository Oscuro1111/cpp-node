#include<iostream>
#include "../includes/cppNode.hpp"
#include <string.h>
#include <unistd.h>

void home_route(Request *req)
{
	response_msg(req->clnt_sock,"Home route");
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