/*3:*/
#line 121 "weaver-network.cweb"

#ifndef __WEAVER_NETWORK
#define __WEAVER_NETWORK
#ifdef __cplusplus
extern"C"{
#endif
#include <stdlib.h>  



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
#endif
/*:3*/
