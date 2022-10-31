#include <stdint.h>
#include <stdio.h>
uint8_t get_3_leftmost_bits(uint32_t x) {
  uint8_t a;
  a = x >> 29;
  return a;
}

int main(int argc, char *argv[]){
  uint32_t x= 4294967295 ;
  uint8_t a = get_3_leftmost_bits(x);
  printf("%d\n", a);
}
