/* NOTES:
   each digit of hex number represents 4 bits in binary so alt_16 will be a 4 digit hex number, alt_8 will be a 2 digit hex number
   to convert to signed 2's complement, invert all bits of the binary number, and then add 1 after inversion.
   so e.g:
      45 degrees = 0x2D i.e. 00101101
     -45 degrees = 0xD3 i.e. 11010011
*/

#include "sys/alt_stdio.h"
#include "system.h"
#include "io.h"

#define mem_limit 1024
alt_16 diag_dist;			 // diagonal distance of obstacle to rover
alt_8 angle;                // angle of obstacle from rover. to left of rover: +ve, to right of rover: -ve
alt_8 detected;    // 1 only if obstacle has been detected, 0 otherwise

void updateTmp(alt_32 base_addr, alt_16 diag_dist, alt_8 angle, alt_8 detected){
	alt_32 tmp = (diag_dist<<16) + (angle<<8) + detected;
	IOWR(base_addr, 0, tmp);
}
int main()
{ 
  alt_putstr("Hello from Nios II!\n");

  // EDIT VALUES OF PARAMETERS HERE as rover moves around.
  diag_dist = 0x0503;			       // diagonal distance of obstacle to rover in cm
  angle = 0xa6;                  // angle of obstacle from rover. to left of rover: +ve, to right of rover: -ve
  detected = 0x01;      // 1 only if obstacle has been detected, 0 otherwise

  alt_putstr("Writing into memory...\n");

  // writes into the 1st word of I2C MEM
  updateTmp(I2C_MEM_BASE, diag_dist, angle, detected);\
  
  alt_putstr("Memory write done!\n");


  while (1);

  return 0;
}
