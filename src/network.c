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

void f_255_19_incomplete_add(uint64_t destiny[4],uint64_t sum[4],int size){
int i;
uint64_t initial_value;
int carry= 0;
for(i= size-1;i>=0;i--){
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
f_255_19_incomplete_add(n,nineteen,4);
still_big= (n[0]==0x7fffffffffffffff)*(n[1]==~0x0)*
(n[2]==~0x0)*(n[3]> 0xffffffffffffffec);
n[0]-= n[0]*still_big;
n[1]-= n[1]*still_big;
n[2]-= n[2]*still_big;
n[3]-= 0xffffffffffffffed*still_big;
}
/*:14*//*15:*/
#line 523 "weaver-network_en.cweb"

void f_255_19_add(uint64_t a[4],uint64_t b[4]){
f_255_19_incomplete_add(a,b,4);
f_255_19_normalize(a);
}
/*:15*//*16:*/
#line 535 "weaver-network_en.cweb"

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
/*:16*//*17:*/
#line 575 "weaver-network_en.cweb"

void f_255_19_normalize2(uint64_t src[8],uint64_t dst[4]){

dst[3]= src[7];
dst[2]= src[6];
dst[1]= src[5];
dst[0]= src[4]&0x7fffffffffffffff;

{
int i;
uint64_t aux[4],aux_copy[4];
aux[3]= (src[4]>>63);
aux[3]+= (src[3]<<1);
aux[2]= (src[3]>>63);
aux[2]+= (src[2]<<1);
aux[1]= (src[2]>>63);
aux[1]+= (src[1]<<1);
aux[0]= (src[1]>>63);
aux[0]+= ((src[0]<<2)>>1);
memcpy(aux_copy,aux,sizeof(uint64_t)*4);
for(i= 0;i<4;i++){
f_255_19_add(aux,aux);
}
for(i= 0;i<3;i++){
f_255_19_add(aux,aux_copy);
}
f_255_19_add(dst,aux);
}
}
/*:17*//*18:*/
#line 610 "weaver-network_en.cweb"

void f_255_19_incomplete_multiply(uint64_t a[4],uint64_t b[4],uint64_t r[8]){
bool little_endian= true;
uint64_t aux[8];
uint32_t a32[8],b32[8],aux32[16],carry;
int i,j;
memset(r,0,sizeof(uint64_t)*8);

memcpy(a32,a,sizeof(uint64_t)*4);
memcpy(b32,b,sizeof(uint64_t)*4);
if(little_endian){
for(i= 0;i<8;i+= 2){
uint32_t tmp= a32[i];
a32[i]= a32[i+1];
a32[i+1]= tmp;
tmp= b32[i];
b32[i]= b32[i+1];
b32[i+1]= tmp;
}
}
for(i= 7;i>=0;i--){
memset(aux32,0,sizeof(uint32_t)*16);
carry= 0;
for(j= 7;j>=0;j--){
uint64_t result= ((uint64_t)a32[j])*((uint64_t)b32[i])+carry;
carry= (uint32_t)(result/0x100000000);
aux32[1+i+j]= result%0x100000000;
}
aux32[1+i+j]= carry;
if(little_endian){
for(j= 0;j<16;j+= 2){
uint32_t tmp= aux32[j];
aux32[j]= aux32[j+1];
aux32[j+1]= tmp;
}
}
memcpy(aux,aux32,sizeof(uint64_t)*8);
f_255_19_incomplete_add(r,aux,8);
}
}
/*:18*//*19:*/
#line 656 "weaver-network_en.cweb"

void f_255_19_multiply(uint64_t a[4],uint64_t b[4]){
uint64_t aux[8];
f_255_19_incomplete_multiply(a,b,aux);
f_255_19_normalize2(aux,a);
}
/*:19*//*20:*/
#line 683 "weaver-network_en.cweb"

void f_255_19_multiplicative_inverse(uint64_t g[4]){
int i;
uint64_t t0[4],t1[4],t2[4],t3[4],t4[4],t5[4],t6[4],t7[4],z[4],g2[4];
memcpy(t0,g,sizeof(uint64_t)*4);
f_255_19_multiply(t0,t0);
memcpy(g2,t0,sizeof(uint64_t)*4);
f_255_19_multiply(t0,g);
memcpy(z,t0,sizeof(uint64_t)*4);
f_255_19_multiply(z,z);
f_255_19_multiply(z,g);
memcpy(t1,z,sizeof(uint64_t)*4);
for(i= 0;i<2;i++)
f_255_19_multiply(t1,t1);
f_255_19_multiply(t1,t0);
f_255_19_multiply(z,g2);
f_255_19_multiply(z,g2);
memcpy(t2,t1,sizeof(uint64_t)*4);
for(i= 0;i<5;i++)
f_255_19_multiply(t2,t2);
f_255_19_multiply(t2,t1);
memcpy(t3,t2,sizeof(uint64_t)*4);
for(i= 0;i<10;i++)
f_255_19_multiply(t3,t3);
f_255_19_multiply(t3,t2);
memcpy(t4,t3,sizeof(uint64_t)*4);
for(i= 0;i<20;i++)
f_255_19_multiply(t4,t4);
f_255_19_multiply(t4,t3);
for(i= 0;i<10;i++)
f_255_19_multiply(t4,t4);
f_255_19_multiply(t4,t2);
memcpy(t5,t4,sizeof(uint64_t)*4);
for(i= 0;i<50;i++)
f_255_19_multiply(t5,t5);
f_255_19_multiply(t5,t4);
memcpy(t6,t5,sizeof(uint64_t)*4);
for(i= 0;i<100;i++)
f_255_19_multiply(t6,t6);
f_255_19_multiply(t6,t5);
memcpy(t7,t6,sizeof(uint64_t)*4);
for(i= 0;i<50;i++)
f_255_19_multiply(t7,t7);
f_255_19_multiply(t7,t4);
memcpy(g,t7,sizeof(uint64_t)*4);
for(i= 0;i<5;i++)
f_255_19_multiply(g,g);
f_255_19_multiply(g,z);
}
/*:20*//*21:*/
#line 786 "weaver-network_en.cweb"

void curve25519_double(uint64_t x[4],uint64_t z[4]){
uint64_t a[4],b[4],e[4],constant[4];
constant[0]= constant[1]= constant[2]= 0x0;
constant[3]= 121666;
memcpy(a,x,sizeof(uint64_t)*4);
f_255_19_add(a,z);
f_255_19_multiply(a,a);
memcpy(b,z,sizeof(uint64_t)*4);
f_255_19_additive_inverse(b);
f_255_19_add(b,x);
f_255_19_multiply(b,b);
memcpy(e,b,sizeof(uint64_t)*4);
f_255_19_additive_inverse(e);
f_255_19_add(e,a);
memcpy(x,a,sizeof(uint64_t)*4);
f_255_19_multiply(x,b);
z[0]= z[1]= z[2]= 0x0;z[3]= 121666;
f_255_19_multiply(z,e);
f_255_19_add(z,b);
f_255_19_multiply(z,e);
}
/*:21*//*22:*/
#line 830 "weaver-network_en.cweb"

void curve25519_add(uint64_t x1[4],uint64_t z1[4],
uint64_t x2[4],uint64_t z2[4],
uint64_t xb[4],uint64_t zb[4]){
uint64_t a[4],b[4],c[4],d[4],x[4],z[4];
memcpy(a,x1,sizeof(uint64_t)*4);
f_255_19_add(a,z1);
memcpy(b,z1,sizeof(uint64_t)*4);
f_255_19_additive_inverse(b);
f_255_19_add(b,x1);
memcpy(c,x2,sizeof(uint64_t)*4);
f_255_19_add(c,z2);
memcpy(d,z2,sizeof(uint64_t)*4);
f_255_19_additive_inverse(d);
f_255_19_add(d,x2);
f_255_19_multiply(d,a);
f_255_19_multiply(c,b);
memcpy(x,d,sizeof(uint64_t)*4);
f_255_19_add(x,c);
f_255_19_multiply(x,x);
f_255_19_multiply(x,zb);
memcpy(z,c,sizeof(uint64_t)*4);
f_255_19_additive_inverse(z);
f_255_19_add(z,d);
f_255_19_multiply(z,z);
f_255_19_multiply(z,xb);
memcpy(x1,x,sizeof(uint64_t)*4);
memcpy(z1,z,sizeof(uint64_t)*4);
}
/*:22*//*23:*/
#line 877 "weaver-network_en.cweb"

void cswap(int swap,uint64_t x2[4],uint64_t z2[4],
uint64_t x3[4],uint64_t z3[4]){
uint64_t dummy[4];
uint64_t mask= ((uint64_t)-swap);
dummy[0]= (x2[0]^x3[0])&mask;
dummy[1]= (x2[1]^x3[1])&mask;
dummy[2]= (x2[2]^x3[2])&mask;
dummy[3]= (x2[3]^x3[3])&mask;
x2[0]^= dummy[0];x2[1]^= dummy[1];
x2[2]^= dummy[2];x2[3]^= dummy[3];
x3[0]^= dummy[0];x3[1]^= dummy[1];
x3[2]^= dummy[2];x3[3]^= dummy[3];
dummy[0]= (z2[0]^z3[0])&mask;
dummy[1]= (z2[1]^z3[1])&mask;
dummy[2]= (z2[2]^z3[2])&mask;
dummy[3]= (z2[3]^z3[3])&mask;
z2[0]^= dummy[0];z2[1]^= dummy[1];
z2[2]^= dummy[2];z2[3]^= dummy[3];
z3[0]^= dummy[0];z3[1]^= dummy[1];
z3[2]^= dummy[2];z3[3]^= dummy[3];
}
void curve25519_mult(uint64_t x[4],uint64_t z[4],uint64_t scalar[4]){
int i,swap;
uint64_t one[4],zero[4];
uint64_t x2[4],z2[4],x3[4],z3[4];
zero[0]= zero[1]= zero[2]= zero[3]= 0x0;
one[0]= one[1]= one[2]= 0x0;one[3]= 0x1;
memcpy(x2,one,sizeof(uint64_t)*4);
memcpy(z2,zero,sizeof(uint64_t)*4);
memcpy(x3,x,sizeof(uint64_t)*4);
memcpy(z3,z,sizeof(uint64_t)*4);
swap= 0;
for(i= 254;i>=0;i--){
uint64_t a[4],b[4],aa[4],bb[4],e[4],c[4],d[4],da[4],cb[4];
int bit= (((scalar[3-(i/64)])>>(i%64))&0x1);
swap^= bit;
cswap(swap,x2,z2,x3,z3);
swap= bit;
memcpy(a,x2,sizeof(uint64_t)*4);
f_255_19_add(a,z2);
memcpy(aa,a,sizeof(uint64_t)*4);
f_255_19_multiply(aa,aa);
memcpy(b,z2,sizeof(uint64_t)*4);
f_255_19_additive_inverse(b);
f_255_19_add(b,x2);
memcpy(bb,b,sizeof(uint64_t)*4);
f_255_19_multiply(bb,bb);
memcpy(e,bb,sizeof(uint64_t)*4);
f_255_19_additive_inverse(e);
f_255_19_add(e,aa);
memcpy(c,x3,sizeof(uint64_t)*4);
f_255_19_add(c,z3);
memcpy(d,z3,sizeof(uint64_t)*4);
f_255_19_additive_inverse(d);
f_255_19_add(d,x3);
memcpy(da,d,sizeof(uint64_t)*4);
f_255_19_multiply(da,a);
memcpy(cb,c,sizeof(uint64_t)*4);
f_255_19_multiply(cb,b);
memcpy(x3,da,sizeof(uint64_t)*4);
f_255_19_add(x3,cb);
f_255_19_multiply(x3,x3);
memcpy(z3,cb,sizeof(uint64_t)*4);
f_255_19_additive_inverse(z3);
f_255_19_add(z3,da);
f_255_19_multiply(z3,z3);
f_255_19_multiply(z3,x);
memcpy(x2,aa,sizeof(uint64_t)*4);
f_255_19_multiply(x2,bb);
z2[0]= z2[1]= z2[2]= 0x0;z2[3]= 121665;
f_255_19_multiply(z2,e);
f_255_19_add(z2,aa);
f_255_19_multiply(z2,e);
}
cswap(swap,x2,z2,x3,z3);
memcpy(x,x2,sizeof(uint64_t)*4);
memcpy(z,z2,sizeof(uint64_t)*4);
}
/*:23*/
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
