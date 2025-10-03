/*3:*/
#line 109 "weaver-network_en.cweb"

#ifndef __WEAVER_NETWORK
#define __WEAVER_NETWORK
#ifdef __cplusplus
extern"C"{
#endif
#include <stdlib.h>  



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
#endif
/*:3*/
