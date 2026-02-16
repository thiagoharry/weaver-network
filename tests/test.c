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
      printf("FOUND:    %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n",
	     biggest[0], biggest[1], biggest[2], biggest[3]);
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
    printf("%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n",
	   sample[0], sample[1], sample[2], sample[3]);
    printf("ERROR: Group F_255_19: -0 is not 0.\n");
    error = true;
  }
  if(!error){
    f_255_19_additive_inverse(inverse);
    f_255_19_add(sample, inverse);
    if(sample[0] != 0 || sample[1] != 0 || sample[2] != 0 || sample[3] != 0){
      printf("%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n",
	     sample[0], sample[1], sample[2], sample[3]);
      printf("ERROR: Group F_255_19: Wrong inverse additive number\n");
      error = true;
    }
  }
  assert("Unit Test: 'f_255_19_additive_inverse'", !error);
}

void test_f_255_19_multiplicative_inverse(void){
  bool error = false;
  uint64_t sample[4], inverse[4], one[4];
  sample[0] = inverse[0] = 0x03D759DE9824881D;
  sample[1] = inverse[1] = 0x572F1E0FDA9F1845;
  sample[2] = inverse[2] = 0x07ADE778AA2C1261;
  sample[3] = inverse[3] = 0x9DF45583E16C6D03;
  one[0] = one[1] = one[2] = 0; one[3] = 1;
  f_255_19_multiplicative_inverse(one);
  if(one[0] != 0 || one[1] != 0 || one[2] != 0 || one[3] != 1){
    printf("%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n",
	   sample[0], sample[1], sample[2], sample[3]);
    printf("ERROR: Group F_255_19: 1^(-1) is not 1.\n");
    error = true;
  }
  if(!error){
    uint64_t two[4], inverse_two[4];
    two[0] = two[1] = two[2] = 0x0; two[3] = 0x2;
    memcpy(inverse_two, two, sizeof(uint64_t) * 4);
    f_255_19_multiplicative_inverse(inverse_two);
    if(inverse_two[0] != 0x3fffffffffffffff ||
       inverse_two[1] != 0xffffffffffffffff ||
       inverse_two[2] != 0xffffffffffffffff ||
       inverse_two[3] != 0xfffffffffffffff7){
      printf("EXPECTED: 0x3fffffffffffffff 0xffffffffffffffff "
	     "0xffffffffffffffff 0xfffffffffffffff7\n");
      printf("FOUND:    0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64"\n",
	     inverse_two[0], inverse_two[1], inverse_two[2], inverse_two[3]);
      printf("ERROR: Group F_255_19: Wrong multiplicative inverse of 2\n");
      error = true;
    }
  }
  if(!error){
    f_255_19_multiplicative_inverse(inverse);
    f_255_19_multiply(sample, inverse);
    if(sample[0] != 0 || sample[1] != 0 || sample[2] != 0 || sample[3] != 1){
      printf("%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n",
	     sample[0], sample[1], sample[2], sample[3]);
      printf("ERROR: Group F_255_19: Wrong multiplicative inverse number\n");
      error = true;
    }
  }
  assert("Unit Test: 'f_255_19_multiplicative_inverse'", !error);
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
      printf("FOUND:    %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n",
	     biggest[0], biggest[1], biggest[2], biggest[3]);
      error = true;
    }
  }
  assert("Unit Test: 'f_255_19_multiply'", !error);
}

