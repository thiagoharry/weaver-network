#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

//#include "../src/network.h"
#include "../src/network.c"

int numero_de_testes = 0, acertos = 0, falhas = 0;
void imprime_resultado(void){
  printf("\n%d tests: %d sucess, %d fails\n\n",
	 numero_de_testes, acertos, falhas);
}

void assert(char *descricao, bool valor){
  char pontos[72], *s = descricao;
  size_t tamanho_string = 0;
  int i;
  while(*s)
    tamanho_string += (*s++ & 0xC0) != 0x80;
  pontos[0] = ' ';
  for(i = 1; i < 71 - (int) tamanho_string; i ++)
    pontos[i] = '.';
  pontos[i] = '\0';
  numero_de_testes ++;
  printf("%s%s", descricao, pontos);
  if(valor){
#if defined(__unix__) && !defined(__EMSCRIPTEN__)
    printf("\e[32m[OK]\033[0m\n");
#else
    printf("[OK]\n");
#endif
    acertos ++;
  }
  else{
#if defined(__unix__) && !defined(__EMSCRIPTEN__)
    printf("\033[0;31m[FAIL]\033[0m\n");
#else
    printf("[FAIL]\n");
#endif
    falhas ++;
  }
}

void test_detect_address_type(void){
  // The function 'detect_address_type' is an internal function.
  // It gets a string (ended by '\0')as input.
  // It returns '-1' if this cannot be a valid address.
  // It returns 1 if we have a IPv4 address
  // It returns 2 if we have a valid IPv6 Address
  // It returns 3 if we have something that could be a domain name.
  int response1, response2, response3, response4, response5;
  response1 = detect_address_type("ftp.is.co.za");
  if(response1 != 3)
    printf("ERROR: detect_address_type: Not detected as domanin name: 'ftp.is.co.za'\n");
  response2 = detect_address_type("2001:db8::7");
  if(response2 != 2)
    printf("ERROR: detect_address_type: Not detected as IPv6: '2001:db8::7'\n");
  response3 = detect_address_type("192.0.2.16");
  if(response3 != 1)
    printf("ERROR: detect_address_type: Not detected as IPv4: '192.0.2.16'\n");
  response4 = detect_address_type("");
  if(response4 != -1)
    printf("ERROR: detect_address_type: Detected empty string as valid address\n");
  response5 = detect_address_type("a");
  if(response5 != 3)
    printf("ERROR: detect_address_type: Single letter not recognized as address\n");
  assert("Unit Test: 'detect_address_type'", response1 == 3 && response2 == 2 && response3 == 1
	 && response4 == -1 && response5 == 3);  
}

void test_connect_socket(void){
  // The function 'connect_socket is an internal function.
  // It gets a string as first argument: an address to connect (IPv4, IPv6 or domain name)
  // It gets a string as second argument: a service name or a port number in decimal
  // It gets either SOCK_STREAM (TCP) or SOCK_DGRAM (UDP) as third argument.
  // It returns an integer representing an open socket descriptor or -1.
  int response1 = connect_socket("", "", SOCK_STREAM);
  if(response1 != -1)
    printf("ERROR: connect_socket not signaled error connectiong to empty string.\n");
  int response2 = connect_socket("www.example.org", "80", SOCK_STREAM);
  if(response2 == -1)
    printf("ERROR: connect_socket failed connecting to example.org (TCP).\n");
  int response3 = connect_socket("8.8.8.8", "53", SOCK_DGRAM);
  if(response3 == -1)
    printf("ERROR: connect_socket failed connecting to 8.8.8.8 (UDP).\n");
  if(response2 != -1 && response2 == response3)
    printf("ERROR: connect_socket returned same port twice.\n");
  assert("Unit Test: 'connect_socket'", response1 == -1 && response2 != -1 &&
	 response3 != -1 && response2 != response3);
}

void test_f_255_19_add(void){
  bool error = false;
  uint64_t sample1[4], zero[4], one[4], biggest[4];
  int i;
  sample1[0] = UINT64_C(0x0363e4c133519b97);
  sample1[1] = UINT64_C(0x7ed11a9faa6bf87d);
  sample1[2] = UINT64_C(0x55945c9a0368341f);
  sample1[3] = UINT64_C(0x0613a4ed89eaa33a);
  zero[0] = zero[1] = zero[2] = zero[3] = 0x0;
  one[0] = one[1] = one[2] = 0x0; one[3] = 0x1;
  biggest[0] = UINT64_C(0x7fffffffffffffff);
  biggest[1] = UINT64_C(0xffffffffffffffff);
  biggest[2] = UINT64_C(0xffffffffffffffff);
  biggest[3] = UINT64_C(0xffffffffffffffec);
  f_255_19_add(sample1, zero);
  if(sample1[0] != UINT64_C(0x0363e4c133519b97) ||
     sample1[1] != UINT64_C(0x7ed11a9faa6bf87d) ||
     sample1[2] != UINT64_C(0x55945c9a0368341f) ||
     sample1[3] != UINT64_C(0x0613a4ed89eaa33a)){
    printf("ERROR: Group F_255_19: zero is not neutral in addition.\n");
    error = true;
  }
  if(!error){
    for(i = 0; i < 255; i ++){
      f_255_19_add(one, one);
    }
    if(one[0] != 0x0 || one[1] != 0x0 || one[2] != 0x0 || one[3] != 19){
      printf("ERROR: Group F_255_19: 2^255 is not 19.\n");
      error = true;
    }
  }
  if(!error){
    f_255_19_add(biggest, biggest);
    if(biggest[0] != UINT64_C(0x7fffffffffffffff) ||
       biggest[1] != UINT64_C(0xffffffffffffffff) ||
       biggest[2] != UINT64_C(0xffffffffffffffff) ||
       biggest[3] != UINT64_C(0xffffffffffffffeb)){
      printf("ERROR: Group F_255_19: (-1)+(-1) is not -2.\n");
      printf("EXPECTED: 0x7fffffffffffffff 0xffffffffffffffff "
	     "0xffffffffffffffff 0xffffffffffffffeb\n");
      printf("FOUND:    %lx %lx %lx %lx\n", biggest[0], biggest[1],
	     biggest[2], biggest[3]);
      error = true;
    }
  }
  assert("Unit Test: 'f_255_19_add'", !error);
}

