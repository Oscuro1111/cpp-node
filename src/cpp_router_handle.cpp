#include "../includes/cppNode.hpp"
#include <string.h>

int cpp_node::CPP_NODE_ROUTER::set_route(char *name, char *method)
{

	String *string;

	uint32_t position;

	if (this->_mem != NULL && name != NULL && method != NULL)
	{

		string = this->_mem;

		insert_string(&string, name);

		position = string->current;

		insert_string(&string, " ");

		insert_string(&string, method);

		string->str[position] = '\0';

		this->route_name = string->str;

		this->route_method = string->str + position + 1;
	}
	else
	{
		fprintf(stderr, "unable to allocate memory for route");
		return -1;
	}

	return 0;
}

void cpp_node::CPP_NODE_ROUTER::set_route_handle(CPP_ROUTE handle)
{
	if (handle != NULL)
	{
		this->route_handle = handle;
	}
	else
	{
		fprintf(stderr, "handle arg cannot be NULL");
	}
}

void cpp_node::free_route(void *route)
{
	cpp_node::CPP_NODE_ROUTER *router = (cpp_node::CPP_NODE_ROUTER *)route;
	router->dispose();
}

uint8_t cpp_node::routes_condition_func::find_router(Node *node, void *ele)
{

	cpp_node::CPP_NODE_ROUTER *router = (cpp_node::CPP_NODE_ROUTER *)ele;
	cpp_node::CPP_NODE_ROUTER *node_ele = (cpp_node::CPP_NODE_ROUTER *)node->element;

	if (strcmp(router->route_name, node_ele->route_name) == 0 && 
	    strcmp(node_ele->route_method, router->route_method) == 0)
	{
		return 1;
	}else return 0;
}

cpp_node::CPP_ROUTE cpp_node::Routes::get_route(char *name, char *method)
{
	if (name == NULL || method == NULL)
		return NULL;
	cpp_node::CPP_NODE_ROUTER *router = new cpp_node::CPP_NODE_ROUTER();

	if(router->set_route(name,method)==0){
	
	   Node *node = mpm_list_find(this->routers,router,cpp_node::routes_condition_func::find_router);

	if(node==NULL) return NULL;
	
	return ((cpp_node::CPP_NODE_ROUTER*) node->element)->route_handle;
   }else return NULL;
}