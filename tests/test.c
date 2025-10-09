#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
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
  //test_initialization();
  imprime_resultado();
  return 0;
}
