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


namespace cpp_node{

typedef void (*CPP_ROUTE)(Request *);


//handle routes
void handle_routes(Request *req);

  int init_server();

  void close_server(); 

  class Routes{
    private:
      CPP_ROUTE route[2048];
      char route_name[2048][1024];

      uint32_t current;
      uint32_t length;

    public:
    void add_route(char *name,CPP_ROUTE route);

    void remove_route(char *name,CPP_ROUTE route);

    CPP_ROUTE get_route(char *name);

    CPP_ROUTE operator[](int index){
      if(index>=this->length){
        return NULL;
      }
      return this->route[index];
    }

    Routes():current(0),length(2048){}
  };



  class Http_Server{
     private:
           char port[16];
           char root_dir[1024];
           Routes *routes;
     public:
          //start server
          int start();
          //cloe server
          void close_server();

          void handleRoutes(Request *);
          
          Http_Server():port("3001"),root_dir("."){
            this->routes = new Routes();
          };

          Http_Server(int port,char *root_directory);
  };

  #ifdef __IN_CPP_NODE_DEFINE__
  Http_Server *_http_server;
  #endif

  Http_Server *allocate_server(int port,char *root_dir);

}

#endif