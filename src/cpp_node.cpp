#include <iostream>
#include <string.h>
#include <unistd.h>


#define __IN_CPP_NODE_DEFINE__


#include "../includes/cppNode.hpp"

#define RESOURCE_DIR "public"

#define MAX_NUM_WRK_THREADS 4

extern MPM_HTTP_ERROR HTTP_ERROR_CODES;

typedef struct __thr_arg__{
	int clntSock;
	char *resource_path;
	char *root_dir;
	void (*handle_routes)(Request *);
}wrk_args;

//function called by threads worker
void thr_func(void *_arg){

	wrk_args *arg = (wrk_args *)_arg;
	
	HandleClient(arg->clntSock, arg->resource_path, arg->root_dir, arg->handle_routes);
	
	close(arg->clntSock);

	free(arg);
}

cpp_node::Http_Server::Http_Server(int port, char *root)
{

	int_to_str(this->port, port);
	
	memcpy(this->root_dir, root, strlen(root));

	this->routes =  new cpp_node::Routes();
}


void cpp_node::Http_Server::handleRoutes(Request *request)
{
	Response *response = (Response *)malloc(sizeof(Response));

	if(response==NULL){
		std::cout<<"Unableto allcoate memory for response"<<std::endl;
		return;
	}

	response->sockfd =request->clnt_sock;

	if(request->header.route_name==NULL){
		response_msg(request->clnt_sock,"Not a valid route.");
		free(response);
		return;
	}
	if(request->header.file_name!=NULL){
		return;
	}
	cpp_node::CPP_ROUTE route = this->routes->get_route(request->header.url_path,request->header.method);

	if(route==NULL) response_error(response,request,"Not Found",HTTP_ERROR_CODES.CODE_404);
	else {
		//Execute handler attached to this route	
		route(request);
	}
		free(response);

	log_str("End of the handlRoutes");
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

	worker_poll *wrk_poll = allocate_wrk_poll();

	worker_poll_init(wrk_poll,MAX_NUM_WRK_THREADS);

	poll_worker *wrk = NULL;
	
	wrk_args *arg =NULL;

	// serveSock is now ready to accept connection
	// Create an input stream from the socket
	for (;;)
	{
		// Wait for client to connect
		int clntSock = AcceptTCPConnection(servSock);
		wrk  = get_worker_poll(wrk_poll);
		if(wrk!=NULL){
			
			arg=(wrk_args *)malloc(sizeof(wrk_args));
			if(arg==NULL) continue;

			arg->clntSock = clntSock;
			arg->handle_routes = cpp_node::handle_routes;
			arg->resource_path = resource_path;
			arg->root_dir = this->root_dir;

			wrk->arg =arg; 
			wrk->name="client handler";
			wrk->work_func = thr_func;

			start_worker(wrk_poll,wrk);		}
	
	} // Each client

	free(resource_path);
	
	if(wrk_poll!=NULL)
	wrk_poll_free(wrk_poll);

	return 0;
}

void cpp_node::Routes::add_route(cpp_node::CPP_NODE_ROUTER *router)
{
	if(mpm_list_add(this->routers,router)==0){
		return;
	}else {
		fprintf(stderr,"unable to add router to list");
	}
}

void cpp_node::Routes::remove_route(cpp_node::CPP_NODE_ROUTER *router)
{
}

//Allocate Server instance.
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