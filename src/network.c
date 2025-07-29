/*4:*/
#line 158 "weaver-network.cweb"

#include "network.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten.h> 
#endif
#line 163 "weaver-network.cweb"
/*5:*/
#line 182 "weaver-network.cweb"

#if defined(_WIN32)
#include <winsock2.h> 
#pragma comment(lib,"wsock32.lib")
#endif
#line 187 "weaver-network.cweb"
/*:5*//*6:*/
#line 193 "weaver-network.cweb"

#if defined(__unix__)
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#endif
#line 199 "weaver-network.cweb"
/*:6*//*7:*/
#line 214 "weaver-network.cweb"

#include <stdlib.h>  
/*:7*//*20:*/
#line 465 "weaver-network.cweb"

#if defined(__unix__)
#include <unistd.h>  
#endif
#line 469 "weaver-network.cweb"
/*:20*//*24:*/
#line 539 "weaver-network.cweb"

#if defined(__unix__)
#include <arpa/inet.h>    
#include <stdio.h>       
#include <sys/types.h>  
#include <ifaddrs.h>   
#endif
#line 546 "weaver-network.cweb"
/*:24*//*32:*/
#line 752 "weaver-network.cweb"

#include <string.h>  
/*:32*/
#line 163 "weaver-network.cweb"

/*15:*/
#line 400 "weaver-network.cweb"

#if !defined(W_PORT)
#define W_PORT 0
#endif
#line 404 "weaver-network.cweb"
/*:15*//*16:*/
#line 409 "weaver-network.cweb"

#define TYPE_UNKNOWN 0
#define TYPE_IPV4    1
#define TYPE_IPV6    2
#define TYPE_WEBRTC  3
/*:16*/
#line 164 "weaver-network.cweb"


/*8:*/
#line 218 "weaver-network.cweb"



static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:8*/
#line 166 "weaver-network.cweb"

/*29:*/
#line 710 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
EM_ASYNC_JS(int,new_rtc_connection,(),{
const peerConnection= new RTCPeerConnection({'iceServers':iceServers});
const offer= await peerConnection.createOffer();
await peerConnection.setLocalDescription(offer);
const index= _Wlist_of_connections.push(peerConnection);
return index-1;
});
#endif
#line 720 "weaver-network.cweb"
/*:29*//*30:*/
#line 726 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
EM_JS(int,rtc_connection_data_size,(int index),{
return JSON.stringify(_Wlist_of_connections[index].localDescription).length;
});
#endif
#line 732 "weaver-network.cweb"
/*:30*//*31:*/
#line 740 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
EM_JS(char*,get_rtc_connection_data,(int index),{
return stringToNewUTF8(JSON.stringify(_Wlist_of_connections[index].localDescription));
});
#endif
#line 746 "weaver-network.cweb"
/*:31*/
#line 167 "weaver-network.cweb"

/*9:*/
#line 227 "weaver-network.cweb"


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
#line 239 "weaver-network.cweb"
/*27:*/
#line 678 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
emscripten_run_script("_Wlist_of_connections = [];");
#endif
#line 682 "weaver-network.cweb"
/*:27*//*28:*/
#line 697 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
emscripten_run_script(
"var iceServers;"
"iceServers = ((iceServers===undefined)?([]):(iceServers));");
#endif
#line 703 "weaver-network.cweb"
/*:28*/
#line 239 "weaver-network.cweb"

return;
}
/*:9*//*10:*/
#line 247 "weaver-network.cweb"

void _Wfinish_network(void){
#if defined(_WIN32) 
WSACleanup();
#endif
#line 252 "weaver-network.cweb"
 return;
}
/*:10*//*14:*/
#line 372 "weaver-network.cweb"

