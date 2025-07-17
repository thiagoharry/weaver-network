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

#include "../src/network.h"

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

void test_initialization(void){
  struct connection *c;
  _Winit_network(malloc, free, malloc, free);
  _Wfinish_network();
  assert("Module initialized and finished", true);
  _Winit_network(malloc, free, malloc, free);

#if defined(__EMSCRIPTEN__)
  emscripten_run_script("iceServers=[{'urls': 'stun:stun.l.google.com:19302'}]");
#endif
  c = _Wcreate_connection();
  assert("Creating new connection", c != NULL);
#if defined(__EMSCRIPTEN__)
  printf("Local data connection: %s\n",
	 c -> local_data);
#endif
  _Wfinish_network();
}

int main(int argc, char **argv){
  test_initialization();
  imprime_resultado();
  return 0;
}
