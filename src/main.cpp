#include<iostream>
#include "../includes/cppNode.hpp"
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv){

  cpp_node::Http_Server *http_server;

	if(argc!=3){
		fprintf(stderr,"%s: <port> <current_directory>\n",argv[0]);
		return 1;
	}

  http_server = cpp_node::allocate_server(atoi(argv[1]),argv[2]);

  http_server->start();
  
	return 0;
}