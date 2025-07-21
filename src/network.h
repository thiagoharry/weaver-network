/*3:*/
#line 121 "weaver-network.cweb"

#ifndef __WEAVER_NETWORK
#define __WEAVER_NETWORK
#ifdef __cplusplus
extern"C"{
#endif
#line 127 "weaver-network.cweb"
#include <stdlib.h>  
/*11:*/
#line 346 "weaver-network.cweb"

#include <stdbool.h>  
/*:11*/
#line 128 "weaver-network.cweb"


/*12:*/
#line 350 "weaver-network.cweb"

struct connection{
bool active;
char*local_data,*peer_data;
/*13:*/
#line 365 "weaver-network.cweb"

int _conn_handle;
/*:13*//*17:*/
#line 416 "weaver-network.cweb"

int _connection_type;
/*:17*/
#line 354 "weaver-network.cweb"

};
/*:12*/
#line 130 "weaver-network.cweb"

/*1:*/
#line 85 "weaver-network.cweb"

void _Winit_network(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*));
void _Wfinish_network(void);
/*:1*//*2:*/
#line 95 "weaver-network.cweb"

struct connection*_Wcreate_connection(void);
/*:2*/
#line 131 "weaver-network.cweb"

#ifdef __cplusplus
}
#endif
#line 135 "weaver-network.cweb"
#endif
#line 136 "weaver-network.cweb"
/*:3*/
