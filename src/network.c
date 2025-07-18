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
#line 470 "weaver-network.cweb"

#include <string.h>  
/*:20*//*26:*/
#line 623 "weaver-network.cweb"

#if defined(__unix__)
#include <arpa/inet.h>  
#endif
#line 627 "weaver-network.cweb"
/*:26*/
#line 163 "weaver-network.cweb"

/*22:*/
#line 521 "weaver-network.cweb"

#if !defined(W_PORT)
#define W_PORT 29827 
#endif
#line 525 "weaver-network.cweb"
/*:22*/
#line 164 "weaver-network.cweb"


/*8:*/
#line 218 "weaver-network.cweb"



static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:8*/
#line 166 "weaver-network.cweb"

/*17:*/
#line 428 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
EM_ASYNC_JS(int,new_rtc_connection,(),{
const peerConnection= new RTCPeerConnection({'iceServers':iceServers});
const offer= await peerConnection.createOffer();
await peerConnection.setLocalDescription(offer);
const index= _Wlist_of_connections.push(peerConnection);
return index-1;
});
#endif
#line 438 "weaver-network.cweb"
/*:17*//*18:*/
#line 444 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
EM_JS(int,rtc_connection_data_size,(int index),{
return JSON.stringify(_Wlist_of_connections[index].localDescription).length;
});
#endif
#line 450 "weaver-network.cweb"
/*:18*//*19:*/
#line 458 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
EM_JS(char*,get_rtc_connection_data,(int index),{
return stringToNewUTF8(JSON.stringify(_Wlist_of_connections[index].localDescription));
});
#endif
#line 464 "weaver-network.cweb"
/*:19*/
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
/*15:*/
#line 396 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
emscripten_run_script("_Wlist_of_connections = [];");
#endif
#line 400 "weaver-network.cweb"
/*:15*//*16:*/
#line 415 "weaver-network.cweb"

#if defined(__EMSCRIPTEN__)
emscripten_run_script(
"var iceServers;"
"iceServers = ((iceServers===undefined)?([]):(iceServers));");
#endif
#line 421 "weaver-network.cweb"
/*:16*/
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
#line 374 "weaver-network.cweb"

#if !defined(__EMSCRIPTEN__)
struct connection*_Wcreate_connection(void){
struct connection*cnt;
cnt= permanent_alloc(sizeof(struct connection));
if(cnt!=NULL){
cnt->local_data= cnt->peer_data= NULL;
cnt->_conn_handle= 0;
cnt->active= false;
}
return cnt;
}
#endif
#line 387 "weaver-network.cweb"
/*:14*//*21:*/
#line 474 "weaver-network.cweb"

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
#line 499 "weaver-network.cweb"
/*:21*//*23:*/
#line 531 "weaver-network.cweb"

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
/*24:*/
#line 562 "weaver-network.cweb"

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
/*:24*//*25:*/
#line 586 "weaver-network.cweb"

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
/*:25*//*27:*/
#line 629 "weaver-network.cweb"

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
#line 642 "weaver-network.cweb"
/*:27*//*28:*/
#line 646 "weaver-network.cweb"

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
#line 660 "weaver-network.cweb"
/*:28*/
#line 550 "weaver-network.cweb"

return true;
}
#endif
#line 554 "weaver-network.cweb"
/*:23*/
#line 168 "weaver-network.cweb"

/*:4*/
