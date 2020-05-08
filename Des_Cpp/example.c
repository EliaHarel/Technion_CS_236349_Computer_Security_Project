#include "des_noop.h"

#define pp(x) printf("%10s = %02X %02X %02X %02X %02X %02X %02X %02X\n", \
	  #x, x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);


main()
{
  static unsigned char key[8] = { 44, 55, 66, 77, 88, 99, 111, 122 };
  static unsigned char plain[8] = { 144, 155, 166, 177, 188, 229, 211, 212 };
  unsigned char cipher[8];
  unsigned char p2[8];
  
  DES_encrypt(plain, key, cipher);
  DES_decrypt(cipher, key, p2);
  
  pp(key);
  pp(plain);
  pp(cipher);
  pp(p2);
}