void test_curve25519_double(void){
  bool error = false;
  // Test 1: Infinity point
  uint64_t inf_x[4], inf_z[4];
  inf_x[0] = inf_x[1] = inf_x[2] = 0x0; inf_x[3] = 0x1;
  inf_z[0] = inf_z[1] = inf_z[2] = inf_z[3] = 0x0;
  curve25519_double(inf_x, inf_z);
  if(inf_z[0] != UINT64_C(0x0) || inf_z[1] != UINT64_C(0x0) ||
     inf_z[2] != UINT64_C(0x0) || inf_z[3] != UINT64_C(0x0)){
    printf("ERROR: Curve25519: 0+0 not equal 0.\n");
    error = true;
  }
  if(!error){
    uint64_t nine_x[4], nine_z[4];
    nine_x[0] = nine_x[1] = nine_x[2] = 0x0; nine_x[3] = 0x9;
    nine_z[0] = nine_z[1] = nine_z[2] = 0x0; nine_z[3] = 0x1;
    curve25519_double(nine_x, nine_z);
    f_255_19_multiplicative_inverse(nine_z);
    f_255_19_multiply(nine_x, nine_z);
    if(nine_x[0] != UINT64_C(0x20d342d51873f1b7) ||
       nine_x[1] != UINT64_C(0xd9750c687d157114) ||
       nine_x[2] != UINT64_C(0x8f3f5ced1e350b5c) ||
       nine_x[3] != UINT64_C(0x5cae469cdd684efb)){
      printf("ERROR: Curve25519: P+P with wrong value.\n");
      printf("EXPECTED: 0x20d342d51873f1b7 0xd9750c687d157114"
	     " 0x8f3f5ced1e350b5c 0x5cae469cdd684efb\n");
      printf("FOUND:    0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64"\n",
	     nine_x[0], nine_x[1], nine_x[2], nine_x[3]);
      error = true;
    }
  }
  if(!error){
    uint64_t nine_x[4], nine_z[4];
    nine_x[0] = nine_x[1] = nine_x[2] = 0x0; nine_x[3] = 0x9;
    nine_z[0] = nine_z[1] = nine_z[2] = 0x0; nine_z[3] = 0x1;
    curve25519_double(nine_x, nine_z);
    curve25519_double(nine_x, nine_z);
    f_255_19_multiplicative_inverse(nine_z);
    f_255_19_multiply(nine_x, nine_z);
       
    if(nine_x[0] != UINT64_C(0x79ce98b7e0689d7d) ||
       nine_x[1] != UINT64_C(0xe7d1d074a15b315f) ||
       nine_x[2] != UINT64_C(0xfe1805dfcd5d2a23) ||
       nine_x[3] != UINT64_C(0x0fee85e4550013ef)){
      printf("ERROR: Curve25519: P+P with wrong value.\n");
      printf("EXPECTED: 0x79ce98b7e0689d7d e7d1d074a15b315f"
	     " 0xfe1805dfcd5d2a23 0x0fee85e4550013ef\n");
      printf("FOUND:    0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64"\n",
	     nine_x[0], nine_x[1], nine_x[2], nine_x[3]);
      error = true;
    }
  }
  if(!error){
    uint64_t nine_x[4], nine_z[4];
    nine_x[0] = nine_x[1] = nine_x[2] = 0x0; nine_x[3] = 45;
    nine_z[0] = nine_z[1] = nine_z[2] = 0x0; nine_z[3] = 5;
    curve25519_double(nine_x, nine_z);
    curve25519_double(nine_x, nine_z);
    f_255_19_multiplicative_inverse(nine_z);
    f_255_19_multiply(nine_x, nine_z);
    if(nine_x[0] != UINT64_C(0x79ce98b7e0689d7d) ||
       nine_x[1] != UINT64_C(0xe7d1d074a15b315f) ||
       nine_x[2] != UINT64_C(0xfe1805dfcd5d2a23) ||
       nine_x[3] != UINT64_C(0x0fee85e4550013ef)){
      printf("ERROR: Curve25519: P+P with inconsistent projection.\n");
      printf("EXPECTED: 0x79ce98b7e0689d7d e7d1d074a15b315f"
	     " 0xfe1805dfcd5d2a23 0x0fee85e4550013ef\n");
      printf("FOUND:    0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64"\n",
	     nine_x[0], nine_x[1], nine_x[2], nine_x[3]);
      error = true;
    }
  }
  assert("Unit Test: 'curve25519_double'", !error);
}

