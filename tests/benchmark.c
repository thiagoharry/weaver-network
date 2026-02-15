#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#ifdef __linux__
#include <sys/random.h>
#endif
#include<time.h>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

#include "../src/network.c"

#ifdef __OpenBSD__
size_t getrandom(unsigned long long *buffer, size_t size, int flag){
  arc4random_buf(buffer, size);
  return size;
}
#endif

static inline double
timespec_diff_sec(const struct timespec *start,
                  const struct timespec *end)
{
    time_t sec  = end->tv_sec  - start->tv_sec;
    long   nsec = end->tv_nsec - start->tv_nsec;

    if (nsec < 0) {
        sec--;
        nsec += 1000000000L;
    }

    return (double)sec + (double)nsec * 1e-9;
}



void test_f_255_19_add(void){
  int i, j;
  uint64_t a[4];
  ssize_t result;
  struct timespec begin, end;


  // Warm up:
  for(i = 0; i < 100; i ++){
   do{
      result = getrandom(a, sizeof(uint64_t) * 4, 0);
    } while(result != sizeof(uint64_t) * 4);
    f_255_19_normalize(a);
    f_255_19_multiplicative_inverse(a);
  }
  
  // Measuring for real:
  for(i = 0; i < 1000; i ++){
    do{
      result = getrandom(a, sizeof(uint64_t) * 4, 0);
    } while(result != sizeof(uint64_t) * 4);
    f_255_19_normalize(a);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &begin);
    f_255_19_multiplicative_inverse(a);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    double elapsed = timespec_diff_sec(&begin, &end);
    printf("%lx %f\n", a, elapsed);
  }
 
}

int main(int argc, char **argv){
  test_f_255_19_add();
  
}
