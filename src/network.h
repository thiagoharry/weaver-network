/*3:*/
#line 109 "weaver-network_en.cweb"

#ifndef __WEAVER_NETWORK
#define __WEAVER_NETWORK
#ifdef __cplusplus
extern"C"{
#endif
#line 115 "weaver-network_en.cweb"
#include <stdlib.h>  
/*11:*/
#line 331 "weaver-network_en.cweb"

#include <stdbool.h>  
/*:11*/
#line 116 "weaver-network_en.cweb"


/*12:*/
#line 335 "weaver-network_en.cweb"

struct connection{
bool active;
char*local_data,*peer_data;
/*13:*/
#line 350 "weaver-network_en.cweb"

int _conn_handle;
/*:13*/
#line 339 "weaver-network_en.cweb"

};
/*:12*/
#line 118 "weaver-network_en.cweb"

/*1:*/
#line 75 "weaver-network_en.cweb"

void _Winit_network(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*));
void _Wfinish_network(void);
/*:1*//*2:*/
#line 85 "weaver-network_en.cweb"

struct connection*_Wcreate_connection(void);
/*:2*/
#line 119 "weaver-network_en.cweb"

#ifdef __cplusplus
}
#endif
#line 123 "weaver-network_en.cweb"
#endif
#line 124 "weaver-network_en.cweb"
/*:3*/
