/*3:*/
#line 124 "weaver-network.cweb"

#ifndef __WEAVER_NETWORK
#define __WEAVER_NETWORK
#ifdef __cplusplus
extern"C"{
#endif
#line 130 "weaver-network.cweb"
#include <stdlib.h>  



/*1:*/
#line 87 "weaver-network.cweb"

void _Winit_network(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
uint64_t(*rand)(void));
void _Wfinish_network(void);
/*:1*//*2:*/
#line 98 "weaver-network.cweb"

struct connection*_Wcreate_connection(void);
/*:2*/
#line 134 "weaver-network.cweb"

#ifdef __cplusplus
}
#endif
#line 138 "weaver-network.cweb"
#endif
#line 139 "weaver-network.cweb"
/*:3*/
