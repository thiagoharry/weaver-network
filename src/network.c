/*4:*/
#line 158 "weaver-network.cweb"

#include "network.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten.h> 
#endif
#include <stdbool.h>  
#include <string.h>   
/*5:*/
#line 184 "weaver-network.cweb"

#if defined(_WIN32)
#include <winsock2.h> 
#pragma comment(lib,"wsock32.lib")
#endif
/*:5*//*6:*/
#line 195 "weaver-network.cweb"

#if defined(__unix__)
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <fcntl.h> 
#endif
/*:6*//*7:*/
#line 218 "weaver-network.cweb"

#include <stdlib.h>  
/*:7*/
#line 165 "weaver-network.cweb"



/*8:*/
#line 222 "weaver-network.cweb"



static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:8*/
#line 168 "weaver-network.cweb"

/*11:*/
#line 320 "weaver-network.cweb"

#define ADDRESS_TYPE_INVALID -1
#define ADDRESS_TYPE_IPV4     1
#define ADDRESS_TYPE_IPV6     2
#define ADDRESS_TYPE_DOMAIN   3
int detect_address_type(char*address){
bool may_be_ipv4= true;
char buffer[4],*c;
int number_of_chars_before_point= 0;
if(address[0]=='\0')
return ADDRESS_TYPE_INVALID;
for(c= address;*c!='\0';c++){
if(*c==':')
return ADDRESS_TYPE_IPV6;
if((*c> 'f'&&*c<='z')||(*c> 'F'&&*c<='Z'))
return ADDRESS_TYPE_DOMAIN;
if(may_be_ipv4){
if(*c=='.'){
int num;
if(number_of_chars_before_point<1||
number_of_chars_before_point> 3){
may_be_ipv4= false;
continue;
}
if(number_of_chars_before_point> 1&&buffer[0]=='0'){
may_be_ipv4= false;
continue;
}
buffer[number_of_chars_before_point]= '\0';
num= atoi(buffer);
if(num> 255){
may_be_ipv4= false;
continue;
}
number_of_chars_before_point= 0;
continue;
}
if(*c<'0'||*c> '9'){
may_be_ipv4= false;
continue;
}
buffer[number_of_chars_before_point]= *c;
number_of_chars_before_point++;
if(number_of_chars_before_point> 3){
may_be_ipv4= false;
continue;
}
}
}
if(may_be_ipv4)
return ADDRESS_TYPE_IPV4;
else
return ADDRESS_TYPE_DOMAIN;
}
/*:11*//*12:*/
#line 406 "weaver-network.cweb"

int connect_socket(char*address,char*port,int type){
struct addrinfo hints;
int s,sfd;
struct addrinfo*result,*rp;
memset(&hints,0,sizeof(struct addrinfo));

hints.ai_family= AF_UNSPEC;
hints.ai_socktype= type;
s= getaddrinfo(address,port,&hints,&result);
if(s!=0)
return-1;
for(rp= result;rp!=NULL;rp= rp->ai_next){
sfd= socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
if(sfd==-1)
continue;
if(connect(sfd,rp->ai_addr,rp->ai_addrlen)!=-1)
break;
close(sfd);
}
freeaddrinfo(result);
if(rp==NULL)
return-1;
return sfd;
}
/*:12*/
#line 169 "weaver-network.cweb"

/*9:*/
#line 231 "weaver-network.cweb"


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

return;
}
/*:9*//*10:*/
#line 251 "weaver-network.cweb"

void _Wfinish_network(void){
#if defined(_WIN32) 
WSACleanup();
#endif
return;
}
/*:10*/
#line 170 "weaver-network.cweb"

/*:4*/
