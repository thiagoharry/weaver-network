/*4:*/
#line 146 "weaver-network_en.cweb"

#include "network.h"
/*5:*/
#line 167 "weaver-network_en.cweb"

#if defined(_WIN32)
#include <winsock2.h> 
#pragma comment(lib,"wsock32.lib")
#endif
#line 172 "weaver-network_en.cweb"
/*:5*//*6:*/
#line 179 "weaver-network_en.cweb"

#if defined(__unix__)
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#endif
#line 185 "weaver-network_en.cweb"
/*:6*//*7:*/
#line 199 "weaver-network_en.cweb"

#include <stdlib.h>  
/*:7*/
#line 148 "weaver-network_en.cweb"



/*8:*/
#line 203 "weaver-network_en.cweb"



static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:8*/
#line 151 "weaver-network_en.cweb"


/*9:*/
#line 212 "weaver-network_en.cweb"

void _Winit_network(void*(*p_alloc)(size_t),void(*p_free)(void*),
void*(*t_alloc)(size_t),void(*t_free)(void*)){
permanent_alloc= p_alloc;
permanent_free= p_free;
temporary_alloc= t_alloc;
temporary_free= t_free;
#if defined(_WIN32) 
WSADATA WsaData;
WSAStartup(MAKEWORD(2,2),&WsaData);
#endif
#line 223 "weaver-network_en.cweb"
 return;
}
/*:9*//*10:*/
#line 230 "weaver-network_en.cweb"

void _Wfinish_network(void){
#if defined(_WIN32) 
WSACleanup();
#endif
#line 235 "weaver-network_en.cweb"
 return;
}
/*:10*//*14:*/
#line 355 "weaver-network_en.cweb"

#if !defined(__EMSCRIPTEN__)
struct connection*_Wcreate_connection(void){
struct connection*cnt;
cnt= permanent_alloc(sizeof(struct connection));
if(cnt!=NULL){
cnt->local_data= cnt->peer_data= NULL;
cnt->_socket_handle= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
cnt->active= false;
if(cnt->_socket_handle==0){
if(permanent_free!=NULL)
permanent_free(cnt);
return NULL;
}
}
return cnt;
}
#endif
#line 373 "weaver-network_en.cweb"
/*:14*/
#line 153 "weaver-network_en.cweb"

/*:4*/