void test_curve25519_add(void){
  bool error = false;
  uint64_t x1[4], z1[4], x2[4], z2[4], x3[4], z3[4];
  x1[0] = 0x0f580b4177e2c37f; z1[0] = 0x0;
  x1[1] = 0x831226edd7270db2; z1[1] = 0x0;
  x1[2] = 0x7514a6b3a2d31848; z1[2] = 0x0;
  x1[3] = 0x0f7650f75149badd; z1[3] = 0x1;
  x2[0] = 0x0; z2[0] = 0x0;
  x2[1] = 0x0; z2[1] = 0x0;
  x2[2] = 0x0; z2[2] = 0x0;
  x2[3] = 0x1; z2[3] = 0x0;
  x3[0] = 0x0f580b4177e2c37f; z3[0] = 0x0;
  x3[1] = 0x831226edd7270db2; z3[1] = 0x0;
  x3[2] = 0x7514a6b3a2d31848; z3[2] = 0x0;
  x3[3] = 0x0f7650f75149badd; z3[3] = 0x1;
  curve25519_add(x1, z1, x2, z2, x3, z3);
  f_255_19_multiplicative_inverse(z1);
  f_255_19_multiply(x1, z1);
  if(x1[0] != UINT64_C(0x0f580b4177e2c37f) ||
     x1[1] != UINT64_C(0x831226edd7270db2) ||
     x1[2] != UINT64_C(0x7514a6b3a2d31848) ||
     x1[3] != UINT64_C(0x0f7650f75149badd)){
    printf("ERROR: Curve25519: P+0 not equal P.\n");
    error = true;
  }
  if(!error){
    // 0x0f580b4177e2c37f.831226edd7270db2.7514a6b3a2d31848.0f7650f75149badd
    x1[0] = 0x0f580b4177e2c37f; z1[0] = 0x0;
    x1[1] = 0x831226edd7270db2; z1[1] = 0x0;
    x1[2] = 0x7514a6b3a2d31848; z1[2] = 0x0;
    x1[3] = 0x0f7650f75149badd; z1[3] = 0x1;
    // 0x7b6432b138bc13de.a68f3d22a8cee56e.c803fca707c3bb95.059cf1dec5932453
    x2[0] = 0x7b6432b138bc13de; z2[0] = 0x0;
    x2[1] = 0xa68f3d22a8cee56e; z2[1] = 0x0;
    x2[2] = 0xc803fca707c3bb95; z2[2] = 0x0;
    x2[3] = 0x059cf1dec5932453; z2[3] = 0x1;
    // 0x22835a4b30e7ee30.e5912d70a8d6efb6.0b56a03c04ad2299.99187f7bc2255858
    x3[0] = 0x22835a4b30e7ee30; z3[0] = 0x0;
    x3[1] = 0xe5912d70a8d6efb6; z3[1] = 0x0;
    x3[2] = 0x0b56a03c04ad2299; z3[2] = 0x0;
    x3[3] = 0x99187f7bc2255858; z3[3] = 0x1;
    
    curve25519_add(x1, z1, x2, z2, x3, z3);
    f_255_19_multiplicative_inverse(z1);
    f_255_19_multiply(x1, z1);

    if(x1[0] != UINT64_C(0x3178d08461f18c88) ||
       x1[1] != UINT64_C(0xe01d27b3acfd7990) ||
       x1[2] != UINT64_C(0xac8ab8a270d31dbf) ||
       x1[3] != UINT64_C(0x5bca11b4e194cbf1)){
      printf("ERROR: Curve25519: P+Q with wrong result.\n");
      printf("Expected: 0x3178d08461f18c88 0xe01d27b3acfd7990 0xac8ab8a270d31dbf 0x5bca11b4e194cbf1\n");
      printf("FOUND:    0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64"\n",
	     x1[0], x1[1], x1[2], x1[3]);
      error = true;
    }
  }
  assert("Unit Test: 'curve25519_add'", !error);
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
  test_f_255_19_multiplicative_inverse();
  test_curve25519_double();
  test_curve25519_add();
  //test_initialization();
  imprime_resultado();
  return 0;
}