#if !defined(__EMSCRIPTEN__)
struct connection*_Wcreate_connection(void){
struct connection*cnt;
cnt= permanent_alloc(sizeof(struct connection));
if(cnt!=NULL){
cnt->local_data= cnt->peer_data= NULL;
cnt->_conn_handle= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
if(cnt->_conn_handle==0){
if(permanent_free!=NULL)
permanent_free(cnt);
return NULL;
}
cnt->active= false;
/*21:*/
#line 471 "weaver-network.cweb"

{
bool fail= false;
#if defined(W_PREFER_IPV4)
/*18:*/
#line 424 "weaver-network.cweb"

{
struct sockaddr_in address;
address.sin_family= AF_INET;
address.sin_addr.s_addr= INADDR_ANY;
address.sin_port= htons((unsigned short)W_PORT);
if(bind(cnt->_conn_handle,(struct sockaddr*)&address,
sizeof(struct sockaddr_in))<0){
fail= true;
}
else{
fail= false;
cnt->_connection_type= TYPE_IPV4;
}
}
/*:18*/
#line 475 "weaver-network.cweb"

if(fail){
/*19:*/
#line 443 "weaver-network.cweb"

{
struct sockaddr_in6 address;
address.sin6_family= AF_INET6;
address.sin6_addr= in6addr_any;
address.sin6_port= htons((unsigned short)W_PORT);
if(bind(cnt->_conn_handle,(struct sockaddr*)&address,
sizeof(struct sockaddr_in6))<0){
fail= true;
}
else{
fail= false;
cnt->_connection_type= TYPE_IPV6;
}
}
/*:19*/
#line 477 "weaver-network.cweb"

}
#else
#line 480 "weaver-network.cweb"
/*19:*/
#line 443 "weaver-network.cweb"

{
struct sockaddr_in6 address;
address.sin6_family= AF_INET6;
address.sin6_addr= in6addr_any;
address.sin6_port= htons((unsigned short)W_PORT);
if(bind(cnt->_conn_handle,(struct sockaddr*)&address,
sizeof(struct sockaddr_in6))<0){
fail= true;
}
else{
fail= false;
cnt->_connection_type= TYPE_IPV6;
}
}
/*:19*/
#line 480 "weaver-network.cweb"

if(fail){
/*18:*/
#line 424 "weaver-network.cweb"

{
struct sockaddr_in address;
address.sin_family= AF_INET;
address.sin_addr.s_addr= INADDR_ANY;
address.sin_port= htons((unsigned short)W_PORT);
if(bind(cnt->_conn_handle,(struct sockaddr*)&address,
sizeof(struct sockaddr_in))<0){
fail= true;
}
else{
fail= false;
cnt->_connection_type= TYPE_IPV4;
}
}
/*:18*/
#line 482 "weaver-network.cweb"

}
#endif
#line 485 "weaver-network.cweb"
 if(fail){
#if defined(__unix__)
close(cnt->_conn_handle);
#elif defined(_WIN32)
#line 489 "weaver-network.cweb"
closesocket(cnt->_conn_handle);
#endif
#line 491 "weaver-network.cweb"
 if(permanent_free!=NULL)
permanent_free(cnt);
return NULL;
}
}
/*:21*//*22:*/
#line 504 "weaver-network.cweb"

#if defined(__unix__)
{
if(fcntl(cnt->_conn_handle,F_SETFL,O_NONBLOCK,1)==-1){
close(cnt->_conn_handle);
if(permanent_free!=NULL)
permanent_free(cnt);
return NULL;
}
}
#endif
#line 515 "weaver-network.cweb"
/*:22*//*23:*/
#line 519 "weaver-network.cweb"

#if defined(_WIN32)
{
DWORD non_block= 1;
if(ioctlsocket(cnt->_conn_handle,FIONBIO,&non_block)!=0){
socketclose(cnt->_conn_handle);
if(permanent_free!=NULL)
permanent_free(cnt);
return NULL;
}
}
#endif
#line 531 "weaver-network.cweb"
/*:23*//*25:*/
#line 548 "weaver-network.cweb"

if(cnt->_connection_type==TYPE_IPV4){
struct ifaddrs*ifap,*ifa;
struct sockaddr_in*sa,address;
char*addr,*c;
int counter= 0;
socklen_t size= sizeof(struct sockaddr_in);
char port[6];

getifaddrs(&ifap);
for(ifa= ifap;ifa;ifa= ifa->ifa_next)
if(ifa->ifa_addr&&ifa->ifa_addr->sa_family==AF_INET)
counter++;
freeifaddrs(ifap);





cnt->local_data= (char*)permanent_alloc(22*counter+1);
if(cnt->local_data==NULL){
close(cnt->_conn_handle);
return NULL;
}

if(getsockname(cnt->_conn_handle,(struct sockaddr*)&address,
&size)!=0){
close(cnt->_conn_handle);
return NULL;
}
sprintf(port,"%d",ntohs(address.sin_port));


getifaddrs(&ifap);
cnt->local_data[0]= '\0';
c= cnt->local_data;
for(ifa= ifap;ifa;ifa= ifa->ifa_next){
if(ifa->ifa_addr&&ifa->ifa_addr->sa_family==AF_INET){
sa= (struct sockaddr_in*)ifa->ifa_addr;
addr= inet_ntoa(sa->sin_addr);
strncat(c,addr,16);
c+= strlen(addr);
*c= ':';
c++;
*c= '\0';
strncat(c,port,6);
c+= strlen(port);
*c= ';';
c++;
*c= '\0';
}
}
c--;
*c= '\0';
freeifaddrs(ifap);
}
/*:25*//*26:*/
#line 608 "weaver-network.cweb"

if(cnt->_connection_type==TYPE_IPV6){
struct ifaddrs*ifap,*ifa;
struct sockaddr_in6*sa,address;
char*c;
int counter= 0;
socklen_t size= sizeof(struct sockaddr_in6);
char port[6];

getifaddrs(&ifap);
for(ifa= ifap;ifa;ifa= ifa->ifa_next)
if(ifa->ifa_addr&&ifa->ifa_addr->sa_family==AF_INET6)
counter++;
freeifaddrs(ifap);





cnt->local_data= (char*)permanent_alloc(65*counter+1);
if(cnt->local_data==NULL){
close(cnt->_conn_handle);
return NULL;
}

if(getsockname(cnt->_conn_handle,(struct sockaddr*)&address,
&size)!=0){
close(cnt->_conn_handle);
return NULL;
}
sprintf(port,"%d",ntohs(address.sin6_port));


getifaddrs(&ifap);
cnt->local_data[0]= '\0';
c= cnt->local_data;
for(ifa= ifap;ifa;ifa= ifa->ifa_next){
if(ifa->ifa_addr&&ifa->ifa_addr->sa_family==AF_INET6){
sa= (struct sockaddr_in6*)ifa->ifa_addr;
*c= '[';
c++;
inet_ntop(AF_INET6,&(sa->sin6_addr),c,46);
while(*c!='\0')c++;
*c= ']';
c++;
*c= ':';
c++;
*c= '\0';
strncat(c,port,6);
c+= strlen(port);
*c= ';';
c++;
*c= '\0';
}
}
c--;
*c= '\0';
freeifaddrs(ifap);
}
/*:26*/
#line 386 "weaver-network.cweb"

}
return cnt;
}
#endif
#line 391 "weaver-network.cweb"
/*:14*//*33:*/
#line 756 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
struct connection*_Wcreate_connection(void){
struct connection*cnt;
cnt= permanent_alloc(sizeof(struct connection));
if(cnt!=NULL){
char*temporary_buffer;
int size;
cnt->local_data= cnt->peer_data= NULL;
cnt->active= false;
cnt->_conn_handle= new_rtc_connection();
size= rtc_connection_data_size(cnt->_conn_handle);
cnt->local_data= (char*)permanent_alloc(size+1);
if(cnt->local_data==NULL){
if(permanent_free!=NULL)
permanent_free(cnt);
return NULL;
}
temporary_buffer= get_rtc_connection_data(cnt->_conn_handle);
memcpy(cnt->local_data,temporary_buffer,size+1);
free(temporary_buffer);
}
return cnt;
}
#endif
#line 781 "weaver-network.cweb"
/*:33*//*34:*/
#line 801 "weaver-network.cweb"

