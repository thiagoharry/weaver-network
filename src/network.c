/*4:*/
#line 146 "weaver-network_en.cweb"

#include "network.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten.h> 
#endif
#line 151 "weaver-network_en.cweb"
#include <stdbool.h>  
#include <string.h>   
#include <unistd.h>   
/*5:*/
#line 173 "weaver-network_en.cweb"

#if defined(_WIN32)
#include <winsock2.h> 
#pragma comment(lib,"wsock32.lib")
#endif
#line 178 "weaver-network_en.cweb"
/*:5*//*6:*/
#line 185 "weaver-network_en.cweb"

#if defined(__unix__)
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <fcntl.h> 
#endif
#line 193 "weaver-network_en.cweb"
/*:6*//*7:*/
#line 207 "weaver-network_en.cweb"

#include <stdlib.h>  
/*:7*/
#line 154 "weaver-network_en.cweb"



/*8:*/
#line 211 "weaver-network_en.cweb"



static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:8*/
#line 157 "weaver-network_en.cweb"

/*11:*/
#line 285 "weaver-network_en.cweb"

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
#line 370 "weaver-network_en.cweb"

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
/*:12*//*13:*/
#line 447 "weaver-network_en.cweb"

void f_255_19_incomplete_add(uint64_t destiny[4],uint64_t sum[4]){
int i;
uint64_t initial_value;
int carry= 0;
for(i= 3;i>=0;i--){
initial_value= destiny[i];
destiny[i]+= sum[i]+carry;
carry= (destiny[i]<initial_value);
carry+= ((destiny[i]==initial_value)*(sum[i]!=0));
}
}
/*:13*//*14:*/
#line 489 "weaver-network_en.cweb"

void f_255_19_normalize(uint64_t n[4]){
int still_big;
uint64_t nineteen[4];
nineteen[0]= 0x0;
nineteen[1]= 0x0;
nineteen[2]= 0x0;
nineteen[3]= 0x0;
nineteen[3]= 19*((n[0]&0x8000000000000000)==0x8000000000000000);
n[0]= (n[0]&0x7fffffffffffffff);
f_255_19_incomplete_add(n,nineteen);
still_big= (n[0]==0x7fffffffffffffff)*(n[1]==~0x0)*
(n[2]==~0x0)*(n[3]> 0xffffffffffffffec);
n[0]-= n[0]*still_big;
n[1]-= n[1]*still_big;
n[2]-= n[2]*still_big;
n[3]-= 0xffffffffffffffed*still_big;
}
/*:14*//*15:*/
#line 517 "weaver-network_en.cweb"

void f_255_19_normalize2(uint64_t input[8],uint64_t output[4]){
int still_big,i;
uint64_t nineteen[4];
nineteen[0]= 0x0;
nineteen[1]= 0x0;
nineteen[2]= 0x0;
nineteen[3]= 0x0;
output[3]= input[7];
output[2]= input[6];
output[1]= input[5];
output[0]= input[4];
f_255_19_normalize(output);
for(i= 0;i<4;i++){
int bit;
for(bit= 0;bit<8;bit++){
int value= ((input[i]>>bit)%2);
nineteen[3]= 19*value;
f_255_19_incomplete_add(output,nineteen);
still_big= ((output[0]==0x7fffffffffffffff)*(output[1]==~0x0)*
(output[2]==~0x0)*(output[3]> 0xffffffffffffffec));
output[0]-= output[0]*still_big;
output[1]-= output[1]*still_big;
output[2]-= output[2]*still_big;
output[3]-= 0xffffffffffffffed*still_big;
}
}
}
/*:15*//*16:*/
#line 568 "weaver-network_en.cweb"

void f_255_19_add(uint64_t a[4],uint64_t b[4]){
f_255_19_incomplete_add(a,b);
f_255_19_normalize(a);
}
/*:16*//*17:*/
#line 580 "weaver-network_en.cweb"

void f_255_19_additive_inverse(uint64_t n[4]){
uint64_t biggest[4],one[4],initial;
int i,carry= 0;
biggest[0]= 0x7fffffffffffffff;
biggest[1]= 0xffffffffffffffff;
biggest[2]= 0xffffffffffffffff;
biggest[3]= 0xffffffffffffffec;
one[0]= one[1]= one[2]= 0;
one[3]= 1;
for(i= 3;i>=0;i--){
initial= n[i];
n[i]= biggest[i]-n[i]-carry;
carry= (n[i]> biggest[i]);
carry= (n[i]==biggest[i])*(initial!=0);
}
f_255_19_add(n,one);
}
/*:17*/
#line 158 "weaver-network_en.cweb"

/*9:*/
#line 220 "weaver-network_en.cweb"

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
#line 231 "weaver-network_en.cweb"

return;
}
/*:9*//*10:*/
#line 239 "weaver-network_en.cweb"

void _Wfinish_network(void){
#if defined(_WIN32) 
WSACleanup();
#endif
#line 244 "weaver-network_en.cweb"
 return;
}
/*:10*/
#line 159 "weaver-network_en.cweb"

/*:4*/