void test_f_255_19_additive_inverse(void){
  bool error = false;
  uint64_t sample[4], inverse[4], zero[4];
  sample[0] = inverse[0] = 0x03D759DE9824881D;
  sample[1] = inverse[1] = 0x572F1E0FDA9F1845;
  sample[2] = inverse[2] = 0x07ADE778AA2C1261;
  sample[3] = inverse[3] = 0x9DF45583E16C6D03;
  zero[0] = zero[1] = zero[2] = zero[3] = 0;
  f_255_19_additive_inverse(zero);
  if(zero[0] != 0 || zero[1] != 0 || zero[2] != 0 || zero[3] != 0){
    printf("%lx %lx %lx %lx\n", sample[0], sample[1], sample[2], sample[3]);
    printf("ERROR: Group F_255_19: -0 is not 0.\n");
    error = true;
  }
  if(!error){
    f_255_19_additive_inverse(inverse);
    f_255_19_add(sample, inverse);
    if(sample[0] != 0 || sample[1] != 0 || sample[2] != 0 || sample[3] != 0){
      printf("%lu %lu %lu %lu\n", sample[0], sample[1], sample[2], sample[3]);
      printf("ERROR: Group F_255_19: Wrong inverse additive number\n");
      error = true;
    }
  }
  assert("Unit Test: 'f_255_19_additive_inverse'", !error);
}

void test_f_255_19_multiply(void){
  bool error = false;
  uint64_t sample1[4], zero[4], one[4], biggest[4], two[4];
  int i;
  sample1[0] = UINT64_C(0x0363e4c133519b97);
  sample1[1] = UINT64_C(0x7ed11a9faa6bf87d);
  sample1[2] = UINT64_C(0x55945c9a0368341f);
  sample1[3] = UINT64_C(0x0613a4ed89eaa33a);
  zero[0] = zero[1] = zero[2] = zero[3] = 0x0;
  one[0] = one[1] = one[2] = 0x0; one[3] = 0x1;
  two[0] = two[1] = two[2] = 0x0; two[3] = 0x2;
  biggest[0] = UINT64_C(0x7fffffffffffffff);
  biggest[1] = UINT64_C(0xffffffffffffffff);
  biggest[2] = UINT64_C(0xffffffffffffffff);
  biggest[3] = UINT64_C(0xffffffffffffffec);
  f_255_19_multiply(sample1, one);
  if(sample1[0] != UINT64_C(0x0363e4c133519b97) ||
     sample1[1] != UINT64_C(0x7ed11a9faa6bf87d) ||
     sample1[2] != UINT64_C(0x55945c9a0368341f) ||
     sample1[3] != UINT64_C(0x0613a4ed89eaa33a)){
    printf("ERROR: Group F_255_19: one is not neutral in multiplication.\n");
    error = true;
  }
  if(!error){
    for(i = 0; i < 255; i ++){
      f_255_19_multiply(one, two);
    }
    if(one[0] != 0x0 || one[1] != 0x0 || one[2] != 0x0 || one[3] != 19){
      printf("ERROR: Group F_255_19: 2^255 is not 19.\n");
      error = true;
    }
  }
  if(!error){
    f_255_19_multiply(biggest, biggest);
    if(biggest[0] != UINT64_C(0x0) ||
       biggest[1] != UINT64_C(0x0) ||
       biggest[2] != UINT64_C(0x0) ||
       biggest[3] != UINT64_C(0x1)){
      printf("ERROR: Group F_255_19: (-1)(-1) is not 1.\n");
      printf("EXPECTED: 0 0 0 1\n");
      printf("FOUND:    %lx %lx %lx %lx\n", biggest[0], biggest[1],
	     biggest[2], biggest[3]);
      error = true;
      // 0x3fffffffffffffff.ffffffffffffffff.ffffffffffffffff.ffffffffffffffec.0000000000000000.0000000000000000.0000000000000000.0000000000000190
      // is indeed the multiplication. The error is when normalizing
      // Expected: begin in 0x190
      // Add 0xfb (0x28b, não 1)
      // Normalizing is wrong!
    }
  }
  assert("Unit Test: 'f_255_19_multiply'", !error);
}


/*void test_initialization(void){
  struct connection *c;
  _Winit_network(malloc, free, malloc, free);
  _Wfinish_network();
  assert("Module initialized and finished", true);
  _Winit_network(malloc, free, malloc, free);

#if defined(__EMSCRIPTEN__)
  emscripten_run_script("iceServers=[{'urls': 'stun:stun.l.google.com:19302'}]");
#endif
  c = _Wcreate_connection();
  assert("Creating new connection", c != NULL && c -> _conn_handle != 0);
  printf("Local data connection: %s\n",
	 c -> local_data);
  _Wfinish_network();
  }*/

int main(int argc, char **argv){
  test_detect_address_type();
  test_connect_socket();
  test_f_255_19_add();
  test_f_255_19_additive_inverse();
  test_f_255_19_multiply();
  //test_initialization();
  imprime_resultado();
  return 0;
}