#if !defined(__EMSCRIPTEN__)
bool _Wconnect(struct connection*cnt,char*destiny){
int ip_version= 0;
char*c= destiny;

while(*c!='\0'){
if(*c=='.'){
ip_version= 4;
break;
}
if(*c==':'){
ip_version= 6;
break;
}
c++;
}
if(ip_version!=4&&ip_version!=6)
return false;
/*35:*/
#line 832 "weaver-network.cweb"

char*ip,*port= NULL,*address,*separator= NULL;
address= (char*)temporary_alloc(strlen(destiny)+1);
if(address==NULL)
return false;
if(ip_version==4){
int i= 0;
while(destiny[i]!='\0'){
address[i]= destiny[i];
if(address[i]==':')
separator= &(address[i]);
i++;
}
address[i]= '\0';
ip= address;
if(separator)
port= separator+1;
}
/*:35*//*36:*/
#line 856 "weaver-network.cweb"

if(ip_version==6){
int i= 0;
if(destiny[0]=='['){
bool in_ip= true;
i++;
ip= &(address[1]);
while(destiny[i]!='\0'){
address[i]= destiny[i];
if(address[i]==']')
in_ip= false;
if(!in_ip&&address[i]==':')
separator= &(address[i]);
i++;
}
address[i]= '\0';
}
else{
ip= &(address[0]);
while(destiny[i]!='\0'){
address[i]= destiny[i];
i++;
}
address[i]= '\0';
}
if(separator)
port= separator+1;

}
/*:36*//*37:*/
#line 894 "weaver-network.cweb"

#if defined(__unix__)
if(ip_version==4){
struct sockaddr_in sa;

inet_pton(AF_INET,destiny,&(sa.sin_addr));
}
else if(ip_version==6){
struct sockaddr_in6 sa;

inet_pton(AF_INET6,destiny,&(sa.sin6_addr));
}
#endif
#line 907 "weaver-network.cweb"
/*:37*//*38:*/
#line 911 "weaver-network.cweb"

#if defined(_WIN32)
if(ip_version==4){
struct sockaddr_in sa;

WSAStringToAddressA(destiny,AF_INET,NULL,&sa,sizeof(struct sockaddr_in));

}
else if(ip_version==6){
struct sockaddr_in6 sa;

WSAStringToAddressA(destiny,AF_INET6,NULL,&sa,sizeof(struct sockaddr_in6));
}
#endif
#line 925 "weaver-network.cweb"
/*:38*/
#line 820 "weaver-network.cweb"

return true;
}
#endif
#line 824 "weaver-network.cweb"
/*:34*/
#line 168 "weaver-network.cweb"

/*:4*/
