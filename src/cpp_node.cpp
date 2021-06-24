#include <iostream>
#include <string.h>
#include <unistd.h>

#define __IN_CPP_NODE_DEFINE__

#include "../includes/cppNode.hpp"

#define RESOURCE_DIR "public"

cpp_node::Http_Server::Http_Server(int port, char *root)
{

	int_to_str(this->port, port);

	memcpy(this->root_dir, root, strlen(root));
}


//handle routes
//TODO: Need to implement 
void cpp_node::Http_Server::handleRoutes(Request *request)
{

	response_msg(request->clnt_sock, "cppnode: Done");
}


void cpp_node::Http_Server::close_server(){

}

int cpp_node::Http_Server::start()
{

	const int MAX_SIZE = (strlen(root_dir) + strlen(RESOURCE_DIR) + 2) * sizeof(char);

	char *const resource_path =
	    (char *)malloc(MAX_SIZE);

	if (resource_path == NULL)
	{
		DieWithSystemMessage("Unable to allocate resources");
	}

	if (join_path(resource_path, MAX_SIZE, root_dir, RESOURCE_DIR) < 0)
	{
		DieWithUserMessage("join_path:", "Unable to work");
	}

	int servSock = SetupTCPServerSocket(port);

	// serveSock is now ready to accept connection
	// Create an input stream from the socket
	for (;;)
	{
		// Wait for client to connect
		int clntSock = AcceptTCPConnection(servSock);
		HandleClient(clntSock, resource_path, this->root_dir, cpp_node::handle_routes);
		close(clntSock);
	} // Each client

	free(resource_path);
	return 0;
}

void cpp_node::Routes::add_route(char *name, cpp_node::CPP_ROUTE route)
{
}

void cpp_node::Routes::remove_route(char *name, cpp_node::CPP_ROUTE route)
{
}

cpp_node::CPP_ROUTE cpp_node::Routes::get_route(char *name)
{
}

cpp_node::Http_Server *cpp_node::allocate_server(int port, char *root_dir)
{
	if (cpp_node::_http_server == NULL)

		cpp_node::_http_server = new cpp_node::Http_Server(port, root_dir);

	return cpp_node::_http_server;
}


int cpp_node::init_server()
{
	if (cpp_node::_http_server != NULL)
	{
		return cpp_node::_http_server->start();
	}
	return -1;
}
void cpp_node::close_server()
{
	if (cpp_node::_http_server != NULL)
	{
		cpp_node::_http_server->close_server();
		delete cpp_node::_http_server;
		cpp_node::_http_server = NULL;
	}
}

void cpp_node::handle_routes(Request *request)
{
	if (cpp_node::_http_server != NULL)
	{
		cpp_node::_http_server->handleRoutes(request);
	}
}
#undef __IN_CPP_NODE_DEFINE__