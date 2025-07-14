/*3:*/
#line 144 "weaver-network.cweb"

#include "network.h"
/*4:*/
#line 165 "weaver-network.cweb"

#if defined(_WIN32)
#include <winsock2.h> 
#pragma comment(lib,"wsock32.lib")
#endif
#line 170 "weaver-network.cweb"
/*:4*//*5:*/
#line 176 "weaver-network.cweb"

#if defined(__unix__)
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#endif
#line 182 "weaver-network.cweb"
/*:5*/
#line 146 "weaver-network.cweb"





/*6:*/
#line 195 "weaver-network.cweb"

void _Winit_network(void){
#if defined(_WIN32) 
WSADATA WsaData;
WSAStartup(MAKEWORD(2,2),&WsaData);
#endif
#line 201 "weaver-network.cweb"
 return;
}
/*:6*//*7:*/
#line 208 "weaver-network.cweb"

void _Wfinish_network(void){
#if defined(_WIN32) 
WSACleanup();
#endif
#line 213 "weaver-network.cweb"
 return;
}
/*:7*/
#line 151 "weaver-network.cweb"

/*:3*/
