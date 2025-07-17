/*4:*/
#line 146 "weaver-network_en.cweb"

#include "network.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten.h> 
#endif
#line 151 "weaver-network_en.cweb"
/*5:*/
#line 170 "weaver-network_en.cweb"

#if defined(_WIN32)
#include <winsock2.h> 
#pragma comment(lib,"wsock32.lib")
#endif
#line 175 "weaver-network_en.cweb"
/*:5*//*6:*/
#line 182 "weaver-network_en.cweb"

#if defined(__unix__)
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#endif
#line 188 "weaver-network_en.cweb"
/*:6*//*7:*/
#line 202 "weaver-network_en.cweb"

#include <stdlib.h>  
/*:7*//*20:*/
#line 458 "weaver-network_en.cweb"

#include <string.h>  
/*:20*/
#line 151 "weaver-network_en.cweb"



/*8:*/
#line 206 "weaver-network_en.cweb"



static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:8*/
#line 154 "weaver-network_en.cweb"

/*17:*/
#line 415 "weaver-network_en.cweb"

#if defined(__EMSCRIPTEN__)
EM_ASYNC_JS(int,new_rtc_connection,(),{
const peerConnection= new RTCPeerConnection({'iceServers':iceServers});
const offer= await peerConnection.createOffer();
await peerConnection.setLocalDescription(offer);
const index= _Wlist_of_connections.push(peerConnection);
return index-1;
});
#endif
#line 425 "weaver-network_en.cweb"
/*:17*//*18:*/
#line 432 "weaver-network_en.cweb"

#if defined(__EMSCRIPTEN__)
EM_JS(int,rtc_connection_data_size,(int index),{
return JSON.stringify(_Wlist_of_connections[index].localDescription).length;
});
#endif
#line 438 "weaver-network_en.cweb"
/*:18*//*19:*/
#line 446 "weaver-network_en.cweb"

#if defined(__EMSCRIPTEN__)
EM_JS(char*,get_rtc_connection_data,(int index),{
return stringToNewUTF8(JSON.stringify(_Wlist_of_connections[index].localDescription));
});
#endif
#line 452 "weaver-network_en.cweb"
/*:19*/
#line 155 "weaver-network_en.cweb"

/*9:*/
#line 215 "weaver-network_en.cweb"

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
#line 226 "weaver-network_en.cweb"
/*15:*/
#line 384 "weaver-network_en.cweb"

#if defined(__EMSCRIPTEN__)
emscripten_run_script("_Wlist_of_connections = [];");
#endif
#line 388 "weaver-network_en.cweb"
/*:15*//*16:*/
#line 401 "weaver-network_en.cweb"

#if defined(__EMSCRIPTEN__)
emscripten_run_script(
"var iceServers;"
"iceServers = ((iceServers===undefined)?([]):(iceServers));");
#endif
#line 407 "weaver-network_en.cweb"
/*:16*/
#line 226 "weaver-network_en.cweb"

return;
}
/*:9*//*10:*/
#line 234 "weaver-network_en.cweb"

void _Wfinish_network(void){
#if defined(_WIN32) 
WSACleanup();
#endif
#line 239 "weaver-network_en.cweb"
 return;
}
/*:10*//*14:*/
#line 357 "weaver-network_en.cweb"

#if !defined(__EMSCRIPTEN__)
struct connection*_Wcreate_connection(void){
struct connection*cnt;
cnt= permanent_alloc(sizeof(struct connection));
if(cnt!=NULL){
cnt->local_data= cnt->peer_data= NULL;
cnt->_conn_handle= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
cnt->active= false;
if(cnt->_conn_handle==0){
if(permanent_free!=NULL)
permanent_free(cnt);
return NULL;
}
}
return cnt;
}
#endif
#line 375 "weaver-network_en.cweb"
/*:14*//*21:*/
#line 462 "weaver-network_en.cweb"

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
#line 487 "weaver-network_en.cweb"
/*:21*/
#line 156 "weaver-network_en.cweb"

/*:4*/
