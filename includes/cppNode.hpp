/*
  Author:Oscuro (email:sahiljarial1999@gmail.com)
*/

#ifndef __CPP_NODE__
#define __CPP_NODE__
#ifdef __cplusplus

extern "C"
{
#endif
#include "./mpm-includes/http.h"
#include "./mpm-includes/utils.h"
#include "./mpm-includes/mpm_tcp.h"
#include "./mpm-includes/http_parser_utility.h"
#include "./mpm-includes/mpm_list.h"
#include "./mpm-includes/http_error_codes.h"
#include "./mpm-includes/fs_c.h"
#include "./mpm-includes/error_exit.h"
#ifdef __cplusplus
}
#endif

namespace cpp_node
{

  //Handle for route
  typedef void (*CPP_ROUTE)(Request *);

  class CPP_NODE_ROUTER
  {
  public:
    CPP_ROUTE route_handle;
    char *route_name;
    char *route_method;
  
  private:
    String *_mem;

  public:
    CPP_NODE_ROUTER():_mem(allocate_string(0)){}

    CPP_NODE_ROUTER(CPP_ROUTE handle,String *string)
       :route_handle(handle),
        _mem(string) 
        {}

    ~CPP_NODE_ROUTER(){
         if(this->_mem!=NULL){
           free_string(_mem);
         }
    }

    void dispose(){
      this->~CPP_NODE_ROUTER();
    }

    int set_route(char *name,char *method);
    void set_route_handle(CPP_ROUTE handle);

  };

  //handle routes
  void handle_routes(Request *req);

  int init_server();

  void close_server();



namespace routes_condition_func{

   uint8_t find_router(Node *,void *route);

}
//free router
void free_route(void *route);

class Routes
  {
  private:
    List *routers;

  public:
    void add_route(CPP_NODE_ROUTER *router);

    void remove_route(CPP_NODE_ROUTER *router);

    CPP_ROUTE get_route(char *name,char *method);

    Routes() :routers(allocate_list()){}

    ~Routes(){
    
      if(this->routers!=NULL){
        free_list(this->routers,free_route);
      }
    }
  };

  class Http_Server
  {
  private:
    char port[16];
    char root_dir[1024];

  public:
    Routes *routes;
    //start server
    int start();
    //cloe server
    void close_server();

    void handleRoutes(Request *);

    Http_Server() : port("3001"), root_dir(".")
    {
      this->routes = new Routes();
    };

    Http_Server(int port, char *root_directory);
  };

#ifdef __IN_CPP_NODE_DEFINE__
  Http_Server *_http_server;
#endif

  Http_Server *allocate_server(int port, char *root_dir);

}
#endif